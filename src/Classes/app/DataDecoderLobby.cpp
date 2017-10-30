#include "DataDecoderLobby.h"
#include "NetApp.h"
#include "../wrap/mutex_wrapper.h"
#include "../wrap/crypt.h"

#include "../wrap/config.h"

using namespace NetworkUtil;

int LobbyDataDecoder::onPackage(ClientSocketBase *pClient,const char* buf,unsigned int buflen)
{
	//LobbyTunnel *pLobbyTunnel = (LobbyTunnel*)pClient;

	NetApp::getInstance()->GetResponseHandler()->onLobbyMsg(0, buf, buflen);

	return 0;
}
