#include "buffer.h"

Buffer::Buffer(int initBuffSize) : buffer(initBuffSize), callback(nullptr)
{
    readPos = writePos = cnt = 0;
}

Buffer::Buffer(size_t th, std::function<void()> cb, int iniBuffSize) 
    : buffer(iniBuffSize), threshold(th)
{
    readPos = writePos = cnt = 0;
    callback = std::make_unique<std::function<void()>>(cb);
}

size_t Buffer::writableBytes() const
{
    return buffer.size() - writePos;
}

size_t Buffer::readableBytes() const
{
    return writePos - readPos;
}

size_t Buffer::prependableBytes() const
{
    return readPos;
}

const char *Buffer::peek() const
{
    return beginPtr() + readPos;
}

void Buffer::ensureWriteable(size_t len)
{
    if (writableBytes() < len)
        adjustSpace(len);
}

bool Buffer::retrieve(size_t len)
{
    if (len < 0)
        return false;

    if (readableBytes() <= len)
        retrieveAll();
    else
        readPos += len;
    return true;
}

bool Buffer::retrieveUntil(const char *end)
{
    if (beginPtr() + readPos > end || beginPtr() + writePos < end)
        return false;
    readPos = end - beginPtr();
    return true;
}

void Buffer::retrieveAll()
{
    readPos = writePos = 0;
}

std::string Buffer::retrieveAllToStr()
{
    std::string str(beginPtr() + readPos, readableBytes());
    retrieveAll();
    return std::move(str);
}

char *Buffer::beginWrite()
{
    return beginPtr() + writePos;
}

const char *Buffer::beginWriteConst() const
{
    return beginPtr() + writePos;
}

void Buffer::append(const std::string &str)
{
    append(str.c_str(), str.size());
}

void Buffer::append(const char *str, size_t len)
{
    append((void*)str, len);
}

void Buffer::append(const void *data, size_t len)
{
    ensureWriteable(len);
    std::copy((const char*)data, (const char*)data + len, beginWrite());
    writePos += len;
    checkThreshold();
}

void Buffer::append(const Buffer &buff)
{
    ensureWriteable(buff.readableBytes());
    std::copy(buff.beginPtr() + buff.readPos, buff.beginPtr() + buff.writePos, beginWrite());
    writePos += buff.readableBytes();
    checkThreshold();
}

ssize_t Buffer::readFd(int fd, int& eno)
{
    char buff[65535];
    struct iovec vec[2];
    vec[0].iov_base = beginWrite();
    vec[0].iov_len = writableBytes();
    vec[1].iov_base = buff;
    vec[1].iov_len = sizeof(buff);

    ssize_t n = readv(fd, vec, 2);
    eno = errno;

    if (n > 0)
    {
        size_t len = strlen(buff);
        if (0 == len)
            writePos += n;
        if (len > 0)
        {
            writePos = buffer.size();
            append(buff, len);
        }
        checkThreshold();
    }
    
    return n;
}

ssize_t Buffer::writeFd(int fd, int& eno)
{
    size_t r = readableBytes();
    ssize_t n = write(fd, peek(), r);
    eno = errno;
    if (n >= 0)
        readPos += n;
    return n;
}

void Buffer::setCallback(size_t th, std::function<void()> cb)
{
    threshold = th;
    callback = std::make_unique<std::function<void()>>(cb);
}

char *Buffer::beginPtr()
{
    return &(*buffer.begin());
}

const char *Buffer::beginPtr() const
{
    return &(*buffer.begin());
}

void Buffer::adjustSpace(size_t len)
{
    if (writableBytes() + prependableBytes() < len) // 空间不够，扩容
    {
        buffer.resize(writePos + len + 1);
        cnt = 0;
    }
    else 
    {
        size_t t = readableBytes();
        std::copy(beginPtr() + readPos, beginPtr() + writePos, beginPtr());
        writePos = t;
        readPos = 0;

        if (buffer.size() * 2 < buffer.capacity())
            ++cnt;
    }

    if (SCALEIN_THRESHOLD == cnt)
    {
        buffer.shrink_to_fit();
        cnt = 0;
    }
}

void Buffer::checkThreshold()
{
    if (nullptr == callback)
        return;
    
    if (readableBytes() >= threshold)
        (*callback)();
}
