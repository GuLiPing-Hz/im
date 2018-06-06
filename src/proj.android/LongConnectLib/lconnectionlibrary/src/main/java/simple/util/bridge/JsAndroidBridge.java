package simple.util.bridge;

import android.content.Context;
import android.util.Log;

import java.io.UnsupportedEncodingException;

import simple.bean.CallNativeArg;
import simple.util.gson.GsonUtils;

/**
 * Created by JJ on 2017/4/10.
 * <p>
 * 打开终端，
 * <p>
 * 1 javac -d class StructInfo.java CJniUtil.java   	 #指定生成路径
 * 2 javah -bootclasspath "D:\CocosStore\Android-SDK\platforms\android-25\android.jar" -classpath . simple.util.bridge.JsAndroidBridge	 #指定类路径
 * <p>
 * <p>
 * #javap -classpath class -s com.jni.netutil.CJniUtil >zCJniUtilSig.txt 查看某个类的具体jni标记
 */

public class JsAndroidBridge {

    static final String TAG = "JsAndroidBridge";

    public static final String METHOD_ARG0 = "arg0";
    public static final String METHOD_ARG1 = "arg1";
    public static final String METHOD_ARG2 = "arg2";
    public static final String METHOD_ARG3 = "arg3";
    public static final String METHOD_ARG4 = "arg4";
    public static final String METHOD_ARG5 = "arg5";

    public static final String RESULT_CODE = "code";
    public static final String RESULT_ARG0 = "arg0";
    public static final String RESULT_ARG1 = "arg1";
    public static final String RESULT_ARG2 = "arg2";
    public static final String RESULT_ARG3 = "arg3";
    public static final String RESULT_ARG4 = "arg4";
    public static final String RESULT_ARG5 = "arg5";
    public static final String RESULT_ARG6 = "arg6";

    //Js Call Native
    public static final String JS_2_NATIVE_GET_NET_STATUS = "GET_NET_STATUS";
    public static final String JS_2_NATIVE_GET_TOKEN = "GET_TOKEN";
    public static final String JS_2_NATIVE_GET_CODE = "GET_CODE";
    //获取UUID
    public static final String JS_2_NATIVE_GET_UUID = "GET_UUID";
    //获取手机型号
    public static final String JS_2_NATIVE_GET_PHONEMODEL = "GET_PHONEMODEL";
    //获取产品渠道，android获取渠道，ios获取bundle id
    public static final String JS_2_NATIVE_GET_FLAVOR = "GET_FLAVOR";
    public static final String JS_2_NATIVE_PAY = "PAY";
    public static final String JS_2_NATIVE_GOH5 = "GOH5";
    public static final String JS_2_NATIVE_LOG = "LOG";

    //Native Call Js
    public static final String NATIVE_2_JS_GET_CODE = "CALLBACK_GET_CODE";
    public static final String NATIVE_2_JS_PAY_RESULT = "CALLBACK_PAY_RESULT";

    public static final String Net_None = "0";
    public static final String Net_Wifi = "1";
    public static final String Net_Mobile = "2";

    static OnMethod sOnMethod;

    public static void setOnMethod(OnMethod onMethod) {
        sOnMethod = onMethod;
    }

    /**
     * @param method js传过来的方法名
     * @param param  {
     *               arg0: 第一个参数
     *               arg1: 第二个参数
     *               arg2: 第三个参数
     *               arg3: 第四个参数
     *               arg4: 第五个参数
     *               arg5: 第六个参数   最多支持6个参数
     *               }
     * @return 返回一个json对象
     * <p>
     * json 数据结构    {
     * code: 0表示成功; > 0表示失败
     * - 1 json 解析出错
     * - 2 没有指定method
     * - 3 未知的method name
     * arg0 : 第一个返回值
     * arg1 : 第二个返回值
     * arg2 : 第三个返回值
     * arg3 : 第四个返回值
     * arg4 : 第五个返回值
     * arg5 : 第六个返回值   最多支持6个参数
     * }
     */
    public static String callNative(String method, String param) {
        if (sOnMethod != null)
            return sOnMethod.call(method, param);

        CallNativeArg ret = new CallNativeArg();
        return GsonUtils.getGson().toJson(ret);
    }

    /**
     * 为了兼容param中带有非法的UTF8字符
     *
     * @param method
     * @param param
     * @return
     */
    public static String callNative2(String method, byte[] param) {
        try {
            String nativeString = new String(param, "UTF-8"); // please debate what the safest charset should be?

            return callNative(method, nativeString);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            Log.e(TAG, "Couldn't convert the byte to String");
        }

        return null;
    }

    public static native void setNativeContext(Context context);

    public static native int callCppFromNative(String param);

    public static native String getStringFromCpp();

    public interface OnMethod {
        String call(String method, String param);
    }
}
