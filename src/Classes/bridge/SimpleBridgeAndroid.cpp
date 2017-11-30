#include "SimpleBridgeAndroid.h"
#include "../jni/JniHelper.h"
#include "../app/NetApp.h"

SimpleBridgeAndroid *SimpleBridgeAndroid::getInstance() {
    static SimpleBridgeAndroid sIns;
    return &sIns;
}

SimpleBridgeAndroid::SimpleBridgeAndroid()
        : SimpleBridge() {
}

SimpleBridgeAndroid::~SimpleBridgeAndroid() {
}

void SimpleBridgeAndroid::callNative(const std::string &method, const std::string &param) {
   Wrap::JniHelper::callStaticStringMethod(NetApp::GetInstance()->m_nThreadId,
                                              "simple.util.bridge.JsAndroidBridge", "callNative",
                                              method, param);

//     cocos2d::JniHelper::callStaticVoidMethod(NetApp::getInstance()->m_nThreadId,
//                                              "simple.util.bridge.JsAndroidBridge", "callNative2",
//                                              method, param);
}



