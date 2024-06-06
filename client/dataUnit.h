/**
 * 通信数据单元
*/

#ifndef _DATAUNIT_T_
#define _DATAUNIT_T_


typedef unsigned int uint;

struct DataUnit
{
    // 消息类型
    static int DATA_TYPE_TEXT;    /*普通文本*/
    static int DATA_TYPE_LOGIN;   /*登陆类型*/
    static int DATA_TYPE_NUM;     /*类型数量*/


    uint totalLen;          // 数据单元大小
    uint dataType;          // 消息类型
    char fileName[64];      // 可选字段，文件名
    uint dataLen;           // 消息大小
    int data[];             // 消息内容

    /**
     * 描述：构造一个DataUnit
     * 参数：
     *      dataType     消息类型
     *      dataLen      消息大小
     *      data         消息内容
     *      name         可选字段，文件名
     * 返回：
     *      成功          返回DataUnit指针，指向新创建的对象
     *      失败          空指针
    */
    static DataUnit* make_dataunit(uint dataType, uint dataLen, const char *data, char *name = nullptr);
};

#endif
