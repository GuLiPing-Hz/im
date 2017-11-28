#ifndef RESPONSEBASE__H__
#define RESPONSEBASE__H__
/*
	注释添加以及修改于 2014-4-8 

	声明一个接口类ResponseBase，用于对服务器数据返回的处理，
	用户根据自己业务的需要自己实现响应的方法。
*/

//请求超时
#define RESULT_TIMEOUT -9999
//解析服务器协议失败
#define RESULT_ERROR_PARSE -9998
//SDK内存溢出
#define RESULT_OOM -9997
//请求未送出，可能处于断网状态
#define RESULT_REQ_NOT_SEND -9996

#ifdef WIN32
#ifdef LONGCONNECTION_EXPORTS
#define IM_DLL __declspec(dllexport)
#else
#define IM_DLL __declspec(dllimport)
#endif // LONGCONNECTION_EXPORTS
#else
#define IM_DLL
#endif // WIN32

class IM_DLL ResponseBase
{
public:
	ResponseBase(void){};
	virtual ~ResponseBase(void){};

	/*
	成功连接大厅服务器
	*/
	virtual void onLobbyTunnelConnectSuccess() = 0;
	/*
	成功连接房间服务器
	*/
	virtual void onRoomTunnelConnectSuccess(){};
	
	/*
	连接大厅服务器超时
	*/
	virtual void onLobbyTunnelConnectTimeout() = 0;
	/*
	连接房间服务器超时
	*/
	virtual void onRoomTunnelConnectTimeout(){};

	/*
	连接大厅服务器错误
	@param code 错误码
	*/
	virtual void onLobbyTunnelConnectError(const int code) = 0;
	/*
	连接房间服务器错误
	@param code 错误码
	*/
	virtual void onRoomTunnelConnectError(const int code){};

	//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
	/*
	大厅服务器断开
	*/
	virtual void onLobbyTunnelClose() = 0;	//
	/*
	房间服务器断开
	*/
	virtual void onRoomTunnelClose(){};	//

	//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
	/*
	大厅服务器异常
	@param code 错误码
	*/
	virtual void onLobbyTunnelError(const int code) = 0;
	/*
	房间服务器异常
	@param code 错误码
	*/
	virtual void onRoomTunnelError(const int code){};
	
	/*
	收到来自大厅服务器的信息
	@param code 0 正常, OPTION_TIMEOUT 超时
	@param msg 信息内容
	@param len 信息长度
	*/
	virtual void onLobbyMsg(const int code, const char* msg, const unsigned int len, const int seq = 0) = 0;
};

#endif//RESPONSEBASE__H__
