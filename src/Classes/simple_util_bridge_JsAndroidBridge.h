﻿/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class simple_util_bridge_JsAndroidBridge */

#ifndef _Included_simple_util_bridge_JsAndroidBridge
#define _Included_simple_util_bridge_JsAndroidBridge
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     simple_util_bridge_JsAndroidBridge
 * Method:    setNativeContext
 * Signature: (Landroid/content/Context;)V
 */
JNIEXPORT void JNICALL Java_simple_util_bridge_JsAndroidBridge_setNativeContext
  (JNIEnv *, jclass, jobject);

/*
 * Class:     simple_util_bridge_JsAndroidBridge
 * Method:    callCppFromNative
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_simple_util_bridge_JsAndroidBridge_callCppFromNative
  (JNIEnv *, jclass, jstring);

/*
 * Class:     simple_util_bridge_JsAndroidBridge
 * Method:    getStringFromCpp
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_simple_util_bridge_JsAndroidBridge_getStringFromCpp
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif