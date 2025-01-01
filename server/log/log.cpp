#include "log.h"

using namespace std;
namespace fs = filesystem;

int Log::BUFFER_SIZE = 1024 * 4;
int Log::MAX_LOG_FILE_SIZE = 1024 * 1024 * 4;

void Log::init(LogLevel l, std::string p, LogMode m, int mqs)
{
    if (hasInit)
    {
        cerr << "The log has been initialized" << endl;
        return;
    }

    {
        unique_lock<mutex> lock(mtx);
        level = l;
        basePath = p;
        maxQueueSize = mqs;
        mode = m;
    }
    createResources();
    hasInit = true;
}

void Log::initFromConfig(Json::Value &root)
{
    if (hasInit)
    {
        cerr << "The log has been initialized" << endl;
        return;
    }
    
    if (!root.isMember("log"))
        throw std::runtime_error("Profile is missing value 'log'!");
    
    auto check = [&](const std::string& key) {
        if (!root["log"].isMember(key))
            throw std::runtime_error("The log configuration is missing the value '" + key + "'");
    };
    
    for (const std::string& key : {"level", "path", "mode"})
        check(key);
    
    int tmp_level = root["log"]["level"].asInt();
    int tmp_mode = root["log"]["mode"].asInt();

    assert(-1 < tmp_level && tmp_level < 4);
    assert(-1 < tmp_mode && tmp_mode < 2);
    
    {
        unique_lock<mutex> lock(mtx);
        level = static_cast<LogLevel>(tmp_level);
        mode = static_cast<LogMode>(tmp_mode);
        basePath = root["log"]["path"].asString();
        if (LogMode::ASYNC == mode)
            maxQueueSize = root["log"]["maxQueueSize"].asInt();    
    }
    createResources();
    hasInit = true;
}

void Log::writeLog(LogLevel l, const char *format, ...)
{
    if (l < level)
        return;

    va_list args;
    va_start(args, format);
    int needed = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (needed <= 0)
        return;
    char* tmp = (char*)malloc(needed + 1 + 7 + 19 + 3);
    if (nullptr == tmp)
    {
        cerr << "Memory allocation failed." << endl;
        return;
    }

    strcpy(tmp, getLogPrefix(l).c_str());
    strcpy(tmp + 7, getCurrDateAndTime().c_str());
    strcpy(tmp + 26, " - ");
    va_start(args, format);
    int len = vsprintf(tmp + 29, format, args);
    va_end(args);

    if (len <= 0)
    {
        free(tmp);
        return;
    }

    {
        len += 29;
        tmp[len++] = '\n';
        if (writeErrStream || LogLevel::ERROR == l)
            fprintf(stdout, "%.*s", len, tmp);
        lock_guard<mutex> lock(mtx);
        buff->append(tmp, len);
    }

    free(tmp);
}

Log &Log::getInstance()
{
    static Log instance;
    return instance;
}

LogLevel Log::getLevel()
{
    lock_guard<mutex> lock(mtx);
    return level;
}

void Log::setLevel(LogLevel l)
{
    lock_guard<mutex> lock(mtx);
    level = l;
}

void Log::enableWriteErrorStream()
{
    writeErrStream = true;
}

void Log::disableWriteErrorStream()
{
    writeErrStream = false;
}

Log::Log()
{
    buff = nullptr;
    q = nullptr;
    t = nullptr;
    basePath = "";
    maxQueueSize = -1;
    fd = -1;
    fileNo = 0;
    hasInit = false;
    writeErrStream = false;
}

Log::~Log()
{
    resetResources();
}

std::string Log::getLogPrefix(LogLevel l)
{
    switch (l)
    {
    case LogLevel::DEBUG:
        return "[DEBUG]";
    case LogLevel::INFO:
        return "[INFO ]";
    case LogLevel::WARN:
        return "[WARN ]";
    case LogLevel::ERROR:
        return "[ERROR]";
    default:
        return "[INFO ]";
    }
}

void Log::asyncThreadFunction()
{
    getInstance().asyncWriteLog();
}

void Log::asyncWriteLog()
{
    string str = "";
    while (q->pop(str))
    {
        lock_guard<mutex> lock(mtx);
        if (updateFile())
            write(fd, str.c_str(), str.size());
        else; // 错误处理
    }
}

std::string Log::getCurrDate() const 
{
    auto now = chrono::system_clock::now();
    std::time_t now_tt = chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_tt);

    std::stringstream now_date;
    now_date << std::put_time(now_tm, "%Y-%m-%d");

    return now_date.str();
}

std::string Log::getCurrTime() const 
{
    auto now = chrono::system_clock::now();
    std::time_t now_tt = chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_tt);

    std::stringstream now_time;
    now_time << std::put_time(now_tm, "%H-%M-%S");

    return now_time.str();
}

std::string Log::getCurrDateAndTime() const
{
    auto now = chrono::system_clock::now();
    std::time_t now_tt = chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_tt);

    std::stringstream nowdt;
    nowdt << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

    return nowdt.str();
}

void Log::createResources()
{
    unique_lock<mutex> lock(mtx);

    assert(basePath != "");
    if (LogMode::ASYNC == mode)
        assert(maxQueueSize > 0);

    resetResources();

    if (LogMode::ASYNC == mode)
    {
        q = make_unique<BlockQueue<string>>(maxQueueSize);

        buff = make_unique<Buffer>(BUFFER_SIZE, [this]() {
            // 达到触发门槛将内容放入阻塞队列
            q->push(buff->retrieveAllToStr());
        });

        t = make_unique<thread>(asyncThreadFunction);
    }

    else 
    {
        buff = make_unique<Buffer>(BUFFER_SIZE, [this]() {
            updateFile();
            if (fd > 0)
            {
                write(fd, buff->peek(), buff->readableBytes());
                buff->retrieveAll();
            }        
        });
    }

    assert(updateFile());
}

bool Log::updateFile()
{
    string tmpDate = getCurrDate();
    string path = basePath + "/" + tmpDate;
    if (!fs::exists(path))
    {
        try {
            fs::create_directories(path);
        } catch (const fs::filesystem_error& e) {
            cerr << "Log file creation error: " << e.what() << endl;
            return false;
        }
    }

    if (fd <= 0 || date != tmpDate || lseek(fd, 0, SEEK_CUR) >= MAX_LOG_FILE_SIZE)
    {
        string filePath = path + "/" + to_string(++fileNo) + "_" + getCurrTime() + ".log";
        fd = open(filePath.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (-1 == fd)
        {
            perror("Error opening log file: ");
            return false;
        }
        
        date = tmpDate;
    }

    return true;
}

void Log::resetResources()
{
    while (t && t->joinable())
    {
        while (!q->empty())
            q->flush();
        q->close();
        t->join();
    }

    if (buff && buff->readableBytes() > 0 && fd > 0)
    {
        write(fd, buff->peek(), buff->readableBytes());
        buff->retrieveAll();
        close(fd);
    }

    q.reset();
    t.reset();
    buff.reset();
    q = nullptr;
    t = nullptr;
    buff = nullptr;
    
    fd = -1;
    fileNo = 0;

    hasInit = false;
}
