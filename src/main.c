#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/keysym.h"
#include "EGL/egl.h"


#include "utils.h"
#include "glfuncs.h"


typedef struct
{
    Display *dpy;
    Window  root;
    Window  w;
    XEvent ev;
    GC  gc;

    KeySym key;

    b32 close;

} xstate;

typedef struct
{
    EGLNativeDisplayType  ndpy;
    EGLDisplay  dpy;
    EGLint  major;
    EGLint  minor;

    EGLConfig   ctxconf;
    EGLContext  ctx;
    EGLSurface surf;

    EGLint const *attribs;
    EGLint const *ctxattribs;

} egldata;

void resizevp(xstate *WinState)
{
    unsigned int w;
    unsigned int h;
    unsigned int d;
    int dd;
    Window root;
    XGetGeometry(WinState->dpy, WinState->w, &root, &dd, &dd, &w, &h, &d, &d);

    glViewport(0,0,w,h);
}
int main()
{
    xstate WinState = {
        .dpy = XOpenDisplay(NULL),
        .root = DefaultRootWindow(WinState.dpy),
        .w = XCreateSimpleWindow(WinState.dpy, WinState.root, 0, 0, 100, 200, 0, 0, 0),
    };

    XSelectInput(WinState.dpy, WinState.w, ExposureMask | KeyPressMask | KeyReleaseMask  | StructureNotifyMask);
    Atom wm_delete = XInternAtom(WinState.dpy, "WM_DELETE_WINDOW", 1);


    XMapWindow(WinState.dpy, WinState.w);


// EGL CONTEXT CREATION

    egldata gl = {0};
    gl.ndpy = WinState.dpy;
    gl.dpy = eglGetDisplay(gl.ndpy);
    gl.attribs = (EGLint const[]){
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_CONFORMANT, EGL_OPENGL_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_SURFACE_TYPE, EGL_PIXMAP_BIT | EGL_WINDOW_BIT,
            EGL_NONE
    };
    gl.ctxattribs = (EGLint const[]){
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 3,
        EGL_NONE,
    };



    eglInitialize(gl.dpy, &gl.major, &gl.minor);
    eglBindAPI(EGL_OPENGL_API);

    eglChooseConfig(gl.dpy, gl.attribs, &gl.ctxconf, 1, (EGLint[]){1});


    gl.ctx = eglCreateContext(gl.dpy, gl.ctxconf, EGL_NO_CONTEXT, gl.ctxattribs);

    gl.surf = eglCreateWindowSurface(gl.dpy, gl.ctxconf, (EGLNativeWindowType) WinState.w, NULL);

    eglMakeCurrent(gl.dpy, gl.surf, gl.surf, gl.ctx);

    glfuncload((LOADPROC) eglGetProcAddress);

        
    
    while (!WinState.close)
    {
        XNextEvent(WinState.dpy, &WinState.ev);

        switch (WinState.ev.type) {
            case KeyPress:
                XLookupString(&WinState.ev.xkey, (char[]){'a'}, 1, &WinState.key, NULL);

                if (WinState.key == XK_Escape) WinState.close = TRUE;

                

                break;


            case KeyRelease:
                break;
            
            case Expose:

                resizevp(&WinState);
            break;
        }

        

        glClearColor(0.0f,1.0f,0.7f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        eglSwapBuffers(gl.dpy, gl.surf);
    }

    eglDestroySurface(gl.dpy, gl.surf);
    eglDestroyContext(gl.dpy, gl.ctx);
    
    eglTerminate(gl.dpy);
    XSetWMProtocols(WinState.dpy, WinState.w, &wm_delete, 1);
    XDestroyWindow(WinState.dpy, WinState.w);
    XCloseDisplay(WinState.dpy);
    
        

    return 0;
}
