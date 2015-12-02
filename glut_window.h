/*=================================================
 * CSE-170 Support Code
 * UC Merced, M. Kallmann 
 *=================================================*/

// Ensure the header file is included only once in multi-file projects
#ifndef GLUT_WINDOW_H
#define GLUT_WINDOW_H

// Include GLUT (OpenGL Graphics Utility Library) header files.
// In Windows we include our own freeglut header file, but
// in Linux, the header file should be installed in the system.
#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <freeglut.h>
#else
#include <GL/freeglut.h>
#endif

// This is a class wrapper for the C interface of glut
// It also provides some low-level OpenGL initialization so that
// the user-derived class can concentrate on the specific project implementation
// You may modify/extend this file as needed
class GlutWindow
 { private: // internal callback functions called by freeglut
    static void glutIdleCB ();
    static void glutMenuCB ( int m );
    static void glutMouseCB ( int b, int s, int x, int y );
    static void glutMotionCB ( int x, int y );
    static void glutDisplayCB ();
    static void glutKeyboardCB ( unsigned char key, int x, int y );
    static void glutSpecialCB ( int key, int x, int y );
    static void glutReshapeCB ( int w, int h );

   public:
    // constructor
    GlutWindow ( const char* label, int x, int y, int w, int h );

    // facilitate creation of submenus
    void addMenuEntry ( const char* label, int menuev ) { ::glutAddMenuEntry(label,menuev); }

    // asks glut to redraw the window as soon as possible
    void redraw() { glutPostRedisplay(); }

    // Note that glutMainLoop never returns so your program is entirely event driven
    void run () { glutMainLoop (); }

   private : // member functions to be derived by the user class
    virtual void glutIdle () {}
    virtual void glutMenu ( int m ) {}
    virtual void glutKeyboard ( unsigned char key, int x, int y ) {}
    virtual void glutSpecial ( int key, int x, int y ) {}
    virtual void glutMouse ( int b, int s, int x, int y ) {}
    virtual void glutMotion ( int x, int y ) {}
    virtual void glutReshape ( int w, int h ) {}
    virtual void glutDisplay () {}
 };

#endif // GLUT_WINDOW_H
