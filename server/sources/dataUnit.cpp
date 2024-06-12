#include "dataUnit.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DataUnit* DataUnit::make_dataunit(DataType dataType, uint dataLen, const char *data, char *name)
{
    // 判断数据类型是否合法
    if (dataType >= DataType::DATA_TYPE_NUM)
        return nullptr;
    if (nullptr != name && sizeof(name) >= 64)
        return nullptr;

    uint totalLen = sizeof(DataUnit) + dataLen + 1;
    DataUnit* dunit = (DataUnit*)malloc(totalLen);
    if (!dunit)
    {
        fprintf(stderr, "Failed to malloc!\n");
        return nullptr;
    }
    dunit->totalLen = totalLen;
    dunit->dataType = dataType;
    dunit->dataLen = dataLen + 1;

    memset(dunit->data, 0, dunit->dataLen);
    memcpy(dunit->data, data, dataLen);

    memset(dunit->fileName, 0, sizeof(dunit->fileName));
    if (nullptr != nullptr)
        memcpy(dunit->fileName, name, (size_t)sizeof(name));

    return dunit;
}
