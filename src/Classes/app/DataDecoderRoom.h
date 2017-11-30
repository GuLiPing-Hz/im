#ifndef ROOMDATADECODER__H__
#define ROOMDATADECODER__H__
#include "../wrap/data_decoder.h"

class RoomDataDecoder : public Wrap::DataDecoder
{
public:
	RoomDataDecoder() : Wrap::DataDecoder(Wrap::PROTOCOLTYPE_BINARY, Wrap::HEADER_LEN_2) {}
	virtual ~RoomDataDecoder(){}
	virtual int onPackage(Wrap::ClientSocketBase *pClient, const char* buf, unsigned int buflen);
	static RoomDataDecoder* Instance()
	{
		static RoomDataDecoder dd;
		return &dd;
	};
};
#endif//ROOMDATADECODER__H__
