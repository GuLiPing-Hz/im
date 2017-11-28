#ifndef NATIVEBUFFER__H__
#define NATIVEBUFFER__H__

#include "../wrap/data_block.h"
#include "../wrap/byte_order.h"
#include "../wrap/config.h"
#include <string>
#include <vector>
#include "../wrap/pool.h"

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
#ifdef COCOS_PROJECT
protected:
#else
public:
#endif
	NativeBuffer(char format = OP_BIGENDIAN);
	virtual ~NativeBuffer();

#ifdef COCOS_PROJECT
public:
	static NativeBuffer *Create();
#endif
	//读写位置归0
	void clearBuffer();

	void moveBuffer(char *&data, unsigned int len);

	const NetworkUtil::DataBlockLocal65535* getBuffer() const;

	bool hasData();

	bool skipBuffer(const unsigned int len);

	/**
	 *
	 * @param c
	 * @return 是否写入成功
	 */
	template<typename T>
	bool writeType(const T c);

	template<typename T>
	bool readType(T &c);

	bool writeChar(const char c);

	bool writeUChar(const unsigned char c);

	bool writeShort(const short c);

	bool writeUShort(const unsigned short c);

	bool writeInt(const int c);

	bool writeUInt(const unsigned int c);

	bool writeInt64(const long long c);

	bool writeUInt64(const unsigned long long c);

	bool writeFloat(const float c);

	bool writeString(const unsigned short len, const char *c);

	bool writeStringNoLen(const unsigned short len, const char *c);

	bool readChar(char &c);

	bool readUChar(unsigned char &c);

	bool readShort(short &c);

	bool readUShort(unsigned short &c);

	bool readInt(int &c);

	bool readUInt(unsigned int &c);

	bool readInt64(long long &c);

	bool readUInt64(unsigned long long &c);

	bool readFloat(float &c);

	bool readString(unsigned short &len, char *c);

	bool readBuffer(char *c, unsigned int len);

	std::string readString();

private:
	NetworkUtil::DataBlockLocal65535 data_;
	int swap_;

	//记录读取位置
	unsigned int readPos_;
};

typedef std::vector<std::string> VECSTRING;

struct BufferJson /*: PoolObj*/{
	enum eDataType {
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
	eDataType type;

	struct Data{
		union BaseData {//联合数据结构
			char c;
			short s;
			int i;
			long long ll;
			float f;
		};
		BaseData base;
		//字符串需要另外存放
		std::string str;
	};
	Data data;

	typedef std::vector<BufferJson*> VECTORBJ;
	VECTORBJ list;

	//BufferJson() :PoolObj("BufferJson"){}

// 	virtual void reuse(){
// 		PoolObj::reuse();
// 		clear();
// 	}

	void clear(){
		type = type_unknow;
		list.clear();
	}
};

//自动解析网络的协议数据
BufferJson* AutoParseNativeBufferEx(NativeBuffer *nativeBuf);
//调用上面的函数需要配对调用释放函数
void RecycleBufferList(BufferJson* &bj);

std::string XorString(const char *data, int datalen, const char *key, int len);

#endif//NATIVEBUFFER__H__
