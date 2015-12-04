//Justin Nguyen
# include <iostream>
//# include <gsim/gs.h>
# include "ogl_tools.h"
# include "app_window.h"

//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
 {
   // Init freeglut library:
   glutInit ( &argc, argv );
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitContextProfile ( GLUT_CORE_PROFILE );

   // Display some info:
   std::cout<<"Camera keys: Spacebar for view switch\n";
   std::cout<<"wasd for movements\n";
   std::cout<<"mnbv for front wings movement\n";
   std::cout << "hjkl for back wings movement\n";
   std::cout << "T for animation\n";

   std::cout<<"Notes:\n";
   //std::cout<<"- the smooth normal generation runs much faster in Release mode;\n";
   /*std::cout<<"- this application crashed in one of my computers but I could not reproduce the problem, "<<
              "let me know if you find a bug somewhere in the code.\n\n";*/
   // Now create the window of your application:
   AppWindow* w = new AppWindow ( "Flight Simulator VI", 300, 300, 640, 480 );

   // Finally start the main loop:
   w->run ();
}
