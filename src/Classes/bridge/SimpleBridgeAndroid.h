#include "../wrap/config.h"
#ifndef COCOS_PROJECT

#ifndef SIMPLE_BRIDGE_ANDROID_H__
#define SIMPLE_BRIDGE_ANDROID_H__

#include "SimpleBridge.h"
#include "../wrap/mutex_wrapper.h"


/*
android callback
*/
class SimpleBridgeAndroid : public SimpleBridge{
	SimpleBridgeAndroid();
	virtual ~SimpleBridgeAndroid();
public:
	static SimpleBridgeAndroid* getInstance();

    CriticalSectionWrapper* getJavaCS(){return mCs;}
protected:
	virtual void callNative(const std::string& method,const std::string& param);

private:
    CriticalSectionWrapper* mCs;
};

#endif//SIMPLE_BRIDGE_ANDROID_H__

#endif//COCOS_PROJECT
