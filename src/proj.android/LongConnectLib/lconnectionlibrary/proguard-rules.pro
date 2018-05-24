# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile

#Gson混淆
-keepattributes Signature
-keep class sun.misc.Unsafe { *; }
-keep class com.google.gson.examples.android.model.** { *; }

#LConnection
-keep public class simple.bean.** {*;}
-keep public class simple.util.bridge.JsAndroidBridge {*;}
-keep public class simple.config.LConnection {*;}
-keep public class simple.config.LCRequest {*;}
-keep public class simple.config.LCResponse {*;}

#保留注释
-keepattributes *Annotation*
-keep class * extends java.lang.annotation.Annotation { *; }
