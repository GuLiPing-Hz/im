package com.lc.jj.longconnectlib;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import simple.config.LCRequest;
import simple.config.LCResponse;
import simple.config.LConnection;

/**
 * 一：
 * 程序创建的so库位置查找：
 * <p>
 * 首先切换成Project视图
 * 找到app->build->intermediates->cmake
 * <p>
 * 自己选择需要的so库
 * <p>
 * 二：
 * 其他android程序应用，首先把aar放到app->libs
 * 然后app\src\main 下面新建jniLibs 把上面的自己需要的架构库放入其中即可
 */

public class MainActivity extends AppCompatActivity {

    public static final String Tag = "MainActivity";

    public static final String APPKEY = "4ed38057df363e8355229ec53687c549";
    public static final String HOST = "192.168.0.18";//"123.206.229.213";//
    public static final short PORT = 27710;

    public static final String UID1 = "1100001";
    public static final String TOKEN1 = "018f0be0cb075ee761cf8d44be2b287e";
    public static final String UID2 = "1100002";
    public static final String TOKEN2 = "9a78580de4e75b65b9b3e6840aede0dd";
    public static final String UID11 = "1100011";
    public static final String TOKEN11 = "06c16a5cf83d77f8f531c3e0d65e31a9";
    public static final String UID12 = "1100012";
    public static final String TOKEN12 = "30f9f8feceee317a66df684915be86f9";
    public static final String UID0 = "5757009";
    public static final String TOKEN0 = "76f0622bc02c8272300cbe13c9add794";
    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("LConnection");
//    }
    public static Handler sHandler = new Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        LConnection.initLConnection(this, sHandler);
        LCRequest.SetMaxReLoginTimes(3);
        LCRequest.SetAppHostPort(APPKEY, HOST, PORT, 2);

        LCRequest req = LCRequest.login(UID2, TOKEN2, new LCResponse() {
            @Override
            public void onSuccess(Bundle data) {
                /**
                 * 成功登录后,开启我们的监听服务器的连接
                 */
                LCRequest.listenConnect(mConnectResp, false);

                Toast.makeText(MainActivity.this, "登陆成功", Toast.LENGTH_SHORT).show();

//              boolean ret = LCRequest.logout();
                enterRoom();
//              sayTo();
            }

            @Override
            public void onFailed(int code, String jsonReq) {
                Log.e(Tag, "失败,请求内容是:(code=" + code + ")" + jsonReq);
                Toast.makeText(MainActivity.this, "登陆失败code =" + code, Toast.LENGTH_SHORT).show();
            }
        });

        if (req != null) {
            Toast.makeText(this, "正在登录", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "网络库启动失败,无法连接服务器", Toast.LENGTH_SHORT).show();
        }

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    void sayTo(int type, String to, String content) {
        LCRequest request = LCRequest.sayTo(type, UID0, to, content, "", new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                //Toast.makeText(MainActivity.this, "发言成功", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailed(int i, String s) {
                Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                //Toast.makeText(MainActivity.this, "发言失败 code =" + i, Toast.LENGTH_SHORT).show();
            }
        });

        if (request != null) {
            //Toast.makeText(this, "正在发言...", Toast.LENGTH_SHORT).show();
        } else {
            //Toast.makeText(this, "发言请求失败...", Toast.LENGTH_SHORT).show();
        }
    }

    void enterRoom() {
        //监听房间用户列表
        LCRequest.listenRoomUser(new LCResponse() {
            @Override
            public void onSuccess(Bundle data) {
                Log.i(Tag, "listenRoomUser data = " + data.toString());
            }

            @Override
            public void onFailed(int code, String jsonReq) {

            }
        }, false);
        LCRequest request = LCRequest.enterRoom("1000", new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                Toast.makeText(MainActivity.this, "进入房间成功", Toast.LENGTH_SHORT).show();

//                exitRoom();
                //sayTo(LConnection.TYPE_TEAM,UID1,"<gpmsg from=\"3001041\" dateline=\"2017-10-27 11:25:13\" id=\"30010411509074713903\" type=\"1\" to=\"3002167\"><send avatar=\"http://eaglelive-10077467.image.myqcloud.com/31cc83ff-2ef6-43fe-9b96-5d5df6ce66dc?imageView2/1/w/260/h/260 \" uid=\"3001041\" nickname=\"Erennnnnnnnnnnnn\" level=\"100\" grade=\"0\" role_id=\"1\" icon=\"0\"/><gift image=\"\" id=\"\" mode=\"\" number=\"\" name=\"\"/><praise num=\"\"/>" + "<action avatar=\"\" gender=\"\" uid=\"\" nickname=\"\" age=\"\" total_num=\"\" role_id=\"\" type=\"\"/><msg file_location=\"\" file=\"\" body=\"是\"/></gpmsg>");

//                new Thread() {
//                    @Override
//                    public void run() {
//                        while (true) {
////                            sHandler.post(new Runnable() {
////                                @Override
////                                public void run() {
////                                    sayTo(LConnection.TYPE_TEAM, UID1,"123456");
////                                }
////                            });
//
//                            sayTo(LConnection.TYPE_TEAM, UID1, "123456");
////                            LCRequest.sayTo(LConnection.TYPE_TEAM, "1000001", UID1, "123456", "", null);
//
//                            try {
//                                Thread.sleep(10);
//                            } catch (InterruptedException e) {
//                                e.printStackTrace();
//                            }
//                        }
//
//                    }
//                }.start();
            }

            @Override
            public void onFailed(int i, String s) {
                Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                Toast.makeText(MainActivity.this, "进入房间失败 code =" + i, Toast.LENGTH_SHORT).show();
            }
        });

        if (request != null) {
            Toast.makeText(this, "正在进入房间...", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "进入房间请求失败...", Toast.LENGTH_SHORT).show();
        }
    }

    void exitRoom() {
        LCRequest request = LCRequest.exitRoom(new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                Toast.makeText(MainActivity.this, "离开房间成功", Toast.LENGTH_SHORT).show();

                //销毁的时候关闭监听
//                finish();
            }

            @Override
            public void onFailed(int i, String s) {
                Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                Toast.makeText(MainActivity.this, "离开房间失败 code =" + i, Toast.LENGTH_SHORT).show();
            }
        });

        if (request != null) {
            Toast.makeText(this, "离开房间...", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "离开房间请求失败...", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 服务器连接监听器
     */
    LCResponse mConnectResp = new LCResponse() {
        @Override
        public void onSuccess(Bundle bundle) {

        }

        @Override
        public void onFailed(int i, String s) {
            Toast.makeText(MainActivity.this, "服务器异常code=" + i, Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onClose(int code) {
            super.onClose(code);

            Toast.makeText(MainActivity.this, "服务器主动断开,请退到登陆界面重新登录", Toast.LENGTH_SHORT).show();
        }
    };
}

