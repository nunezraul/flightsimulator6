
// Ensure the header file is included only once in multi-file projects
#ifndef SO_AXIS_H
#define SO_AXIS_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoAxis : public GlObjects
 { private :
    GlShader _vsh, _fsh;
    GlProgram _prog;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsColor> C; // color
    int _numpoints;     // just saves the number of points

   public :
    SoAxis ();
    void init ();
    void build ( float r );
    void draw ( GsMat& tr, GsMat& pr );
 };

#endif // SO_AXIS_H
