package simple.bean;

import java.io.Serializable;

public class IMUser implements Serializable {
    public String uid;
    public int type = -1;//-1未知，0普通用户，1机器人
    public String avatar = "";//头像
    public String name = "";//昵称
    public int roleId = 0;//vip等级
    public int level = 0;//等级

    public IMUser(String uid, int type, String avatar, String name, int roleId, int level) {
        this.uid = uid;
        this.type = type;
        this.avatar = avatar == null ? "" : avatar;
        this.name = name == null ? "" : name;
        this.roleId = roleId;
        this.level = level;
    }

    public IMUser(String uid) {
        this.uid = uid;
    }

    @Override
    public String toString() {
        return "IMUser{" +
                "uid='" + uid + '\'' +
                ", type=" + type +
                ", avatar='" + avatar + '\'' +
                ", name='" + name + '\'' +
                ", roleId=" + roleId +
                ", level=" + level +
                '}';
    }
}
