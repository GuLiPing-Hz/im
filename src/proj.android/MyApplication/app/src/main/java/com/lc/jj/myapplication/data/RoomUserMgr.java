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
        mIMUserMap.put(user.uid, user);
    }

    public void removeIMUser(String uid) {
        mIMUserMap.remove(uid);
    }

    public void clearIMUser() {
        mIMUserMap.clear();
    }

    public void logIMUser() {
        //
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
    }
}
