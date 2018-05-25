#ifndef VSBridge_H__
#define VSBridge_H__

#include <string>
#include <map>
#include "bridge/SimpleBridge.h"

struct User {
	std::string uid;
	int type;
};
typedef std::map<std::string, User> MAPUSER;

class VSBridge : public  SimpleBridge
{
protected:
	VSBridge();
	virtual ~VSBridge();
public:
	static VSBridge *getInstance();

	virtual void callNative(const std::string& method, const std::string& param);

	void updateRoomUser(bool isEnter, std::string uid, int type);
	void logRoomUser();

	std::string mMyId;
	MAPUSER mUsers;
};


#endif//VSBridge_H__
