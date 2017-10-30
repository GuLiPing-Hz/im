#ifndef VSBridge_H__
#define VSBridge_H__

#include <string>
#include <map>
#include "bridge/SimpleBridge.h"

class VSBridge : public  SimpleBridge
{
protected:
	VSBridge();
	virtual ~VSBridge();
public:
	static VSBridge *getInstance();

	virtual void callNative(const std::string& method, const std::string& param);
};


#endif//VSBridge_H__
