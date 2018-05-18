#include "Tunnel.h"
#include "NetApp.h"

#include "../wrap/wrap_config.h"

#define MAXBUFSIZETEA 256

void Tunnel::closeSocket()
{
	m_bIsTokenLogin = false;
	memset(m_Sessionkey,0,sizeof(m_Sessionkey));
	ClientSocket::closeSocket();
}

bool Tunnel::onSocketConnect()
{
	if (NetApp::GetInstance()->getLobbyTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onLobbyTunnelConnectSuccess();
	else if (NetApp::GetInstance()->getRoomTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onRoomTunnelConnectSuccess();

	return true;
}

void Tunnel::onSocketConnectTimeout()
{
	if (NetApp::GetInstance()->getLobbyTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onLobbyTunnelConnectTimeout();
	else if (NetApp::GetInstance()->getRoomTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onRoomTunnelConnectTimeout();
}

void Tunnel::onSocketConnectError(int errCode)
{
	if (NetApp::GetInstance()->getLobbyTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onLobbyTunnelConnectError(errCode);
	else if (NetApp::GetInstance()->getRoomTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onRoomTunnelConnectError(errCode);
}

void Tunnel::onSocketClose()
{
	if (NetApp::GetInstance()->getLobbyTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onLobbyTunnelClose();
	else if (NetApp::GetInstance()->getRoomTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onRoomTunnelClose();
}

void Tunnel::onSocketRecvError(int errCode)
{
	LOGI("recv error, fd=%d, error=%d", getFD(), errCode);
	onNetLevelError(errCode);
}

void Tunnel::onSocketSendError(int errCode)
{
	LOGI("send error, fd=%d, error=%d", getFD(), errCode);
	onNetLevelError(errCode);
}

void Tunnel::onNetLevelError(int errCode)
{
	if (NetApp::GetInstance()->getLobbyTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onLobbyTunnelError(errCode);
	else if (NetApp::GetInstance()->getRoomTunnel() == this)
		NetApp::GetInstance()->getResponseHandler()->onRoomTunnelError(errCode);
}
