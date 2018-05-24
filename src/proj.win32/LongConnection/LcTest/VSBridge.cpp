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
	if (method == "onLobbyTunnelConnectSuccess"){

		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "login", allocator);
		root.AddMember("arg0", UID12, allocator);
		root.AddMember("arg1", TOKEN12, allocator);
		root.AddMember("arg2", APPKEY, allocator);

		callByNative(GetStrFromRoot(root));

	}
	else if (method == "login") {
		//LCLogout();
		if (paramDoc["code"] != 0)
			return;

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

		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "exitRoom", allocator);

		callByNative(GetStrFromRoot(root));
	}
	else if (method == "exitRoom") {
	}
}



