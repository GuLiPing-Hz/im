#include "Tunnel.h"
#include "../wrap/crypt.h"
#include "../wrap/rw_stream.h"
#include "NetApp.h"

#include "../wrap/config.h"

#define MAXBUFSIZETEA 256

void Tunnel::closeSocket()
{
	m_bIsTokenLogin = false;
	memset(m_Sessionkey,0,sizeof(m_Sessionkey));
	ClientSocket::closeSocket();
}

bool Tunnel::SendRightBuf(NetworkUtil::BinaryWriteStream &stream)
{
	if(stream.getSize() < MAXBUFSIZETEA)
		return SendTEABuf(stream);
	return SendCompressBuf(stream);
}

bool Tunnel::SendTEABuf(NetworkUtil::BinaryWriteStream &stream)
{
	char outbuf[65535];                     
	int outbuflen = sizeof(outbuf);                 
	if(NetworkUtil::StreamEncrypt(stream.getData(),(int)stream.getSize(),outbuf,outbuflen,m_Sessionkey,1))
	{
		return sendBuf(outbuf,outbuflen);                                    
	}
	return false;
}

bool Tunnel::SendCompressBuf(NetworkUtil::BinaryWriteStream &stream)
{
	char outbuf[65535];                     
	int outbuflen = sizeof(outbuf);                 
	if(NetworkUtil::StreamCompress(stream.getData(),(int)stream.getSize(),outbuf,outbuflen))
	{
		return sendBuf(outbuf,outbuflen);                                     
	}
	return false;
}

bool Tunnel::onSocketConnect()
{
	if (NetApp::getInstance()->GetLobbyTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onLobbyTunnelConnectSuccess();
	else if (NetApp::getInstance()->GetRoomTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onRoomTunnelConnectSuccess();

	return true;
}

void Tunnel::onSocketConnectTimeout()
{
	if (NetApp::getInstance()->GetLobbyTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onLobbyTunnelConnectTimeout();
	else if (NetApp::getInstance()->GetRoomTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onRoomTunnelConnectTimeout();
}

void Tunnel::onSocketConnectError(int errCode)
{
	if (NetApp::getInstance()->GetLobbyTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onLobbyTunnelConnectError(errCode);
	else if (NetApp::getInstance()->GetRoomTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onRoomTunnelConnectError(errCode);
}

void Tunnel::onSocketClose()
{
	if (NetApp::getInstance()->GetLobbyTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onLobbyTunnelClose();
	else if (NetApp::getInstance()->GetRoomTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onRoomTunnelClose();
}

void Tunnel::onSocketRecvError(int errCode)
{
	LOGI("recv error, fd=%d, error=%d\n", getFD(), errCode);
	onNetLevelError(errCode);
}

void Tunnel::onSocketSendError(int errCode)
{
	LOGI("send error, fd=%d, error=%d\n", getFD(), errCode);
	onNetLevelError(errCode);
}

void Tunnel::onNetLevelError(int errCode)
{
	if (NetApp::getInstance()->GetLobbyTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onLobbyTunnelError(errCode);
	else if (NetApp::getInstance()->GetRoomTunnel() == this)
		NetApp::getInstance()->GetResponseHandler()->onRoomTunnelError(errCode);
}
