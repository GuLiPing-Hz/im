#include <assert.h>
#include "DataDecoderRoom.h"
#include "NetApp.h"
#include "../wrap/mutex_wrapper.h"
#include "../wrap/crypt.h"
#include "../wrap/config.h"
#include "ResponseBase.h"
#include <vector>

using namespace NetworkUtil;

int RoomDataDecoder::onPackage(NetworkUtil::ClientSocketBase *pClient, const char* buf, unsigned int buflen)
{
	//RoomTunnel *pRoomTunnel = (RoomTunnel*)pClient;

	return 0;
}
