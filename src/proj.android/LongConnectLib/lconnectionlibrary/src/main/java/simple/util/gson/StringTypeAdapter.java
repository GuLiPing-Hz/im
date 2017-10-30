package simple.util.gson;

import com.google.gson.TypeAdapter;
import com.google.gson.stream.JsonReader;
import com.google.gson.stream.JsonToken;
import com.google.gson.stream.JsonWriter;

import java.io.IOException;

/**
 * 自定义TypeAdapter ,null对象将被解析成空字符串
 */
public class StringTypeAdapter extends TypeAdapter<String> {
    @Override
    public void write(JsonWriter writer, String value) throws IOException {
        try {
            if(value == null){
                writer.nullValue();
                return;
            }
            writer.value(value);
        }catch (Throwable ex){
            ex.printStackTrace();
        }
    }

    @Override
    public String read(JsonReader reader) throws IOException {
        try {
            if(reader.peek() == JsonToken.NULL){
                reader.nextNull();
                return "";//原先是返回Null，这里改为返回空字符串
            }
            return reader.nextString();
        }catch (Throwable ex){
            ex.printStackTrace();
        }
        return "";
    }
}
