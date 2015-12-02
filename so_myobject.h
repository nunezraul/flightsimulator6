
// Ensure the header file is included only once in multi-file projects
# pragma once

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoMyObject : public GlObjects
 { private :
    GlShader _vsh, _fsh;
    GlProgram _prog;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsVec>   N; // normals
    int _numelements;
    // etc

   public :
    SoMyObject ();
    void init ( const GlProgram& prog );
    void build ( float parameter1, float parameter2 /*etc*/ );
    void draw ( GsMat& tr, GsMat& pr );
 };

