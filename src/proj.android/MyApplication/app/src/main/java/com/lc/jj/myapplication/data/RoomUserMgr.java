package com.lc.jj.myapplication.data;

import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import simple.bean.IMUser;

public class RoomUserMgr {

    static final String Tag = "RoomUserMgr";

    static RoomUserMgr sIns;
    Map<String, IMUser> mIMUserMap;

    private RoomUserMgr() {
        mIMUserMap = new HashMap<>();
    }

    public static RoomUserMgr GetInstance() {
        if (sIns == null) {
            sIns = new RoomUserMgr();
        }
        return sIns;
    }

    public Map<String, IMUser> getIMUserMap() {
        return mIMUserMap;
    }

    public void setIMUserMap(Map<String, IMUser> IMUserMap) {
        mIMUserMap = IMUserMap;
    }

    public void putIMUser(IMUser user) {
        Log.i(Tag, "用户 【uid=" + user.uid + "(" + getIMUserType(user) + ")】 进入房间");
        mIMUserMap.put(user.uid, user);
    }

    public void removeIMUser(String uid) {
        Log.i(Tag, "用户 【uid=" + uid + "】 离开房间");
        mIMUserMap.remove(uid);
    }

    public void clearIMUser() {
        mIMUserMap.clear();
    }

    public void logIMUser() {
        //
        Log.i(Tag, "查看房间其他用户 begin");
        Set<Map.Entry<String, IMUser>> set = mIMUserMap.entrySet();
        Iterator<Map.Entry<String, IMUser>> iter = set.iterator();
        while (iter.hasNext()) {
            Map.Entry<String, IMUser> data = iter.next();
            String type = "未知";
            int itype = data.getValue().type;
            if (itype == 0) {
                type = "普通用户";
            } else if (itype == 1) {
                type = "机器人";
            }
            Log.i(Tag, "房间用户 【uid=" + data.getKey() + "(" + type + ")】");
        }
        Log.i(Tag, "查看房间其他用户 end");
    }

    public String getIMUserType(IMUser user) {
        String type = "未知";
        int itype = user.type;
        if (itype == 0) {
            type = "普通用户";
        } else if (itype == 1) {
            type = "机器人";
        }
        return type;
    }
}
