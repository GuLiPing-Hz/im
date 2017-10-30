#include "NetInformer.h"
#include "NetApp.h"

void NetInformer::dealCustomMsg(NetworkUtil::MSGINFO* msg){
	if (msg){
		if (msg->cmd == MSG_CONNECT_LOBBY){
			DataConnect* p = (DataConnect*)msg->v;
			NetApp::getInstance()->GetLobbyTunnel()->connectTo(p->ip, p->port, p->timeout);
		}
		else if (msg->cmd == MSG_DISCONNECT_LOBBY){
			NetApp::getInstance()->GetLobbyTunnel()->closeSocket();
		}
	}
}

