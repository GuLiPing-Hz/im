#include "NetInformer.h"
#include "NetApp.h"

void NetInformer::dealCustomMsg(Wrap::MSGINFO* msg){
	if (msg){
		if (msg->cmd == MSG_CONNECT_LOBBY){
			clearCurWaitMsg();//清空当前的等待消息，理论上来说，我们上层应用在调用connect之前，最好先调用disconnect
			DataConnect* p = (DataConnect*)msg->v;
			NetApp::GetInstance()->getLobbyTunnel()->connectTo(p->ip, p->port, p->timeout);
		}
		else if (msg->cmd == MSG_DISCONNECT_LOBBY){
			NetApp::GetInstance()->getLobbyTunnel()->closeSocket();
		}
	}
}

