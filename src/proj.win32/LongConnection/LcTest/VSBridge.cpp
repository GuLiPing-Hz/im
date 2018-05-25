#include "VSBridge.h"
#include "LongConnection.h"
#include <memory>
#include <stdarg.h>
#include <windows.h>

VSBridge *VSBridge::getInstance() {
	static VSBridge sIns;
	return &sIns;
}

VSBridge::VSBridge(){

}

VSBridge::~VSBridge() {
}

#define APPKEY  "4ed38057df363e8355229ec53687c549"
#define UID1  "1000001"
#define TOKEN1  "018f0be0cb075ee761cf8d44be2b287e"
#define UID12  "1000012"
#define TOKEN12  "30f9f8feceee317a66df684915be86f9"


void VSBridge::callNative(const std::string& method, const std::string& param)
{
	LOGI("服务器回调数据 %s: %s|%s", __FUNCTION__, method.c_str(), param.c_str());
	rapidjson::Document paramDoc;
	paramDoc.Parse(param.c_str());
	if (paramDoc["code"] != 0)
		return;

	if (method == "onLobbyTunnelConnectSuccess"){

		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "login", allocator);

		mMyId = UID12;
		rapidjson::Value arg0(rapidjson::kStringType);
		arg0.SetString(mMyId.c_str(), mMyId.size(), allocator);
		root.AddMember("arg0", arg0, allocator);
		root.AddMember("arg1", TOKEN12, allocator);
		root.AddMember("arg2", APPKEY, allocator);

		callByNative(GetStrFromRoot(root));
	}
	else if (method == "login") {
		//LCLogout();
		
		mUsers.clear();
		//Sleep(3000);
		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "enterRoom", allocator);
		//root.AddMember("method", "exitRoom", allocator);
		root.AddMember("arg0", "1000", allocator); 
		callByNative(GetStrFromRoot(root));
	}
	else if (method == "sayTo") {
		LOGI("sayTo %s", param.c_str());
	}
	else if (method == "notify") {
	}
	else if (method == "enterRoom") {
		//LOGI("enterRoom %s", param.c_str());

		//rapidjson::Document doc;
		//rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		//rapidjson::Value root(rapidjson::kObjectType);
		//root.AddMember("method", "sayTo", allocator);
		//root.AddMember("arg0", "0", allocator);
		//root.AddMember("arg1", "1000001", allocator);
		//root.AddMember("arg2", "1000002", allocator);
		//root.AddMember("arg3", "123456", allocator);
		//root.AddMember("arg4", "", allocator);

		std::string roomId = paramDoc["arg0"].GetString();
		int type = -1;
		if (paramDoc.HasMember("arg2")) {
			type = paramDoc["arg2"].GetInt();
		}
		updateRoomUser(true, paramDoc["arg1"].GetString(), type);
		logRoomUser();

		//rapidjson::Document doc;
		//rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		//rapidjson::Value root(rapidjson::kObjectType);
		//root.AddMember("method", "exitRoom", allocator);

		//callByNative(GetStrFromRoot(root));
	}
	else if (method == "exitRoom") {
		updateRoomUser(false, paramDoc["arg0"].GetString(), -1);

		logRoomUser();
	}
	else if (method == "roomUser") {
		rapidjson::Value::Array arra = paramDoc["arg1"].GetArray();
		for (int i = 0; i < arra.Size(); i++) {
			auto dataArray = arra[i].GetArray();
			std::string uid = dataArray[0].GetString();
			int type = dataArray[1].GetInt();
			updateRoomUser(true, uid, type);
		}

		logRoomUser();
	}
}

void VSBridge::updateRoomUser(bool isEnter, std::string uid, int type) {
	if (isEnter) {
		if (uid == mMyId)
			return;

		User user;
		user.uid = uid;
		user.type = type;
		mUsers.insert(std::make_pair(uid, user));

		std::string stype = "未知";
		if (type == 0)
			stype = "普通用户";
		else if (type == 1)
			stype = "机器人";
		LOGI("用户 【uid=%s(%s)】进入房间", uid.c_str(), stype.c_str());
	}
	else {
		mUsers.erase(uid);
		LOGI("用户 【uid=%s】离开房间", uid.c_str());
	}
}

void VSBridge::logRoomUser() {
	MAPUSER::iterator it = mUsers.begin();
	for (; it != mUsers.end(); it++) {
		std::string type = "未知";
		if (it->second.type == 0)
			type = "普通用户";
		else if (it->second.type == 1) 
			type = "机器人";

		LOGI("房间用户 【uid=%s(%s)】", it->first.c_str(), type.c_str());
	}
}



