package simple.config;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

import simple.bean.CallNativeArg;
import simple.bean.IMUser;
import simple.util.bridge.JsAndroidBridge;
import simple.util.gson.GsonUtils;

/**
 * Created by JJ on 2017/10/9.
 */

public class LConnection {

    public static final String Tag = "LConnection";
    public static Context sContext;
    public static Handler sHandler;
    static boolean sIsInit = false;
    static List<LCRequest> sLCRequests = new ArrayList<>();

    static ReentrantLock mLock = new ReentrantLock();
    /**
     * 0：点对点发消息，1：群内消息
     */
    public static final int TYPE_P2P = 0;
    public static final int TYPE_TEAM = 1;

    /**
     * SDK 内部Code
     *
     * @return
     */
    //请求超时
    public static final int RESULT_TIMEOUT = -9999;
    //解析服务器协议失败
    public static final int RESULT_ERROR_PARSE = -9998;
    //SDK内存溢出
    public static final int RESULT_OOM = -9997;
    //请求未送出，可能处于断网状态
    public static final int RESULT_REQ_NOT_SEND = -9996;

    public static int getVersionNumber() {
        return 1;
    }

    public static String getVersionString() {
        return "1.0.0";
    }

    public static void appendReq(LCRequest request) {
        mLock.lock();
        if (request.mResponse != null)//需要监听的请求才会放到监听列表中,否则没有意义
            sLCRequests.add(request);
        mLock.unlock();
    }

    public static void removeReq(LCRequest request) {
        mLock.lock();
        sLCRequests.remove(request);
        mLock.unlock();
    }

