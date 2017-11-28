#ifndef NETAPP__H__
#define NETAPP__H__

#include "../wrap/reactor.h"
#include "../wrap/counter.h"
#include "../wrap/seq_map.h"
#include "../wrap/mutex_wrapper.h"
#include "NetInformer.h"
#include "../wrap/tm_seqmap.h"
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

class NetApp : public NetworkUtil::MessageCenter {
private:
    NetApp();

    virtual ~NetApp();

public:
    static NetApp *GetInstance();
	static void ReleaseApp();

    //NetworkUtil::MessageCenter
    virtual int getMessage(NetworkUtil::MSGINFO &msg);

    virtual int sendToSvr(NetworkUtil::ClientSocket *pSvr, const char *buf, int len);

    virtual void addTimeout(int seq, NetworkUtil::ReserveData *data);

    virtual void delTimeout(int seq);

    virtual void onTimeout(NetworkUtil::ReserveData *data);

    void setHeartbeatFunc(NetworkUtil::RUNKEEPLIVE func);

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

    //处理消息的发送与接收
    int postMessage(int serverId, NetworkUtil::ClientSocket *conn, int cmd, void *v, int seq) {
        return postMessage(serverId, conn, cmd, v, 0, seq, false);
    }

    int
    postMessage(int serverId, NetworkUtil::ClientSocket *conn, int cmd, void *v, int len, int seq,
                bool back = true);

    NetInformer *getInformer() { return &m_Informer; }

    inline bool isAuth() { return m_bIsAuth; }

private:
    CriticalSectionWrapper *m_pObjectCS;
    NetworkUtil::NetReactor m_Reactor;
    LobbyTunnel m_LobbyTunnel;
    RoomTunnel m_RoomTunnel;
    Counter m_Counter;
    char m_Token[256];
    int m_iMyIDx;
    unsigned int m_Tokenlen;
    NetworkUtil::TMSeqMap m_RDMap;
    NetInformer m_Informer;
    std::list<NetworkUtil::MSGINFO> m_requestlist;
    std::list<int> m_wseq;
    bool m_bIsAuth;
    ResponseBase *m_pResponse;    //回调响应基类
public:
    unsigned int m_nThreadId;
//    NetworkUtil::CHttpDownloadMgr m_gMgr;
};

#endif//NETAPP__H__
