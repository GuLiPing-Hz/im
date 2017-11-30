#ifndef TUNNEL__H__
#define TUNNEL__H__

#include "../wrap/client_socket.h"

class Tunnel : public Wrap::ClientSocket
{
public:
	virtual ~Tunnel(){}
	Tunnel(Wrap::Reactor *pReactor) : Wrap::ClientSocket(pReactor)
	{
		m_bIsTokenLogin = false;
		memset(m_Sessionkey, 0, sizeof(m_Sessionkey));
	}

	virtual void closeSocket();
	void closeConnect(){ closeSocket(); }
	inline bool IsTokenLogin(){
		return m_bIsTokenLogin;
	}
	inline void SetTokenLogin(bool bIsTokenLogin){
		m_bIsTokenLogin = bIsTokenLogin;
	}

public:
	virtual bool onSocketConnect();
	virtual void onSocketConnectTimeout();
	virtual void onSocketConnectError(int errCode);
	virtual void onSocketClose();
	virtual void onSocketRecvError(int errCode);
	virtual void onSocketSendError(int errCode);
	virtual void onNetLevelError(int errCode);

public:
	bool m_bIsTokenLogin;
	char m_Sessionkey[16];
};

class RoomTunnel: public Tunnel
{
public:
	RoomTunnel(Wrap::Reactor * pReactor):Tunnel(pReactor), m_connect_type(0){}

public:
	int m_connect_type;
};

typedef Tunnel LobbyTunnel;
typedef Tunnel AuthTunnel;
typedef Tunnel ChannellistTunnel;

#endif
