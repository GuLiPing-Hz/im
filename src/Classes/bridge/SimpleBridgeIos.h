#include "../wrap/wrap_config.h"
#ifndef COCOS_PROJECT

#ifndef SIMPLE_BRIDGE_IOS_H__
#define SIMPLE_BRIDGE_IOS_H__

#include "SimpleBridge.h"
#include <functional>

typedef std::function<void(const std::string&,const std::string&)> FUNCIOS;

/*
 这个类用于ios的回调
 */
class SimpleBridgeIos : public SimpleBridge{
    SimpleBridgeIos();
    virtual ~SimpleBridgeIos();
public:
    static SimpleBridgeIos* getInstance();
    
    void setIosBridge(FUNCIOS& bridge){mIosBridge = bridge;}
    
protected:
    virtual void callNative(const std::string& method,const std::string& param);
    
protected:
    FUNCIOS mIosBridge;
};

#endif//SIMPLE_BRIDGE_IOS_H__

#endif//COCOS_PROJECT

