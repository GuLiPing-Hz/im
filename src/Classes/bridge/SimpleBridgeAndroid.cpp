#include "SimpleBridgeAndroid.h"
#include "../jni/JniHelper.h"
#include "../app/NetApp.h"

SimpleBridgeAndroid *SimpleBridgeAndroid::getInstance() {
    static SimpleBridgeAndroid sIns;
    return &sIns;
}

SimpleBridgeAndroid::SimpleBridgeAndroid()
        : SimpleBridge(), mCs(nullptr) {
    //mCs = CriticalSectionWrapper::CreateCriticalSection();
}

SimpleBridgeAndroid::~SimpleBridgeAndroid() {
    if (mCs) {
        delete mCs;
        mCs = nullptr;
    }
}

void SimpleBridgeAndroid::callNative(const std::string &method, const std::string &param) {
   cocos2d::JniHelper::callStaticStringMethod(NetApp::GetInstance()->m_nThreadId,
                                              "simple.util.bridge.JsAndroidBridge", "callNative",
                                              method, param);

//     cocos2d::JniHelper::callStaticVoidMethod(NetApp::getInstance()->m_nThreadId,
//                                              "simple.util.bridge.JsAndroidBridge", "callNative2",
//                                              method, param);
}



