﻿#ifndef NETAPP__H__
#define NETAPP__H__

#include "../wrap/reactor.h"
#include "../wrap/counter.h"
#include "NetInformer.h"
#include "ResponseBase.h"
#include <assert.h>
#include "Tunnel.h"
#include "../charset/CharsetCodec.h"
#include "../wrap/http_download_mgr.h"

extern float g_Version;

enum eServerID {
	none,
	lobby,
};

class NetApp : public Wrap::MessageCenter {
private:
	NetApp();

	virtual ~NetApp();

public:
	static NetApp *GetInstance();
	static void ReleaseApp();

	//Wrap::MessageCenter
	virtual Wrap::ThreadInformer* getInformer();

	virtual void onTimeoutData(Wrap::ReserveData *data);

	void setHeartbeatFunc(Wrap::RUNKEEPLIVE func);

	bool start();

	void stop();

	LobbyTunnel *getLobbyTunnel() { return &m_LobbyTunnel; }

	RoomTunnel *getRoomTunnel() { return &m_RoomTunnel; }

	void setToken(const char *token, unsigned int tokenlen);

	const char *getToken() { return m_Token; }

	unsigned int getTokenlen() { return m_Tokenlen; }

	void setMyIDx(int idx) { m_iMyIDx = idx; }

	const int getMyIDx() { return m_iMyIDx; }

	void setResponseHandler(ResponseBase *pRb) { m_pResponse = pRb; }

	ResponseBase *getResponseHandler() { return m_pResponse; }

	//投递一条不需要回复的，data为空的消息
	int postMessageNoBack(int serverId, Wrap::ClientSocket *conn, int cmd, int seq) {
		return Wrap::MessageCenter::postMessage(serverId, conn, cmd, NULL, 0, seq, false);
	}
	//投递一条不需要回复的消息
	int postMessageNoBack(int serverId, Wrap::ClientSocket *conn, int cmd, void *v,int len, int seq) {
		return Wrap::MessageCenter::postMessage(serverId, conn, cmd, v, len, seq, false);
	}

	inline bool isAuth() { return m_bIsAuth; }

private:
	Wrap::NetReactor m_Reactor;
	LobbyTunnel m_LobbyTunnel;
	RoomTunnel m_RoomTunnel;
	Wrap::Counter m_Counter;
	char m_Token[256];
	int m_iMyIDx;
	unsigned int m_Tokenlen;
	NetInformer m_Informer;
	std::list<Wrap::MSGINFO> m_requestlist;
	std::list<int> m_wseq;
	bool m_bIsAuth;
	ResponseBase *m_pResponse;    //回调响应基类
public:
	unsigned int m_nThreadId;
	//    Wrap::CHttpDownloadMgr m_gMgr;
};

#endif//NETAPP__H__
