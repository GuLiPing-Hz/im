#ifndef ROOMDATADECODER__H__
#define ROOMDATADECODER__H__
#include "../wrap/data_decoder.h"

class RoomDataDecoder : public NetworkUtil::DataDecoder
{
public:
	RoomDataDecoder() : NetworkUtil::DataDecoder(NetworkUtil::PROTOCOLTYPE_BINARY, NetworkUtil::HEADER_LEN_2) {}
	virtual ~RoomDataDecoder(){}
	virtual int onPackage(NetworkUtil::ClientSocketBase *pClient, const char* buf, unsigned int buflen);
	static RoomDataDecoder* Instance()
	{
		static RoomDataDecoder dd;
		return &dd;
	};
};
#endif//ROOMDATADECODER__H__
