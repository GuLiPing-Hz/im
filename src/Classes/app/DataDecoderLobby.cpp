#include "DataDecoderLobby.h"
#include "NetApp.h"
#include "../wrap/mutex.h"

#include "../wrap/config.h"

using namespace Wrap;

int LobbyDataDecoder::onPackage(ClientSocketBase *pClient,const char* buf,unsigned int buflen)
{
	//LobbyTunnel *pLobbyTunnel = (LobbyTunnel*)pClient;

	NetApp::GetInstance()->getResponseHandler()->onLobbyMsg(0, buf, buflen);

	return 0;
}
