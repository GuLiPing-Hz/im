package simple.config;

import android.os.Bundle;

/**
 * Created by JJ on 2017/10/9.
 */

public abstract class LCResponse {

    /**
     *  主线程回调,尽管放心更新UI界面
     */

    /**
     * 请求成功
     *
     * @param data 没有数据的时候为null
     */
    public abstract void onSuccess(Bundle data);

    /**
     * 请求失败
     *
     * @param code    错误码
     * @param jsonReq 请求json
     */
    public abstract void onFailed(int code, String jsonReq);

    /**
     * 服务器主动断开连接
     */
    public void onClose(int code) {
    }

}
