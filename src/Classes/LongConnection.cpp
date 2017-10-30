#include "LongConnection.h"
#include "app/RequestBase.h"
#include "wrap/counter.h"
#include "bridge/NativeBuffer.h"
#include "protocol.h"

static RequestBase gReq;
static Counter gCounter;

void LCSetResponse(ResponseBase *resp) {
    gReq.setResponse(resp);
}

int LCStartClient() {
    return gReq.startClient();
}

void LCStopClient(const bool finish) {
    gReq.stopClient(finish);
}

bool LCIsConnected() {
    return gReq.isConnectLobby();
}

int LCConnect(const char *host, const short port, const int timeout) {
    return gReq.connectLobby(host, port, timeout);
}

int LCDisconnect() {
    return gReq.disConnectLobby();
}

int LCGetSeq() {
    return gCounter.get();
}

void LCSetSeqIsBack(const int seq) {
    gReq.setSeqIsBack(seq);
}

int LCSendMsg(const char *msg, const unsigned long len, const int seq, const bool needBack) {
	//LOGD("________Send Msg: msg[%ld]=[%s]\n", len, ByteString(msg, len));
    
    return gReq.sendMsgToLobby(msg, len, seq, needBack);
}

NativeBuffer *BuildNativeBuffer(short cmd, short seq) {
	static NativeBuffer nativeBuf;//静态对象，防止内存抖动厉害
	nativeBuf.clearBuffer();

    nativeBuf.writeShort(cmd);//cmd
    nativeBuf.writeShort(seq);//seq
    nativeBuf.writeShort(0);//ret 标志位

    return &nativeBuf;
}

extern char NetXorKey[];

std::string FinishNativeBufferW(NativeBuffer *nativeBuf) {
    if (nativeBuf) {
        const NetworkUtil::DataBlockLocal65535* db = nativeBuf->getBuffer();
        std::string xorStr = XorString(db->getBuf(), db->getPos(), NetXorKey, strlen(NetXorKey));

        nativeBuf->clearBuffer();//清空数据
        nativeBuf->writeString(xorStr.length(), xorStr.c_str());//写入最后的字符串

        std::string ret(db->getBuf(), db->getPos());
        return ret;
    } else {
        return "";
    }
}

/*
登录
@param uid
@param token
@param appkey
@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1
*/
int LCLogin(const int seq, const char *uid, const char *token, const char *appkey) {
    if (!uid || !token || !appkey)
        return -1;

    NativeBuffer *nativeBuf = BuildNativeBuffer(CMD_LOGIN_C2S2C, seq);
	bool ok = nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
    ok &= nativeBuf->writeString(strlen(uid), uid);
	ok &= nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
	ok &= nativeBuf->writeString(strlen(token), token);
	ok &= nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
	ok &= nativeBuf->writeString(strlen(appkey), appkey);
	if (!ok)
		return -1;

    std::string ret = FinishNativeBufferW(nativeBuf);
    if (ret.empty())
        return -1;
    return LCSendMsg(ret.c_str(), ret.length(), seq, true);
}

/*
登出
@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1

可以不用理会登出的返回
*/
int LCLogout() {
    NativeBuffer *nativeBuf = BuildNativeBuffer(CMD_LOGOUT_C2S2C, 0);
    std::string ret = FinishNativeBufferW(nativeBuf);
    if (ret.empty())
        return -1;
    return LCSendMsg(ret.c_str(), ret.length(), 0, false);
}

/*
发言
@param type
@param from
@param to
@param content
@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1

注意:需要先登陆
*/
int LCSayTo(const int seq, int type, const char *from, const char *to, const char *content,
            const char *ext) {
    if (!from || !to || !content)
        return -1;

    NativeBuffer *nativeBuf = BuildNativeBuffer(CMD_SAYTO_C2S2C, seq);
    bool ok = nativeBuf->writeChar(BufferUnit::type_int);
	ok &= nativeBuf->writeInt(type);
	ok &= nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
	ok &= nativeBuf->writeString(strlen(from), from) > 0;
	ok &= nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
	ok &= nativeBuf->writeString(strlen(to), to);
	ok &= nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
	ok &= nativeBuf->writeString(strlen(content), content);
	ok &= nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
	ok &= nativeBuf->writeString(ext ? strlen(ext) : 0, ext);
	if (!ok)
		return -1;

    std::string ret = FinishNativeBufferW(nativeBuf);
	if (ret.empty())
        return -1;
    return LCSendMsg(ret.c_str(), ret.length(), seq, true);
}

/*
进入房间
@param room_id
@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1

@注意:需要先登陆
*/
int LCEnterRoom(const int seq, const char *room_id) {
    if (!room_id)
        return -1;

    NativeBuffer *nativeBuf = BuildNativeBuffer(CMD_ENTERROOM_C2S2C, seq);
	bool ok = nativeBuf->writeChar(BufferUnit::type_array + BufferUnit::type_char);
    ok &= nativeBuf->writeString(strlen(room_id), room_id);
	if (!ok)
		return -1;

    std::string ret = FinishNativeBufferW(nativeBuf);
    if (ret.empty())
        return -1;
    return LCSendMsg(ret.c_str(), ret.length(), seq, true);
}

/*
离开房间
*/
int LCExitRoom() {
    NativeBuffer *nativeBuf = BuildNativeBuffer(CMD_EXITROOM_C2S2C, 0);
    std::string ret = FinishNativeBufferW(nativeBuf);
    if (ret.empty())
        return -1;
    return LCSendMsg(ret.c_str(), ret.length(), 0, false);
}

int LCHeartbeat() {
    LOGI("%s\n", __FUNCTION__);
    NativeBuffer *nativeBuf = BuildNativeBuffer(CMD_KEEPALIVE_C2S, 0);
    std::string ret = FinishNativeBufferW(nativeBuf);
    if (ret.empty())
        return -1;
    return LCSendMsg(ret.c_str(), ret.length(), 0, false);
}

