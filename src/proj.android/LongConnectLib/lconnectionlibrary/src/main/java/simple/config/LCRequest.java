package simple.config;

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

    public LCRequest(String method, LCResponse response) {
        this(method, response, true);
    }

    public LCRequest(String method, LCResponse response, boolean auto) {
        mMethod = method;
        mResponse = response;
        mAutoRemove = auto;

        LConnection.appendReq(this);
    }

    /**
     * 连接服务器
     *
     * @param ip
     * @param port
     * @param timeout
     * @param response
     * @return
     */
    public static LCRequest connectTo(String ip, short port, int timeout, LCResponse response) {
        if (LConnection.connectTo(ip, port, timeout))
            return listenConnect(response, true);
        return null;
    }

    /**
     * 监听服务器连接
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenConnect(LCResponse response, boolean auto) {
        return new LCRequest("connectLobby", response, auto);
    }

    /**
     * 断开连接
     *
     * @return
     */
    public static boolean disconnect() {
        return LConnection.disconnect();
    }

    /**
     * 登陆服务器
     *
     * @param uid
     * @param token
     * @param appkey
     * @param response
     * @return
     */
    public static LCRequest login(String uid, String token, String appkey, LCResponse response) {
        if (LConnection.login(uid, token, appkey))
            return new LCRequest("login", response);
        return null;
    }

    /**
     * 登出服务器
     *
     * @return
     */
    public static boolean logout() {
        return LConnection.logout();
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
    public static LCRequest sayTo(int type, String from, String to, String content, String ext, LCResponse response) {
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
    public static LCRequest listenSay(LCResponse response, boolean auto) {
        return new LCRequest("sayTo", response, auto);
    }

    /**
     * 监听通知消息
     *
     * @param response
     * @param auto
     * @return
     */
    public static LCRequest listenNotify(LCResponse response, boolean auto) {
        return new LCRequest("notify", response, auto);
    }

    /**
     * 进入房间
     *
     * @param room_id
     * @param response
     * @return
     */
    public static LCRequest enterRoom(String room_id, LCResponse response) {
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
    public static LCRequest listenEnterRoom(LCResponse response, boolean auto) {
        return new LCRequest("enterRoom", response, auto);
    }

    /**
     * 离开房间
     *
     * @param response
     * @return
     */
    public static LCRequest exitRoom(LCResponse response) {
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
    public static LCRequest listenExitRoom(LCResponse response, boolean auto) {
        return new LCRequest("exitRoom", response, auto);
    }
}
