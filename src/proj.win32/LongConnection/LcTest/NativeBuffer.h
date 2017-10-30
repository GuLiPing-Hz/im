#ifndef NATIVEBUFFER__H__
#define NATIVEBUFFER__H__

#include "wrap/data_block.h"
#include "wrap/byte_order.h"
#include "wrap/config.h"
#include <string>
#include <vector>

#ifdef COCOS_PROJECT
#include "cocos2d.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"

std::string jsval_to_std_string_len(JSContext *cx, JS::HandleValue arg);
void register_NativeBuffer(JSContext *cx, JS::HandleObject ns);

#endif

/*
大端buffer
*/
class NativeBuffer
#ifdef COCOS_PROJECT
    : public cocos2d::Ref
#endif
{
protected:
    NativeBuffer(char format = OP_BIGENDIAN);
    virtual ~NativeBuffer();

public:
    static NativeBuffer *Create();

#ifndef COCOS_PROJECT
private:
    int mRef;
public:
    void release();

#endif

    //读写位置归0
    void clearBuffer();

    void moveBuffer(char *&data, unsigned int len);

    const NetworkUtil::DataBlock &getBuffer();

    bool hasData();

    bool skipBuffer(const unsigned int len);

    template<typename T>
    bool writeType(const T c);

    template<typename T>
    bool readType(T &c);

    bool writeChar(const char c);

    bool writeShort(const short c);

    bool writeInt(const int c);

    bool writeInt64(const long long c);

    bool writeFloat(const float c);

    bool writeString(const short len, const char *c);

    bool writeStringNoLen(const short len, const char *c);

    bool readChar(char &c);

    bool readShort(short &c);

    bool readInt(int &c);

    bool readInt64(long long &c);

    bool readFloat(float &c);

    bool readString(short &len, char *c);

    bool readBuffer(char *c, unsigned int len);

    std::string readString();

private:
    NetworkUtil::DataBlock data_;
    int swap_;

    //记录读取位置
    unsigned int readPos_;
};


#define TYPE_ARRA_MASK 0x10

struct BufferUnit {
    enum DataType {
        type_unknow = 0,
        type_char = 1,//[16+1][数组长度][data]
        type_short = 2,
        type_int = 3,
        type_int64 = 4,
        type_float = 5,//[]
        //17需要单独拎出来
                type_str = 17,

        //下面两个主要给BufferUnitArra使用
                type_custom = 6,//[16+6][数组长度][{结构长度,结构体}]...
        type_array = 16,//
    };

    bool isSingle;//true 为BufferUnitSingle ，false 为BufferUnitArra
    DataType type;
};

//自动化解析buffer基础单元
struct BufferUnitSingle : public BufferUnit {
    union Data {
        char c;
        short s;
        int i;
        long long ll;
        float f;
    };
    Data data;
    //字符串需要另外存放
    std::string str;
};

struct BufferUnitArra : public BufferUnit {
// 	type_custom = 6,//[16+6][数组长度][{结构长度,结构体}]...
// 	type_array = 16,//
    bool isInner;//是否是内嵌的数组 true 读取array ，false 读取 custom
    std::vector<BufferUnit *> base;//基础数组数据
    std::vector<BufferUnitArra *> data;//通用数据类型 嵌套的数组
};

typedef std::vector<BufferUnit *> VECBUNIT;
typedef std::vector<std::string> VECSTRING;

VECBUNIT AutoParseNativeBuffer(NativeBuffer *nativeBuf);

std::string XorString(const char *data, int datalen, const char *key, int len);

#endif//NATIVEBUFFER__H__