    public static boolean initLConnection(Context context, Handler handler) {
        try {
            sContext = context;
            sHandler = handler;
            if (sContext == null || sHandler == null)
                return false;

            System.loadLibrary("LConnection");

            JsAndroidBridge.setOnMethod(new JsAndroidBridge.OnMethod() {
                @Override
                public String call(String method, String param) {

                    final CallNativeArg result = GsonUtils.getGson().fromJson(param, CallNativeArg.class);
                    mLock.lock();
                    Iterator<LCRequest> it = sLCRequests.iterator();
                    while (it.hasNext()) {
                        final LCRequest request = it.next();

                        if (TextUtils.equals(request.mMethod, "connectLobby") && (TextUtils.equals(method, "onLobbyTunnelConnectSuccess")
                                || TextUtils.equals(method, "onLobbyTunnelConnectTimeout") || TextUtils.equals(method, "onLobbyTunnelConnectError")
                                || TextUtils.equals(method, "onLobbyTunnelClose") || TextUtils.equals(method, "onLobbyTunnelError")
                                || TextUtils.equals(method, "driveAway")
                        )) {
                            if (request.mAutoRemove)//检查是否需要移除当前监听
                                it.remove();

                            if (TextUtils.equals(method, "onLobbyTunnelConnectSuccess")) {
                                sHandler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        request.mResponse.onSuccess(null);
                                    }
                                });
                            } else if (TextUtils.equals(method, "onLobbyTunnelClose") || TextUtils.equals(method, "driveAway")) {
                                sHandler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        request.mResponse.onClose(result.code);
                                    }
                                });
                            } else {
                                sHandler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        request.mResponse.onFailed(result.code, "");
                                    }
                                });
                            }
                        } else if (TextUtils.equals(request.mMethod, "login") && TextUtils.equals(method, "onLobbyTunnelError")) {
                            if (request.mAutoRemove)//检查是否需要移除当前监听
                                it.remove();

                            sHandler.post(new Runnable() {
                                @Override
                                public void run() {
                                    request.mResponse.onFailed(result.code, result.request);
                                }
                            });
                        } else if (TextUtils.equals(request.mMethod, "roomUser") && (TextUtils.equals(method, "enterRoom")
                                || TextUtils.equals(method, "exitRoom") || TextUtils.equals(method, "roomUser"))) {
                            if (request.mAutoRemove)//检查是否需要移除当前监听
                                it.remove();

                            final Bundle data = new Bundle();
                            if (TextUtils.equals(method, "enterRoom")) {
                                data.putBoolean("is_enter", true);
                                data.putString("room_id", result.arg0);
                                int type = 0;
                                if (!TextUtils.isEmpty(result.arg2))//如果不为空
                                    type = Integer.parseInt(result.arg2);
                                IMUser user = new IMUser(result.arg1, type);
                                ArrayList<IMUser> list = new ArrayList<>();
                                list.add(user);
                                data.putSerializable("uids", list);
                            } else if (TextUtils.equals(method, "exitRoom")) {
                                data.putBoolean("is_enter", false);
                                IMUser user = new IMUser(result.arg1);
                                ArrayList<IMUser> list = new ArrayList<>();
                                list.add(user);
                                data.putSerializable("uids", list);
                            } else {
                                data.putBoolean("is_enter", true);

                                ArrayList<IMUser> list = new ArrayList<>();
                                try {
                                    JSONArray jsonArray = new JSONArray(result.arg1);
                                    for (int i = 0; i < jsonArray.length(); i++) {
                                        JSONObject jsonObject = (JSONObject) jsonArray.get(i);
                                        IMUser user = new IMUser(jsonObject.getString("arg1_0"), jsonObject.getInt("arg1_1"));
                                        list.add(user);
                                    }
                                } catch (JSONException e) {
                                    e.printStackTrace();
                                }

                                data.putSerializable("uids", list);
                            }

                            sHandler.post(new Runnable() {
                                @Override
                                public void run() {
                                    request.mResponse.onSuccess(data);
                                }
                            });
                        } else if (TextUtils.equals(request.mMethod, method)) {
                            if (request.mAutoRemove)//检查是否需要移除当前监听
                                it.remove();

                            if (result.code == 0) {
                                if (TextUtils.equals(method, "login")) {
                                    sHandler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            request.mResponse.onSuccess(null);
                                        }
                                    });
                                } else if (TextUtils.equals(method, "sayTo")) {
                                    final Bundle data = new Bundle();
                                    data.putInt("type", Integer.parseInt(result.arg0));
                                    data.putString("from", result.arg1);
                                    data.putString("to", result.arg2);
                                    data.putString("content", result.arg3);
                                    data.putString("ext", result.arg4);

                                    sHandler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            request.mResponse.onSuccess(data);
                                        }
                                    });
                                } else if (TextUtils.equals(method, "notify")) {
                                    final Bundle data = new Bundle();
                                    data.putString("from", result.arg0);
                                    data.putString("content", result.arg1);

                                    sHandler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            request.mResponse.onSuccess(data);
                                        }
                                    });
                                } else if (TextUtils.equals(method, "enterRoom")) {
                                    final Bundle data = new Bundle();
                                    data.putString("room_id", result.arg0);
                                    data.putString("uid", result.arg1);

                                    sHandler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            request.mResponse.onSuccess(data);
                                        }
                                    });
                                } else if (TextUtils.equals(method, "exitRoom")) {
                                    final Bundle data = new Bundle();
                                    data.putString("uid", result.arg0);

                                    sHandler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            request.mResponse.onSuccess(data);
                                        }
                                    });
                                }
                            } else {
                                sHandler.post(new Runnable() {
                                    @Override
                                    public void run() {
                                        request.mResponse.onFailed(result.code, result.request);
                                    }
                                });
                            }
                        }
                    }
                    mLock.unlock();

                    return null;
                }
            });

            //设置android context
            JsAndroidBridge.setNativeContext(context);
            sIsInit = startClient();//启动网络层
            return sIsInit;
        } catch (Exception e) {
            return false;
        }
    }

    private static boolean startClient() {
        CallNativeArg param = new CallNativeArg();
        param.method = "startClient";
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean stopClient(boolean finished) {
        if (!sIsInit)
            return true;

        CallNativeArg param = new CallNativeArg();
        param.method = "stopClient";
        param.arg0 = finished ? "1" : "0";
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean isConnected() {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "isConnected";
        return 1 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean connectTo(String ip, short port, int timeout) {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "connectLobby";
        param.arg0 = ip;
        param.arg1 = String.valueOf(port);
        param.arg2 = String.valueOf(timeout);

        String strParam = GsonUtils.getGson().toJson(param);
        Log.d(Tag, strParam);

        return 0 == JsAndroidBridge.callCppFromNative(strParam);
    }

    public static boolean disconnect() {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "disConnectLobby";
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean login(String uid, String token, String appkey) {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "login";
        param.arg0 = uid;
        param.arg1 = token;
        param.arg2 = appkey;
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean logout() {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "logout";
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean sayTo(int type, String from, String to, String content, String ext) {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "sayTo";
        param.arg0 = String.valueOf(type);
        param.arg1 = from;
        param.arg2 = to;
        param.arg3 = content;
        param.arg4 = ext;
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean enterRoom(String room_id) {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "enterRoom";
        param.arg0 = room_id;
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }

    public static boolean exitRoom() {
        if (!sIsInit)
            return false;

        CallNativeArg param = new CallNativeArg();
        param.method = "exitRoom";
        return 0 == JsAndroidBridge.callCppFromNative(GsonUtils.getGson().toJson(param));
    }
}
