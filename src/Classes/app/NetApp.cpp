#include "NetApp.h"
#include "DataDecoderLobby.h"
#include "DataDecoderRoom.h"
#include "../wrap/crypt.h"

#ifdef _WIN32
#include <Windows.h>
#endif

float g_Version = 1.00;

#ifndef max
#define max(a, b)            (((a) > (b)) ? (a) : (b))
#endif//max
#ifndef min
#define min(a, b) \
(((a)>(b))?(b):(a))
#endif//min

void KeepLiveLobbyAndRoom() {
    //心跳包时间计数
    static int time_old = 0;
    static int time_new = 0;
    time_new = (int) time(NULL);
    if (15 < (time_new - time_old)) {
        time_old = time_new;
        //业务
        if (NetApp::getInstance()->GetRoomTunnel()->IsTokenLogin()) {
            //Room服务器 心跳包
// 			NetApp::getInstance()->postMessage((int)VATYPE_ROOMKEEPALIVE, NULL);
// 			NetApp::getInstance()->GetInformer()->Inform();
        }

        if (NetApp::getInstance()->GetLobbyTunnel()->IsTokenLogin()) {
            //Lobby服务器 心跳包
// 			NetApp::getInstance()->postMessage((int)VATYPE_LOBBYKEEPALIVE, NULL);
// 			NetApp::getInstance()->GetInformer()->Inform();
        }
    }
}

NetApp *NetApp::getInstance() {
    static NetApp ins;
    return &ins;
}

NetApp::NetApp()
        : m_LobbyTunnel(&m_Reactor),
          m_RoomTunnel(&m_Reactor),
          m_RDMap(&m_Reactor),
#ifdef TEST_UDP
        m_Informer(&m_Reactor),
#else
          m_Informer(this),
#endif
          m_bIsAuth(false),
          m_iMyIDx(0),
          m_pResponse(NULL),
          m_nThreadId(0),
          m_Tokenlen(0) {
    //注册空闲处理默认心跳包的回调事件。
    m_Reactor.mFuncKeepLive = KeepLiveLobbyAndRoom;

    m_pObjectCS = CriticalSectionWrapper::CreateCriticalSection();
    assert(m_pObjectCS != NULL);
}

NetApp::~NetApp() {
    if (m_pObjectCS)
        delete m_pObjectCS;
}

void NetApp::SetToken(const char *token, unsigned int tokenlen) {
    m_bIsAuth = true;
    memset(m_Token, 0, sizeof(m_Token));
    int len = min(sizeof(m_Token) - 1, tokenlen);
    memcpy(m_Token, token, len);
    m_Tokenlen = len;
}

//NetworkUtil::MessageCenter
int NetApp::getMessage(NetworkUtil::MSGINFO &msg) {
    CriticalSectionScoped lock(m_pObjectCS);
    if (!m_requestlist.empty()) {
        msg = m_requestlist.front();
        m_requestlist.pop_front();
        return 0;
    }
    return -1;
}

int NetApp::sendToSvr(NetworkUtil::ClientSocket *pSvr, const char *buf, int len) {
    if (pSvr && !pSvr->isConnected())
        return -1;

    return pSvr->sendBuf(buf, len) ? 0 : -1;
}

//请求线程
void NetApp::addTimeout(int seq, NetworkUtil::ReserveData *data) {
    m_RDMap.put(seq, data);
}

//网络线程
void NetApp::delTimeout(int seq) {
    NetworkUtil::ReserveData **data = m_RDMap.get(seq);
	if (data && *data) {
		free(*data);//释放内存
		m_RDMap.del(seq);//移除
    }
}

//网络线程
void NetApp::onTimeout(NetworkUtil::ReserveData *data) {
    if (data) {
		int seq = data->seq;

        if (data->serverid == eServerID::lobby) {//给对应的连接构造一条超时回调。
			GetResponseHandler()->onLobbyMsg(data->type == NetworkUtil::ReserveData::TYPE_TIMEOUT
				? RESULT_TIMEOUT : RESULT_REQ_NOT_SEND, nullptr, 0, seq);
        }

		free(data);
		m_RDMap.del(seq);//移除
    }
}

void NetApp::setHeartbeatFunc(NetworkUtil::RUNKEEPLIVE func) {
    m_Reactor.mFuncKeepLive = func;
}

bool NetApp::Start() {
    m_bIsAuth = false;
    int id = 0;

    m_LobbyTunnel.setTimerID(id++);
    m_LobbyTunnel.setDecoder(LobbyDataDecoder::Instance());

    m_RoomTunnel.setTimerID(id++);
    m_RoomTunnel.setDecoder(RoomDataDecoder::Instance());

    m_RDMap.setTimerID(id++);
	m_RDMap.registerTimer(3);//3秒检查一次

//    if (!m_gMgr.initDownload())
//        return false;

    if (m_Informer.init() != 0)
        return false;
    return m_Reactor.run();
}

void NetApp::Stop() {
    m_Reactor.stop();
    m_Informer.unInit();
}

int NetApp::postMessage(int serverId, NetworkUtil::ClientSocket *conn, int cmd, void *v, int len,
                        int seq, bool back) {
    CriticalSectionScoped lock(m_pObjectCS);
    if (m_requestlist.size() > 1000)//请求队列最多1000
        return -1;

    NetworkUtil::MSGINFO msg = {0};
	msg.server = serverId;
    msg.con = conn;
    msg.cmd = cmd;
    msg.v = v;
    msg.len = len;
    msg.back = back;
    msg.wseq = seq;// m_Counter.Get();
    m_requestlist.push_back(msg);

    //通知消息处理器处理
    m_Informer.inform();
    return seq;
}

