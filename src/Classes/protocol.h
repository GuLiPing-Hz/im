#ifndef PROTOCOL_H__20170930
#define PROTOCOL_H__20170930

enum eCMD{
	/*
	C->S:
	string uid    用户ID
	string token  Token
	string appkey 应用Key
	S->C: 空
	*/
	CMD_LOGIN_C2S2C = 1,//登录

	/*
	C->S: 空
	S->C: 空 客户端不理会没事
	*/
	CMD_LOGOUT_C2S2C = 2,//登出

	/*
	C->S: 一样
	S->C: 一样
	char    type       type==0时,to_uid表示是用户id，type==1时,to_uid表示是群id
	string from_uid   发言者 0 表示系统,其他表示用户ID
	string to_uid     发言者对象
	string content    json字符串
	string ext        json字符串
	*/
	CMD_SAYTO_C2S2C = 3,//发言

	/*
	S->C:
	string from_uid   0 表示系统,其他表示用户ID
	string content    json字符串
	*/
	CMD_NOTIFY_S2C = 4,//通知消息

	/*
	C->S:
	string room_id  房间ID

	S->C:
	string room_id  房间ID
	string uid
	*/
	CMD_ENTERROOM_C2S2C = 5,//进入房间

	/*
	C->S: 空
	S->C:
	string uid
	*/
	CMD_EXITROOM_C2S2C = 6,//离开房间

	/*
	C->S: 空
	*/
	CMD_KEEPALIVE_C2S = 7,//心跳包

	/*
	S->C: 空
	*/
	CMD_DRIVEAWAY_S2C = 8,//踢人下线
};

#endif // PROTOCOL_H__20170930
