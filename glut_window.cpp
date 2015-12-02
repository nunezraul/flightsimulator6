/*=================================================
 * glut_window.cpp
 * glut wrapper to GlutWindow class
 * UC Merced, M. Kallmann 
 *=================================================*/

#include <stdlib.h>
#include <stdio.h>
#include "ogl_tools.h"
#include "glut_window.h"


//===== static members =====

static GlutWindow* Singleton=0;      // we make it statice so that this pointer is hidden from other source files

//===== GlutWindow =====

GlutWindow::GlutWindow ( const char* label, int x, int y, int w, int h )
 {
   // Check if this is the only instance
   if ( Singleton ) std::cout<<"WARNING: The support code has not been tested with multiple GLUT Windows!\n";

   // First store this instance in our singleton pointer
   Singleton = this;

   // Set window position (from top corner), and size (width and height)
   glutInitWindowPosition ( x, y );
   glutInitWindowSize ( w, h );
   glutCreateWindow ( label );

   // Init glew library (after a glut window is created!):
   GLenum res = glewInit();
   if ( res!=GLEW_OK ) std::cout<<glewGetString(GLEW_VERSION)<<", Error: "<<glewGetErrorString(res)<<"\n";
   glPrintInfo();

   // Initialize OpenGL settings as we want
   glEnable ( GL_DEPTH_TEST );
   glEnable ( GL_POINT_SMOOTH );
   glEnable ( GL_LINE_SMOOTH );
   glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
   glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
   glPointSize ( 4 );
   glLineWidth ( 2 );

   // Set up GLUT callback functions to receive events:
   ::glutKeyboardFunc ( glutKeyboardCB );
   ::glutSpecialFunc ( glutSpecialCB );
   ::glutMouseFunc ( glutMouseCB );
   ::glutMotionFunc ( glutMotionCB );

   // Set up idle callback for background processing if needed:
   ::glutIdleFunc ( glutIdleCB );

   // Set up GLUT callback function for resizing window:
   ::glutReshapeFunc ( glutReshapeCB );

   // Set up GLUT callback for drawing the scene:
   ::glutDisplayFunc ( glutDisplayCB );

   // GLUT also supports a simple menu system, you may try this:
   int id = ::glutCreateMenu ( glutMenuCB ); // the returned id could be used for adding submenus if needed
   glutAttachMenu ( GLUT_RIGHT_BUTTON );
 }

//===== freeglut callbacks =====

void GlutWindow::glutKeyboardCB ( unsigned char key, int x, int y )
 {
   Singleton->glutKeyboard ( key, x, y );
 }

void GlutWindow::glutSpecialCB ( int key, int x, int y )
 {
   Singleton->glutSpecial ( key, x, y );
 }

void GlutWindow::glutMouseCB ( int b, int s, int x, int y )
 {
   Singleton->glutMouse ( b, s, x, y );
 }

void GlutWindow::glutMotionCB ( int x, int y )
 {
   Singleton->glutMotion ( x, y );
 }

void GlutWindow::glutMenuCB ( int m )
 {
   Singleton->glutMenu ( m );
 }

void GlutWindow::glutIdleCB ()
 {
   Singleton->glutIdle ();
 }

void GlutWindow::glutReshapeCB ( int w, int h )
 {
   Singleton->glutReshape ( w, h );
 }

void GlutWindow::glutDisplayCB ()
 {
   Singleton->glutDisplay ();
 }

