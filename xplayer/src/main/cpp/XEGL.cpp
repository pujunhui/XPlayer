//
// Created by 24415 on 2024-11-04.
//

#include "XEGL.h"

#include <EGL/egl.h>
#include <android/native_window.h>

#include "util/XLog.h"

class CXEGL : public XEGL {
public:
    bool Init(void *win) override {
        //获取原始窗口
        ANativeWindow *window = (ANativeWindow *) win;

        //初始化EGL
        //1 获取EGLDisplay对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            XLOGE("eglGetDisplay failed!");
            return false;
        }

        //2 初始化Display
        if (EGL_TRUE != eglInitialize(display, 0, 0)) {
            XLOGE("eglInitialize failed!");
            return false;
        }

        //3 获取配置
        //输出配置项
        EGLint numConfig;
        //输入配置项
        EGLint attribList[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                EGL_NONE
        };
        if (EGL_TRUE != eglChooseConfig(display, attribList, &config, 1, &numConfig)) {
            XLOGE("eglChooseConfig failed!");
            return false;
        }

        //4 创建surface
        surface = eglCreateWindowSurface(display, config, window, NULL);
        if (EGL_NO_SURFACE == surface) {
            XLOGE("eglCreateWindowSurface failed!");
            return false;
        }

        //5 创建并打开EGL上下文
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (EGL_NO_CONTEXT == context) {
            XLOGE("eglCreateContext failed!");
            return false;
        }

        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            XLOGE("eglMakeCurrent failed!");
            return false;
        }
        return true;
    }

    void makeCurrent() override {
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
        };
        EGLContext shareContext = eglCreateContext(display, config, context, ctxAttr);
        if (EGL_NO_CONTEXT == context) {
            XLOGE("eglCreateContext failed!");
            return;
        }

        // 切换到新的渲染上下文
        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, shareContext)) {
            XLOGE("eglMakeCurrent failed!");
            return;
        }
    }

    void swapBuffers() override {
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            return;
        }
        //窗口显示
        eglSwapBuffers(display, surface);
    }

    void clean() {
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
    }

private:
    EGLConfig config = NULL;
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLContext context = EGL_NO_CONTEXT;
    EGLSurface surface = EGL_NO_SURFACE;
};

XEGL *XEGL::Get() {
    static CXEGL egl;
    return &egl;
}
