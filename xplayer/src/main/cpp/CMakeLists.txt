cmake_minimum_required(VERSION 3.4.1)

project("xplayer")

set(FFMPEG ${CMAKE_SOURCE_DIR}/ffmpeg) # ffmpeg的路径
include_directories(${FFMPEG}/include) # 导入ffmpeg的头文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${FFMPEG}/libs/${CMAKE_ANDROID_ARCH_ABI}") # ffmpeg库指定

add_definitions(-DDEBUG)

add_library(${CMAKE_PROJECT_NAME} SHARED
        com_pujh_xplayer_XPlayer.cpp
        XPlayer.cpp
        IDemux.cpp
        FFDemux.cpp
        IDecode.cpp
        FFDecode.cpp
        XData.cpp
        XParameter.cpp
        util/XThread.cpp
        util/ISubject.cpp
        utils/AAssetUtil.cpp
        utils/GLUtil.cpp
        IVideoView.cpp
        view/GLVideoView.cpp
        view/WindowVideoView.cpp
        XEGL.cpp
        XGLRender.cpp
        render/YUV420PRender.cpp
        render/RGB24Render.cpp
        render/NV12Render.cpp
        render/NV21Render.cpp
        IResample.cpp
        FFResample.cpp
        IAudioPlayer.cpp
        SLAudioPlayer.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        # 忽略顺序的方式，导入
        avcodec avfilter avformat avutil swresample swscale fdk-aac x264
        android
        log
        z
        GLESv3
        EGL
        OpenSLES
)
