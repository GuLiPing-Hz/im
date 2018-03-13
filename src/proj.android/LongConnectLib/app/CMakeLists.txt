# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             LConnection

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp

             ../../../Classes/zip/ioapi.c
             ../../../Classes/zip/unzip.c
             ../../../Classes/zip/zip.c

             ../../../Classes/zlib/adler32.c
             ../../../Classes/zlib/compress.c
             ../../../Classes/zlib/crc32.c
             ../../../Classes/zlib/deflate.c
             ../../../Classes/zlib/gzclose.c
             ../../../Classes/zlib/gzlib.c
             ../../../Classes/zlib/gzread.c
             ../../../Classes/zlib/gzwrite.c
             ../../../Classes/zlib/infback.c
             ../../../Classes/zlib/inffast.c
             ../../../Classes/zlib/inflate.c
             ../../../Classes/zlib/inftrees.c
             ../../../Classes/zlib/trees.c
             ../../../Classes/zlib/uncompr.c
             ../../../Classes/zlib/zutil.c

             ../../../Classes/crypto/md5.cpp

             ../../../Classes/wrap/ext/event.cpp
             ../../../Classes/wrap/ext/thread.cpp

             ../../../Classes/wrap/buffer_value.cpp
             ../../../Classes/wrap/client_socket.cpp
             ../../../Classes/wrap/data_decoder.cpp
             ../../../Classes/wrap/event_handler.cpp
             ../../../Classes/wrap/file_mgr.cpp
             ../../../Classes/wrap/funcs.cpp
             ../../../Classes/wrap/http_download.cpp
             ../../../Classes/wrap/http_download_mgr.cpp
             ../../../Classes/wrap/httpcontent.cpp
             ../../../Classes/wrap/native_buffer.cpp
             ../../../Classes/wrap/reactor.cpp
             ../../../Classes/wrap/serversocket.cpp
             ../../../Classes/wrap/thread_informer.cpp
             ../../../Classes/wrap/thread_mgr.cpp
             ../../../Classes/wrap/timer.cpp

             ../../../Classes/charset/CharsetCodec.cpp
             ../../../Classes/charset/Chinese.cpp
             ../../../Classes/charset/Utf.cpp

             ../../../Classes/bridge/SimpleBridge.cpp
             ../../../Classes/bridge/SimpleBridgeAndroid.cpp

             ../../../Classes/jni/JniHelper.cpp

             ../../../Classes/app/DataDecoderLobby.cpp
             ../../../Classes/app/DataDecoderRoom.cpp
             ../../../Classes/app/NetApp.cpp
             ../../../Classes/app/NetInformer.cpp
             ../../../Classes/app/RequestBase.cpp
             ../../../Classes/app/Tunnel.cpp

             ../../../Classes/LongConnection.cpp
             ../../../Classes/simple_util_bridge_JsAndroidBridge.cpp
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       LConnection

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )