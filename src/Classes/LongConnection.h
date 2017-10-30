#ifndef LONGCONNECTION_H__
#define LONGCONNECTION_H__

class ResponseBase;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WIN32

#ifdef LONGCONNECTION_EXPORTS
#define DLL_LC __declspec(dllexport)
#else
#define DLL_LC __declspec(dllimport)
#endif // LONGCONNECTION_EXPORTS

#else

#define DLL_LC 

#endif // WIN32

	/*
	设置异步回调接口对象
	*/
	DLL_LC void LCSetResponse(ResponseBase* resp);

	/*
	启动网络客户端
	@return 0 启动成功,-1 启动失败
	*/
	DLL_LC int LCStartClient();

	/*
	停止网络客户端
	*/
	DLL_LC void LCStopClient(const bool finish);

	/*
	检查我们是否已经连接到服务器
	*/
	DLL_LC bool LCIsConnected();

	/*
	连接服务器
	@param host 域名或IP
	@param port 端口地址
	@param timeout 设定连接超时时间
	*/
	DLL_LC int LCConnect(const char* host, const short port, const int timeout);

	/*
	断开服务器连接
	@return 0 断开成功,-1 断开失败,需要重新断开
	*/
	DLL_LC int LCDisconnect();

	/*
	获取序列号
	*/
	DLL_LC int LCGetSeq();

	/*
	通知模块,该seq的请求已经得到回应
	@param seq 指定的序列号
	*/
	DLL_LC void LCSetSeqIsBack(const int seq);

	/*
	登录
	@param seq 请求序列号
	@param uid
	@param token
	@param appkey
	@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1
	*/
	DLL_LC int LCLogin(const int seq,const char* uid, const char* token, const char* appkey);

	/*
	登出
	@return 成功 返回0值 ; 失败 返回-1 

	可以不用理会登出的返回
	*/
	DLL_LC int LCLogout();

	/*
	发言
	@param seq 请求序列号
	@param type 0：点对点发消息，1：群内消息
	@param from
	@param to
	@param content attach
	@param ext 扩展内容
	@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1

	注意:需要先登陆
	*/
	DLL_LC int LCSayTo(const int seq, int type, const char* from, const char* to, const char* content, const char *ext);

	/*
	进入房间
	@param seq 请求序列号
	@param room_id
	@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1

	@注意:需要先登陆
	*/
	DLL_LC int LCEnterRoom(const int seq, const char* room_id);

	/*
	离开房间
	*/
	DLL_LC int LCExitRoom();

	/*
	心跳包
	*/
	DLL_LC int LCHeartbeat();

	/*
	发送信息给服务器
	@param msg 内容
	@param len msg内容长度
	@param seq 序列号  请使用LCGetSeq获取
	@param needBack 是否需要服务器给于回馈,

	@return 成功 返回seq值,我们的请求的序列号 ; 失败 返回-1

	@注意: 当设置needBack为true的时候.我们在消息返回的时候一定记得调用LCSetSeqIsBack,告诉模块消息已经返回,否则你将会收到超时的通知
	*/
	DLL_LC int LCSendMsg(const char* msg, const unsigned long len, const int seq, const bool needBack);
#ifdef __cplusplus
}
#endif

#endif//LONGCONNECTION_H__
