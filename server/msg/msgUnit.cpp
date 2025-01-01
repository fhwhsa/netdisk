#include "msgUnit.h"

MsgUnit* MsgUnit::make_dataunit(MsgType msgType, uint msgLen, const char *msg)
{
    // 判断数据类型是否合法
    if (msgType >= msgTypeNum || msgType < 0)
        return nullptr;

    uint totalLen = sizeof(MsgUnit) + msgLen + 1;   // 留空一个字节
    MsgUnit *munit = (MsgUnit*)malloc(totalLen);
    if (!munit)
    {
        fprintf(stderr, "Failed to malloc!\n");
        return nullptr;
    }
    munit->totalLen = totalLen;
    munit->msgType = msgType;
    munit->msgLen = msgLen + 1;
    memset(munit->msg, 0, munit->msgLen);
    memcpy(munit->msg, msg, msgLen);

    return munit;
}