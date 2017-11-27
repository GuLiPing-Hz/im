#include "VSBridge.h"
#include "LongConnection.h"
#include <memory>
#include <stdarg.h>

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

void VSBridge::callNative(const std::string& method, const std::string& param)
{
	if (method == "onLobbyTunnelConnectSuccess"){

		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "login", allocator);
		root.AddMember("arg0", UID1, allocator);
		root.AddMember("arg1", TOKEN1, allocator);
		root.AddMember("arg2", APPKEY, allocator);

		callByNative(GetStrFromRoot(root));

	}
	else if (method == "login") {
		//LCLogout();

		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "enterRoom", allocator);
		root.AddMember("arg0", "1000001", allocator);
		callByNative(GetStrFromRoot(root));
	}
	else if (method == "sayTo") {
		LOGI("sayTo %s\n", param.c_str());
	}
	else if (method == "notify") {
	}
	else if (method == "enterRoom") {
		LOGI("enterRoom %s\n", param.c_str());

		rapidjson::Document doc;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator = doc.GetAllocator();
		rapidjson::Value root(rapidjson::kObjectType);
		root.AddMember("method", "sayTo", allocator);
		root.AddMember("arg0", "0", allocator);
		root.AddMember("arg1", "1000001", allocator);
		root.AddMember("arg2", "1000002", allocator);
		root.AddMember("arg3", "123456", allocator);
		root.AddMember("arg4", "", allocator);
		callByNative(GetStrFromRoot(root));
	}
	else if (method == "exitRoom") {
	}
}



