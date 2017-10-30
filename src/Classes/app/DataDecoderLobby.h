#ifndef LOBBYDATADECODER__H__
#define LOBBYDATADECODER__H__
/*
	注释添加以及修改于 2014-4-3

	实现大厅网络数据的解析
	相关协议在 protocal.h中
*/
#include "../wrap/data_decoder.h"

class LobbyDataDecoder : public NetworkUtil::DataDecoder
{
public:
	LobbyDataDecoder() : NetworkUtil::DataDecoder(NetworkUtil::PROTOCOLTYPE_BINARY_BIG, NetworkUtil::HEADER_LEN_2){}
	virtual ~LobbyDataDecoder(){}
	virtual int onPackage(NetworkUtil::ClientSocketBase *pClient, const char* buf, unsigned int buflen);
	static LobbyDataDecoder* Instance()
	{
		static LobbyDataDecoder dd;
		return &dd;
	};
};
#endif//LOBBYDATADECODER__H__
