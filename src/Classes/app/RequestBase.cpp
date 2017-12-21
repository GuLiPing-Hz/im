/*************************************
*请求处理类 源文件
*************************************/
#include "../wrap/config.h"
#include "RequestBase.h"
#include <string.h>
#include "NetApp.h"
#include "Tunnel.h"
#include <string.h>
#include "../wrap/ext/thread.h"

//解决windonws 头文件冲突 或者先引入winsock2
// #define _WINSOCKAPI_  
// #include <Windows.h>  
// #include <Winsock2.h> 

#define HONGBAO_ID_LEN 51

ThreadWrapper* handleNetThread = NULL;
//static int s_login_way = 0;

bool RunClientThreadFunc(void* lpParam)
{
	return NetApp::GetInstance()->start();
}

void OnThreadStart(unsigned int threadid)
{
	//保存线程ID到g_NetApp中
	NetApp::GetInstance()->m_nThreadId = threadid;
    
#if defined(NETUTIL_ANDROID) && !defined(COCOS_PROJECT)
    cocos2d::JniHelper::attachCurThread(threadid);
#endif
}

void OnTreadEnd(unsigned int threadid)
{
#if defined(NETUTIL_ANDROID) && !defined(COCOS_PROJECT)
    cocos2d::JniHelper::detachCurThread(threadid);
#endif
}

void RequestBase::setResponse(ResponseBase* resp){
	NetApp::GetInstance()->setResponseHandler(resp);
}

/************************************************************************/
/*			add by jinguanfu 2010/3/2 <begin> 回调API改造成接口			*/
int RequestBase::startClient()
{
	LOGI("RequestBase::StartClient\n");
    
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0) 
		return -1;
#endif

	if(handleNetThread != NULL)
	{
		stopClient(false);
	}

	handleNetThread = ThreadWrapper::CreateThread(RunClientThreadFunc,NULL,kNormalPriority,"Net Thread");
	handleNetThread->SetCallBack(OnThreadStart,OnTreadEnd);
	unsigned int thread_id;
	if(!handleNetThread || !handleNetThread->Start(thread_id))
		return -1;
    m_bInit = true;
	return 0;
}

void RequestBase::stopClient(bool finish)
{
	NetApp::GetInstance()->stop();
	if(handleNetThread)
	{
		if(!handleNetThread->WaitFor(3000))
		{
			handleNetThread->Terminate(0);
		}
		delete handleNetThread;
		handleNetThread = NULL;
	}

	if (finish){
#ifdef _WIN32
		WSACleanup();
#endif
		NetApp::ReleaseApp();
	}
}

const char* RequestBase::getToken()
{
	return NetApp::GetInstance()->getToken();
}

unsigned int RequestBase::getTokenlen()
{
	return NetApp::GetInstance()->getTokenlen();
}

//大厅是否连接
bool RequestBase::isConnectLobby()
{
	return NetApp::GetInstance()->getLobbyTunnel()->isConnected();
}

//房间是否连接
bool RequestBase::isConnectRoom()
{
	return NetApp::GetInstance()->getRoomTunnel()->isConnected();
}

//大厅是否token
bool RequestBase::isTokenLoginLobby()
{
	return NetApp::GetInstance()->getLobbyTunnel()->IsTokenLogin();
}

//房间是否token
bool RequestBase::isTokenLoginRoom()
{
	return NetApp::GetInstance()->getRoomTunnel()->IsTokenLogin();
}

int RequestBase::connectLobby(const char* host,short port,int timeout)
{
    if(!m_bInit)
    {
        LOGE("NetUtil is not initialize\n");
        return -1;
    }

	int len = sizeof(DataConnect);
	DataConnect data;
	StrLCpy(data.ip, host, sizeof(data.ip));
	data.port = port;
	data.timeout = timeout;
	int ret = NetApp::GetInstance()->postMessageNoBack(eServerID::none, nullptr, MSG_CONNECT_LOBBY, &data, len, 0);
	if (ret == -1){
		return -1;
	}
	return ret;
}
int RequestBase::disConnectLobby()
{
	return NetApp::GetInstance()->postMessageNoBack(eServerID::none, nullptr, MSG_DISCONNECT_LOBBY, 0);
}
int RequestBase::connectRoom(const char* host,short port,int type,int timeout)
{
    if(!m_bInit)
    {
        LOGI("NetUtil is not initialize\n");
        return -1;
    }
    
	return 0;
}
int RequestBase::disConnectRoom()
{
	return 0;
}

void RequestBase::setSeqIsBack(int seq)
{
	NetApp::GetInstance()->delTimeout(seq);
}

int RequestBase::sendMsgToLobby(const char* msg, unsigned int len, int seq, bool needBack){
	if (seq < 0)
		return -1;

	int ret = NetApp::GetInstance()->postMessage(eServerID::lobby, NetApp::GetInstance()->getLobbyTunnel()
		, Wrap::MSG_SEND_DATA, (void*)msg, len, seq, needBack);
	return ret;
}

