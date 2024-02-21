# 设置构建本地库所需的最小版本的cbuild。
cmake_minimum_required(VERSION 3.10.2)

project("openglc")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI})

# 设置ffmpeg依赖库及头文件所在目录，并存进指定变量
set(ffmpeg_libs_DIR ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI})
set(ffmpeg_headers_DIR ${PROJECT_SOURCE_DIR}/../../media/ffmpeg/${ANDROID_ABI}/include)
set(yuv_headers_DIR ${PROJECT_SOURCE_DIR}/../../localfile/include)

#对于find_package找不到的外部依赖库，可以用add_library添加
# SHARED表示添加的是动态库
# IMPORTED表示是引入已经存在的动态库
add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavformat.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavfilter.so)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavcodec.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavutil.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswscale.so)

add_library(yuv SHARED IMPORTED)
set_target_properties(yuv PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libyuv.so)

find_library(log-lib  # 设置path变量的名称。
        log
        #        android
        #        GLESv2#  指定NDK库的名称 你想让CMake来定位。
        )

include_directories(${ffmpeg_headers_DIR})
include_directories(${yuv_headers_DIR})
add_library(
        openglc
        SHARED
        jni/com_media_opengl_NativeMethod.cpp
        jni/FFmpegEncode.cpp
        jni/FFmpegEncodeStream.cpp
        jni/MediaCallBack.cpp
        jni/nativeFfmpeg.cpp
        jni/Util.cpp)

target_link_libraries(
        openglc
        avformat
        avfilter
        avcodec
        avutil
        swresample
        swscale
        yuv
        ${log-lib}
)
