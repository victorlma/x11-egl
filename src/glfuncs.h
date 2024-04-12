#ifndef GL_FUNCS_
#define GL_FUNCS_

typedef void * (* LOADPROC)(char const *procname);

typedef float GLFloat;
typedef unsigned int GLbitfield;
typedef int GLint;
typedef int GLsizei;

#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

typedef void (* GL_CLEARCOLOR)(GLFloat, GLFloat, GLFloat, GLFloat);
GL_CLEARCOLOR glClearColor = 0x0;

typedef void (* GL_CLEAR) (GLbitfield);
GL_CLEAR glClear = 0x0;

typedef void (* GL_FLUSH) (void);
GL_FLUSH glFlush = 0x0;

typedef void (* GL_VIEWPORT) (GLint, GLint, GLsizei, GLsizei);
GL_VIEWPORT glViewport = 0x0;


void glfuncload(LOADPROC loadproc)
{
    glClearColor = (GL_CLEARCOLOR) loadproc("glClearColor");
    glClear = (GL_CLEAR) loadproc("glClear");
    glFlush = (GL_FLUSH) loadproc("glFlush");
    glViewport = (GL_VIEWPORT) loadproc("glViewport");

}

#endif
