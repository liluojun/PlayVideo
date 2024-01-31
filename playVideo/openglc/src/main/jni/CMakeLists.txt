# 设置构建本地库所需的最小版本的cbuild。
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

aux_source_directory(. SRC_LIST)
# 设置ffmpeg依赖库及头文件所在目录，并存进指定变量
set(ffmpeg_libs_DIR ${CMAKE_SOURCE_DIR}/../../../media/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/lib)
set(ffmpeg_headers_DIR ${CMAKE_SOURCE_DIR}/../../../media/ffmpeg/${CMAKE_ANDROID_ARCH_ABI}/include)


#对于find_package找不到的外部依赖库，可以用add_library添加
# SHARED表示添加的是动态库
# IMPORTED表示是引入已经存在的动态库
add_library(avcodec SHARED IMPORTED)
#指定所添加依赖库的导入路径
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavcodec.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavfilter.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libswscale.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${ffmpeg_libs_DIR}/libavutil.so)

include_directories(${ffmpeg_headers_DIR})
link_directories(${ffmpeg_libs_DIR})
add_library(render-lib SHARED ${SRC_LIST})
#add_executable(render-lib  #设置库的名称。即SO文件的名称，生产的so文件为“libhello-lib.so”,在加载的时候“System.loadLibrary("hello-lib");”
#        ${SRC_LIST}
#        #[[SHARED  # 将库设置为共享库。 # 提供一个源文件的相对路径
#        nativeFfmpeg.cpp
#        FFmpegEncode.cpp
#        Util.cpp
#        MediaCallBack.cpp
#        FFmpegEncodeStream.cpp]])
find_library(log-lib  # 设置path变量的名称。
        log
        android
        GLESv2#  指定NDK库的名称 你想让CMake来定位。
        )
target_link_libraries(render-lib
        ${log-lib}
        avformat
        avcodec
        avfilter
        avutil
        swresample
        swscale)