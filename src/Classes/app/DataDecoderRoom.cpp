#include <assert.h>
#include "DataDecoderRoom.h"
#include "NetApp.h"
#include "../wrap/mutex.h"
#include "../wrap/wrap_config.h"
#include "ResponseBase.h"
#include <vector>

using namespace Wrap;

int RoomDataDecoder::onPackage(Wrap::ClientSocketBase *pClient, const char* buf, unsigned int buflen)
{
	//RoomTunnel *pRoomTunnel = (RoomTunnel*)pClient;

	return 0;
}
