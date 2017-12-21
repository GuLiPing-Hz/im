#include "NetApp.h"
#include "DataDecoderLobby.h"
#include "DataDecoderRoom.h"
#include "../wrap/pool.h"

#ifdef _WIN32
#include <Windows.h>
#endif

float g_Version = 1.00;

void KeepLiveLobbyAndRoom() {
    //心跳包时间计数
    static int time_old = 0;
    static int time_new = 0;
    time_new = (int) time(NULL);
    if (15 < (time_new - time_old)) {
        time_old = time_new;
        //业务
        if (NetApp::GetInstance()->getRoomTunnel()->IsTokenLogin()) {
            //Room服务器 心跳包
// 			NetApp::getInstance()->postMessage((int)VATYPE_ROOMKEEPALIVE, NULL);
// 			NetApp::getInstance()->GetInformer()->Inform();
        }

        if (NetApp::GetInstance()->getLobbyTunnel()->IsTokenLogin()) {
            //Lobby服务器 心跳包
// 			NetApp::getInstance()->postMessage((int)VATYPE_LOBBYKEEPALIVE, NULL);
// 			NetApp::getInstance()->GetInformer()->Inform();
        }
    }
}

NetApp *NetApp::GetInstance() {
    static NetApp ins;
    return &ins;
}

void NetApp::ReleaseApp(){
	CCharsetCodec::UninitCharset();
	Wrap::PoolMgr::ReleaseIns();//反初始化字符转换器
}

NetApp::NetApp()
	:Wrap::MessageCenter(&m_Reactor)
	, m_LobbyTunnel(&m_Reactor)
	, m_RoomTunnel(&m_Reactor)
#ifdef TEST_UDP
	,m_Informer(&m_Reactor)
#else
	, m_Informer(this)
#endif
	, m_bIsAuth(false)
	, m_iMyIDx(0)
	, m_pResponse(NULL)
	, m_nThreadId(0)
	, m_Tokenlen(0)
{
    //注册空闲处理默认心跳包的回调事件。
    m_Reactor.mFuncKeepLive = KeepLiveLobbyAndRoom;
}

NetApp::~NetApp() {
}

void NetApp::setToken(const char *token, unsigned int tokenlen) {
    m_bIsAuth = true;
    memset(m_Token, 0, sizeof(m_Token));
    int len = MIN(sizeof(m_Token) - 1, tokenlen);
    memcpy(m_Token, token, len);
    m_Tokenlen = len;
}

//Wrap::MessageCenter
Wrap::ThreadInformer* NetApp::getInformer(){
	return &m_Informer;
}

//网络线程
void NetApp::onTimeoutData(Wrap::ReserveData *data) {
    if (data) {
		int seq = data->seq;
        if (data->serverid == eServerID::lobby) {//给对应的连接构造一条超时回调。
			getResponseHandler()->onLobbyMsg(data->type == Wrap::ReserveData::TYPE_TIMEOUT
				? RESULT_TIMEOUT : RESULT_REQ_NOT_SEND, nullptr, 0, seq);
        }
    }
}

void NetApp::setHeartbeatFunc(Wrap::RUNKEEPLIVE func) {
    m_Reactor.mFuncKeepLive = func;
}

bool NetApp::start() {
    m_bIsAuth = false;
    int id = 0;

    m_LobbyTunnel.setTimerID(id++);
    m_LobbyTunnel.setDecoder(LobbyDataDecoder::Instance());

    m_RoomTunnel.setTimerID(id++);
    m_RoomTunnel.setDecoder(RoomDataDecoder::Instance());

	setTimerID(id++);
	registerTimer(3);//3秒检查一次

//    if (!m_gMgr.initDownload())
//        return false;

    if (m_Informer.init() != 0)
        return false;
    return m_Reactor.run();
}

void NetApp::stop() {
    m_Reactor.stop();
    m_Informer.unInit();
}


