#ifndef NETINFORMER_H__
#define NETINFORMER_H__

#include "../wrap/thread_informer.h"

enum eNetInformer{
	MSG_CONNECT_LOBBY = NetworkUtil::MSG_SEND_DATA+1,
	MSG_DISCONNECT_LOBBY,
};

struct DataConnect{
	short port;
	char ip[128];//支持ipv6
	short timeout;
};

class NetInformer : public NetworkUtil::ThreadInformer
{
public:
	NetInformer(NetworkUtil::MessageCenter* center):NetworkUtil::ThreadInformer(center){}
	virtual ~NetInformer(){}

protected:
	//处理 > MSG_SEND_DATA 的消息
	virtual void dealCustomMsg(NetworkUtil::MSGINFO* msg);
};

#endif//NETINFORMER_H__
