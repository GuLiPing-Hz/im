package com.lc.jj.myapplication;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import butterknife.OnTextChanged;
import simple.config.LCRequest;
import simple.config.LCResponse;
import simple.config.LConnection;

public class LoginActivity extends AppCompatActivity {
    /**
     * uid 1000001
     * token 018f0be0cb075ee761cf8d44be2b287e
     * <p>
     * uid 1000002
     * token 9a78580de4e75b65b9b3e6840aede0dd
     * <p>
     * appkey
     * 4ed38057df363e8355229ec53687c549
     * <p>
     * 长连接服务器
     * 192.168.1.11:27710
     */
    public static final String Tag = "LoginActivity";
    public static final int RequestCode1 = 1;

    public static final String APPKEY = "4ed38057df363e8355229ec53687c549";
    //    public static final String HOST = "192.168.1.67";//
//    public static final String HOST = "192.168.0.18";//
    //正式服
    public static final String HOST = "34.199.120.59";
    //    public static final String HOST = "123.206.229.213";
    public static final short PORT = 27710;

    public static final String UID1 = "1100001";
    public static final String TOKEN1 = "018f0be0cb075ee761cf8d44be2b287e";
    public static final String UID2 = "1100002";
    public static final String TOKEN2 = "9a78580de4e75b65b9b3e6840aede0dd";
    public static final String UID11 = "1100011";
    public static final String TOKEN11 = "06c16a5cf83d77f8f531c3e0d65e31a9";
    public static final String UID12 = "1100012";
    public static final String TOKEN12 = "30f9f8feceee317a66df684915be86f9";

    //正式服
    public static final String UID4218 = "4218458";
    public static final String TOKEN4218 = "fc8a24d0c45c698dcffafeb57fcdbdee";
    public static final String UID4219 = "4217955";
    public static final String TOKEN4219 = "c0e7ae7d85fca3ae30c5053d1b0b5954";

    @BindView(R.id.et_uid)
    EditText mEtUid;
    @BindView(R.id.et_token)
    EditText mEtToken;
    @BindView(R.id.tv_appkey)
    TextView mTvAppkey;

    public static Handler sHandler = new Handler();
    public static String sMyUID = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        ButterKnife.bind(this);

        mTvAppkey.setText("当前测试APP KEY:" + APPKEY);
        mEtUid.setText(UID1);

        /**
         * 建议放到Application的onCreate中
         */
        LConnection.initLConnection(this.getApplicationContext(), sHandler);

        /**
         * 新增IM模块参数配置
         */
        LCRequest.SetMaxReLoginTimes(3);//设置重连最大次数
        LCRequest.SetAppHostPort(APPKEY, HOST, PORT, 10);//设置APP和服务器信息
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        //记得回收资源
        LConnection.stopClient(true);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == RequestCode1) {
            //都认为是退出登录的,移除链接监听
            LConnection.removeReq(mConnectReq);
        } else
            super.onActivityResult(requestCode, resultCode, data);
    }

    @OnClick(R.id.btn_login)
    public void onViewClicked() {
        final String uid = mEtUid.getText().toString();
        final String token = mEtToken.getText().toString();
        if (TextUtils.isEmpty(uid) || TextUtils.isEmpty(token)) {
            Toast.makeText(this, "UID或TOKEN为空", Toast.LENGTH_SHORT).show();
            return;
        }

        /**
         * 如果这里报错,请添加build.gradle中添加 compile 'com.google.code.gson:gson:2.7'
         */
        LCRequest req = LCRequest.login(uid, token, new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                /**
                 * 成功后,开启我们的监听服务器的连接
                 */
                mConnectReq = LCRequest.listenConnect(mConnectResp, false);

                Toast.makeText(LoginActivity.this, "登陆成功", Toast.LENGTH_SHORT).show();

                sMyUID = uid;
                //前往主界面
                Intent intent = new Intent(LoginActivity.this, MainActivity.class);
                LoginActivity.this.startActivityForResult(intent, RequestCode1);
            }

            @Override
            public void onFailed(int i, String s) {
                Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                Toast.makeText(LoginActivity.this, "登陆失败code =" + i, Toast.LENGTH_SHORT).show();
            }
        });

        if (req != null) {
            Toast.makeText(LoginActivity.this, "正在登录...", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(LoginActivity.this, "无法请求登录", Toast.LENGTH_SHORT).show();
        }
    }

    LCRequest mConnectReq;
    /**
     * 服务器连接监听器
     */
    LCResponse mConnectResp = new LCResponse() {
        @Override
        public void onSuccess(Bundle bundle) {

        }

        @Override
        public void onFailed(int i, String s) {
            /**连接断开了，我们必须移除监听*/
            LConnection.removeReq(mConnectReq);

            Toast.makeText(LoginActivity.this, "服务器异常 code=" + i + "开始重连。。。", Toast.LENGTH_SHORT).show();

            //重连
            LCRequest.relogin(new LCResponse() {
                @Override
                public void onSuccess(Bundle bundle) {
                    Toast.makeText(LoginActivity.this, "重连成功", Toast.LENGTH_SHORT).show();

                    /**重连成功才能继续监听*/
                    LConnection.appendReq(mConnectReq);
                }

                @Override
                public void onFailed(int i, String s) {
                    Toast.makeText(LoginActivity.this, "重连超过最大次数，返回登录界面", Toast.LENGTH_SHORT).show();

                    /**
                     * LoginActivity launchMode 是 singleTask
                     */
                    Intent intent = new Intent(LoginActivity.this.getApplicationContext(), LoginActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    LoginActivity.this.getApplicationContext().startActivity(intent);
                }
            });
        }

        @Override
        public void onClose(int code) {
            super.onClose(code);

            /**连接断开了，我们必须移除监听*/
            LConnection.removeReq(mConnectReq);

            if (code == 0) {//服务器直接close
                Toast.makeText(LoginActivity.this, "服务器主动断开,请退到登陆界面重新登录", Toast.LENGTH_SHORT).show();
            } else if (code == 9) {//收到服务器的关闭code 参见服务器文档code原因
                Toast.makeText(LoginActivity.this, "有其他设备登录您的账号", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(LoginActivity.this, "服务器主动断开,请退到登陆界面重新登录 code=" + code, Toast.LENGTH_SHORT).show();
            }

            /**
             * LoginActivity launchMode 是 singleTask
             */
            Intent intent = new Intent(LoginActivity.this.getApplicationContext(), LoginActivity.class);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            LoginActivity.this.getApplicationContext().startActivity(intent);
        }
    };

    @OnTextChanged(R.id.et_uid)
    void onTextChanged(CharSequence text) {
        if (TextUtils.equals(text, UID1)) {
            mEtToken.setText(TOKEN1);
        } else if (TextUtils.equals(text, UID2)) {
            mEtToken.setText(TOKEN2);
        } else if (TextUtils.equals(text, UID11)) {
            mEtToken.setText(TOKEN11);
        } else if (TextUtils.equals(text, UID12)) {
            mEtToken.setText(TOKEN12);
        } else if (TextUtils.equals(text, UID4218)) {
            mEtToken.setText(TOKEN4218);
        } else if (TextUtils.equals(text, UID4219)) {
            mEtToken.setText(TOKEN4219);
        }
    }
}
