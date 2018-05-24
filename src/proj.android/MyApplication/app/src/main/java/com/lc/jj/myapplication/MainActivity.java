package com.lc.jj.myapplication;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.lc.jj.myapplication.adapter.TalkAdapter;
import com.lc.jj.myapplication.bean.TalkBean;

import java.util.ArrayList;
import java.util.List;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import simple.config.LCRequest;
import simple.config.LCResponse;
import simple.config.LConnection;

public class MainActivity extends AppCompatActivity {

    public static final String Tag = "MainActivity";

    @BindView(R.id.et_uid)
    EditText mEtUid;
    @BindView(R.id.et_say)
    EditText mEtSay;
    @BindView(R.id.et_roomid)
    EditText mEtRoomid;
    @BindView(R.id.rv)
    RecyclerView mRecyclerView;

    List<TalkBean> mTalkBeanList = new ArrayList<>();
    LCRequest mTalkListener;
    public static LCRequest sRoomUserListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        mRecyclerView.setAdapter(new TalkAdapter(mTalkBeanList));
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));

        mTalkListener = LCRequest.listenSay(new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                TalkBean talkBean = new TalkBean();
                talkBean.type = bundle.getInt("type");
                talkBean.from = bundle.getString("from");
                talkBean.to = bundle.getString("to");
                talkBean.content = bundle.getString("content");
                talkBean.ext = bundle.getString("ext");

                mTalkBeanList.add(talkBean);
                mRecyclerView.getAdapter().notifyDataSetChanged();
                mRecyclerView.scrollToPosition(mTalkBeanList.size() - 1);
            }

            @Override
            public void onFailed(int i, String s) {
                //监听不会收到失败消息
            }
        }, false);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        /**
         * 这里也有可能被系统回收，根据你的需求
         */
        if (mTalkListener != null)
            LConnection.removeReq(mTalkListener);
    }

    @OnClick({R.id.btn_logout, R.id.btn_send, R.id.btn_enterroom})
    public void onViewClicked(View view) {
        switch (view.getId()) {
            case R.id.btn_logout:
                if (LCRequest.logout()) {
                    finish();
                } else {
                    Toast.makeText(this, "登出失败", Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.btn_send: {
                String sayId = mEtUid.getText().toString();
                String content = mEtSay.getText().toString();
                if (TextUtils.isEmpty(sayId) || TextUtils.isEmpty(content)) {
                    Toast.makeText(this, "请输入对话ID或者想说的内容", Toast.LENGTH_SHORT).show();
                    return;
                }

                LCRequest request = LCRequest.sayTo(LConnection.TYPE_P2P, LoginActivity.sMyUID, sayId
                        , content, "", new LCResponse() {
                            @Override
                            public void onSuccess(Bundle bundle) {
                                Toast.makeText(MainActivity.this, "发言成功", Toast.LENGTH_SHORT).show();
                            }

                            @Override
                            public void onFailed(int i, String s) {
                                Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                                Toast.makeText(MainActivity.this, "发送聊天失败 code =" + i, Toast.LENGTH_SHORT).show();
                            }
                        });

                if (request != null) {
                    Toast.makeText(this, "正在发送聊天...", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(this, "发送聊天请求失败...", Toast.LENGTH_SHORT).show();
                }
            }
            break;
            case R.id.btn_enterroom: {
                final String roomId = mEtRoomid.getText().toString();
                if (TextUtils.isEmpty(roomId)) {
                    Toast.makeText(this, "请输入房间ID", Toast.LENGTH_SHORT).show();
                    return;
                }

                //离开房间后，就算监听着，因为服务器不会给我们消息，所以等于没有监听
                //那我就简单的输出个用户进出房间的日志
                if (sRoomUserListener == null) {
                    //进入房间之前监听用户列表
                    sRoomUserListener = LCRequest.listenRoomUser(new LCResponse() {
                        @Override
                        public void onSuccess(Bundle data) {
                            Log.i(Tag, "listenRoomUser data = " + data.toString());
                        }

                        @Override
                        public void onFailed(int code, String jsonReq) {

                        }
                    }, false);
                }

                LCRequest request = LCRequest.enterRoom(roomId, new LCResponse() {
                    @Override
                    public void onSuccess(Bundle bundle) {
                        Toast.makeText(MainActivity.this, "进入房间成功", Toast.LENGTH_SHORT).show();

                        Intent intent = new Intent(MainActivity.this, RoomActivity.class);
                        intent.putExtra(RoomActivity.ROOMID, roomId);
                        MainActivity.this.startActivity(intent);
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
            break;
        }
    }
}
