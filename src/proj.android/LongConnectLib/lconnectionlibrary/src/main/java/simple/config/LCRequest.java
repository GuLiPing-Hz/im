package simple.config;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

/**
 * Created by JJ on 2017/10/9.
 */

public class LCRequest {

    public String mMethod;
    public LCResponse mResponse;
    /**
     * 如果设置为false的时候那么需要你自己删除
     *
     * @see LConnection#removeReq(LCRequest)
     */
    public boolean mAutoRemove = true;//

    public static String sAppkey;
    public static String sUId;
    public static String sToken;

    public static String sIp;
    public static short sPort = 0;
    public static int sTimeout = 10;

    static int sMaxReLoginTimes = 3;
    static int sCurReloingTimes = 0;
    static boolean sIsLogin = false;

    public static boolean IsLogin() {
        return sIsLogin;
    }

    /**
     * 重连最大尝试次数
     *
     * @param times
     */
    public static void SetMaxReLoginTimes(int times) {
        sMaxReLoginTimes = times;
    }

    /**
     * 设置登录地址端口超时时间 appkey
     *
     * @param appkey
     * @param ip
     * @param port
     * @param timeout
     */
    public static void SetAppHostPort(@NonNull String appkey, @NonNull String ip, short port, int timeout) {
        sAppkey = appkey;
        sIp = ip;
        sPort = port;
        sTimeout = timeout;
    }

    public LCRequest(String method, @Nullable LCResponse response) {
        this(method, response, true);
    }

    public LCRequest(String method, @Nullable LCResponse response, boolean auto) {
        mMethod = method;
        mResponse = response;
        mAutoRemove = auto;

        LConnection.appendReq(this);
    }

    /**
     * 监听服务器连接
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenConnect(@NonNull final LCResponse response, boolean auto) {
        return new LCRequest("connectLobby", new LCResponse() {
            @Override
            public void onSuccess(Bundle data) {
                if (response != null)
                    response.onSuccess(data);
            }

            @Override
            public void onFailed(int code, String jsonReq) {
                sIsLogin = false;

                if (response != null)
                    response.onFailed(code, jsonReq);
            }

            @Override
            public void onClose(int code) {
                sIsLogin = false;

                if (response != null)
                    response.onClose(code);
            }
        }, auto);
    }

    /**
     * 登陆服务器
     *
     * @param uid
     * @param token
     * @param response
     * @return
     */
    public static LCRequest login(@NonNull String uid, @NonNull String token, @NonNull final LCResponse response) {
        if (sAppkey == null || sIp == null || sPort == 0)
            return null;

        sIsLogin = false;
        sUId = uid;
        sToken = token;

        //断开之前的连接
        LConnection.disconnect();
        if (LConnection.connectTo(sIp, sPort, sTimeout)) {
            return listenConnect(new LCResponse() {
                @Override
                public void onSuccess(Bundle data) {//连接成功
                    //连接成功我们开始登录
                    if (LConnection.login(sUId, sToken, sAppkey)) {
                        LCRequest ret = new LCRequest("login", new LCResponse() {
                            @Override
                            public void onSuccess(Bundle data) {
                                sIsLogin = true;

                                if (response != null)
                                    response.onSuccess(data);
                            }

                            @Override
                            public void onFailed(int code, String jsonReq) {
                                sIsLogin = false;

                                if (response != null)
                                    response.onFailed(code, jsonReq);
                            }
                        });//构造一个监听
                    } else {
                        if (response != null)
                            response.onFailed(LConnection.RESULT_REQ_NOT_SEND, null);
                    }
                }

                @Override
                public void onFailed(int code, String jsonReq) {
                    if (response != null) {
                        response.onFailed(code, jsonReq);
                    }
                }
            }, true);
        }

        return null;
    }

    /**
     * 重连服务器  重连失败了表示已经尝试了{@link this#sMaxReLoginTimes}次了
     *
     * @param response
     * @return
     */
    public static LCRequest relogin(@NonNull final LCResponse response) {
        sCurReloingTimes = 0;

        return reloginInner(response);
    }

    /**
     * 重连服务器内部方法
     *
     * @param response
     * @return
     */
    private static LCRequest reloginInner(@NonNull final LCResponse response) {
        LCRequest ret = login(sUId, sToken, new LCResponse() {
            @Override
            public void onSuccess(Bundle data) {
                if (response != null)
                    response.onSuccess(data);
            }

            @Override
            public void onFailed(int code, String jsonReq) {
                sCurReloingTimes++;//重连次数加1
                if (sCurReloingTimes < sMaxReLoginTimes) {
                    reloginInner(response);
                } else {
                    if (response != null) {
                        response.onFailed(code, jsonReq);
                    }
                }
            }
        });

        return ret;
    }

    /**
     * 登出服务器
     *
     * @return
     */
    public static boolean logout() {
        return LConnection.disconnect();

//        if (LConnection.logout()) {
//            sIsLogin = false;
//
//            LConnection.sHandler.postDelayed(new Runnable() {
//                @Override
//                public void run() {
//                    LConnection.disconnect();
//                }
//            }, 1000);
//            return true;
//        }
//        return false;
    }

    /**
     * 发言
     *
     * @param type
     * @param from
     * @param to
     * @param content
     * @param response
     * @param ext
     * @return
     */
    public static LCRequest sayTo(int type, @NonNull String from, @NonNull String to, @NonNull String content
            , @NonNull String ext, @NonNull LCResponse response) {
        if (LConnection.sayTo(type, from, to, content, ext))
            return listenSay(response, true);
        return null;
    }

    /**
     * 监听发言消息
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenSay(@NonNull LCResponse response, boolean auto) {
        return new LCRequest("sayTo", response, auto);
    }

    /**
     * 监听通知消息
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenNotify(@NonNull LCResponse response, boolean auto) {
        return new LCRequest("notify", response, auto);
    }

    /**
     * 进入房间
     *
     * @param room_id
     * @param response
     * @return
     */
    public static LCRequest enterRoom(@NonNull String room_id, @NonNull LCResponse response) {
        if (LConnection.enterRoom(room_id))
            return listenEnterRoom(response, true);
        return null;
    }

    /**
     * 监听进入房间的消息
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenEnterRoom(@NonNull LCResponse response, boolean auto) {
        return new LCRequest("enterRoom", response, auto);
    }

    /**
     * 离开房间
     *
     * @param response
     * @return
     */
    public static LCRequest exitRoom(@NonNull LCResponse response) {
        if (LConnection.exitRoom())
            return listenExitRoom(response, true);
        return null;
    }

    /**
     * 监听离开房间的消息
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenExitRoom(@NonNull LCResponse response, boolean auto) {
        return new LCRequest("exitRoom", response, auto);
    }
}
