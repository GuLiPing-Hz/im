package simple.bean;

import java.io.Serializable;

public class IMUser implements Serializable {
    public String uid;
    public int type = -1;//-1未知，0普通用户，1机器人

    public IMUser(String uid, int type) {
        this.uid = uid;
        this.type = type;
    }

    public IMUser(String uid) {
        this.uid = uid;
    }
}
