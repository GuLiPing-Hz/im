package simple.bean;

/**
 * Created by JJ on 2017/4/10.
 */

public class CallNativeArg {
    public String method = null;//参数名称
    public int code = -1;//是否成功,默认没成功
    public String arg0 = null;//: 第一个参数
    public String arg1 = null;//: 第二个参数
    public String arg2 = null;//: 第三个参数
    public String arg3 = null;//: 第四个参数
    public String arg4 = null;//: 第五个参数
    public String arg5 = null;//: 第六个参数
    public String arg6 = null;//: 第7个参数

    public String request = null;//失败回调的时候的请求参数
}
