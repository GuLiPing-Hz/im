#ifndef LOBBYDATADECODER__H__
#define LOBBYDATADECODER__H__
/*
	注释添加以及修改于 2014-4-3

	实现大厅网络数据的解析
	相关协议在 protocal.h中
*/
#include "../wrap/data_decoder.h"

class LobbyDataDecoder : public Wrap::DataDecoder
{
public:
	LobbyDataDecoder() : Wrap::DataDecoder(Wrap::PROTOCOLTYPE_BINARY_BIG, Wrap::HEADER_LEN_2){}
	virtual ~LobbyDataDecoder(){}
	virtual int onPackage(Wrap::ClientSocketBase *pClient, const char* buf, unsigned int buflen);
	static LobbyDataDecoder* Instance()
	{
		static LobbyDataDecoder dd;
		return &dd;
	};
};
#endif//LOBBYDATADECODER__H__
