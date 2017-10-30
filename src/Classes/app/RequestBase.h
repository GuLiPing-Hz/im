/*************************************
*请求处理类 头文件
*Create: 2009/12/21   jinguanfu 
*************************************/
#ifndef REQUESTBASE_H
#define REQUESTBASE_H

#include "ResponseBase.h"

class  RequestBase
{	
public:
	RequestBase(void):m_bInit(false){};
	~RequestBase(void){};

	void setResponse(ResponseBase* resp);

	//启动客户端
	int startClient();
	//停止客户端
	void stopClient(bool finish=true);
	//取得Token
	const char* getToken();
	//取得Token长度
	unsigned int getTokenlen();

	//大厅是否连接
	bool isConnectLobby();
	//房间是否连接
	bool isConnectRoom();
	//大厅是否token
	bool isTokenLoginLobby();
	//房间是否token
	bool isTokenLoginRoom();

	/************************************************************************/
	/* @return -1 失败，其他成功，返回队列中的seqId                                              */
	/************************************************************************/
	int connectLobby(const char* host,short port,int timeout = 60);
	int disConnectLobby();
	int connectRoom(const char* host,short port,int type,int timeout = 60);
	int disConnectRoom();

	void setSeqIsBack(int seq);
	int sendMsgToLobby(const char* msg, unsigned int len, int seq, bool needBack);

private:
    bool m_bInit;
};

#endif

