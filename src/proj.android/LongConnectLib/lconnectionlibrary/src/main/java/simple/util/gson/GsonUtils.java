package simple.util.gson;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

/**
 */
public class GsonUtils {
    public static Gson getGson() {
        GsonBuilder gsonBuilder = new GsonBuilder();
        gsonBuilder.registerTypeAdapter(int.class, new IntegerTypeAdapter());
        gsonBuilder.registerTypeAdapter(Integer.class, new IntegerTypeAdapter());
        gsonBuilder.registerTypeAdapter(long.class, new LongTypeAdapter());
        gsonBuilder.registerTypeAdapter(Long.class, new LongTypeAdapter());
        gsonBuilder.registerTypeAdapter(String.class, new StringTypeAdapter());
        return gsonBuilder.create();
    }
}
