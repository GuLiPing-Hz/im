package com.lc.jj.myapplication;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
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

public class RoomActivity extends AppCompatActivity {

    public static final String Tag = "RoomActivity";

    public static final String ROOMID = "com.lc.jj.myapplication.RoomActivity.ROOMID";

    @BindView(R.id.et_say)
    EditText mEtSay;
    @BindView(R.id.text_roomid)
    TextView mTextRoomid;
    @BindView(R.id.rv)
    RecyclerView mRv;

    List<TalkBean> mTalkBeanList = new ArrayList<>();
    LCRequest mRoomTalk;
    String mRoomID;
    boolean mThreadRun;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_room);
        ButterKnife.bind(this);

        mThreadRun = true;
        mRoomID = getIntent().getStringExtra(ROOMID);

        mTextRoomid.setText("房间ID:" + mRoomID);
        mRv.setAdapter(new TalkAdapter(mTalkBeanList));
        mRv.setLayoutManager(new LinearLayoutManager(this));

        mRoomTalk = LCRequest.listenSay(new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                TalkBean talkBean = new TalkBean();
                talkBean.type = bundle.getInt("type");
                talkBean.from = bundle.getString("from");
                talkBean.to = bundle.getString("to");
                talkBean.content = bundle.getString("content");
                talkBean.ext = bundle.getString("ext");

                mTalkBeanList.add(talkBean);
                mRv.getAdapter().notifyDataSetChanged();
                mRv.scrollToPosition(mTalkBeanList.size() - 1);
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

        if (mRoomTalk != null) {//关闭监听
            LConnection.removeReq(mRoomTalk);
        }

        mThreadRun = false;
    }

    void sayTo(int type, String to, String content, String ext) {
        LCRequest request = LCRequest.sayTo(type, LoginActivity.sMyUID, to, content, ext, new LCResponse() {
            @Override
            public void onSuccess(Bundle bundle) {
                Toast.makeText(RoomActivity.this, "发言成功", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailed(int i, String s) {
                Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                Toast.makeText(RoomActivity.this, "发言失败 code =" + i, Toast.LENGTH_SHORT).show();
            }
        });

        if (request != null) {
            Toast.makeText(this, "正在发言...", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "发言请求失败...", Toast.LENGTH_SHORT).show();
        }
    }

    int mSayInt = 0;

    @OnClick({R.id.btn_send, R.id.btn_exitroom,R.id.btnFor})
    public void onViewClicked(View view) {
        switch (view.getId()) {
            case R.id.btnFor:{
                new Thread() {
                    @Override
                    public void run() {
                        while (mThreadRun) {
                            LCRequest.sayTo(LConnection.TYPE_TEAM, LoginActivity.sMyUID, mRoomID, String.valueOf(mSayInt++), "", null);
                            if (mSayInt == 100000) {
                                mSayInt = 0;
                            }
                            try {
                                Thread.sleep(1);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }

                    }
                }.start();
            }
            break;
            case R.id.btn_send: {
                String say = mEtSay.getText().toString();
                if (TextUtils.isEmpty(say)) {
                    Toast.makeText(this, "请输入想说的内容", Toast.LENGTH_SHORT).show();
                    return;
                }

                sayTo(LConnection.TYPE_TEAM, mRoomID, say, "");
            }
            break;
            case R.id.btn_exitroom: {
                LCRequest request = LCRequest.exitRoom(new LCResponse() {
                    @Override
                    public void onSuccess(Bundle bundle) {
                        Toast.makeText(RoomActivity.this, "离开房间成功", Toast.LENGTH_SHORT).show();

                        //销毁的时候关闭监听
                        finish();
                    }

                    @Override
                    public void onFailed(int i, String s) {
                        Log.e(Tag, "失败,请求内容是:(code=" + i + ")" + s);
                        Toast.makeText(RoomActivity.this, "离开房间失败 code =" + i, Toast.LENGTH_SHORT).show();
                    }
                });

                if (request != null) {
                    Toast.makeText(this, "离开房间...", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(this, "离开房间请求失败...", Toast.LENGTH_SHORT).show();
                }
            }
            break;
        }
    }
}
