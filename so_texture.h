
// Ensure the header file is included only once in multi-file projects
# pragma once

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <gsim/gs_image.h>
# include <gsim/gs_light.h>

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoTriangles : public GlObjects
 { private :
    GlShader _vsh, _fsh;
    GlProgram _prog;
    GsArray<GsVec>   P; // coordinates
    GsArray<GsVec>   N; // normals
	GsArray<GsVec2>  T; // textures
    int _numelements;
	gsuint id;
    // etc

	class Material {
	public:
		GsColor amb, dif, spe;
		float sh; //Shininess
		Material(const GsColor& a, const GsColor& d, const GsColor&s, float sn)
			: amb(a), dif(d), spe(s), sh(sn) {}
		Material() {}
	};

   public :
    SoTriangles();
    void init (const char *file, GLuint *textures);
    void build ( float parameter1, float parameter2 /*etc*/, float parameter3, GLuint *textures);
    void draw ( GsMat& tr, GsMat& pr, const GsLight& l, GLuint *textures);
 };

