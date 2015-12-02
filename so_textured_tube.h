
#ifndef SO_TEXTURED_CAPSULE_H
#define SO_TEXTURED_CAPSULE_H

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "gsim/gs_image.h"
# include "gsim\gs_light.h"


class SoTexturedTube : public GlObjects
{
private:
	std::vector<GsVec> P; // coordinates
	std::vector<GsVec> N; // colors
	std::vector<GsVec2> T; // texture coords
	GsVec norm, v1, v2;
	GlProgram _prog;
	GlShader _vsh, _fsh;
	int _numpoints; // saves the number of points
	gsuint id, id2;

public:
	class Material {
	public:
		GsColor amb, dif, spe;
		float sh; //Shininess
		Material(const GsColor& a, const GsColor& d, const GsColor&s, float sn)
			: amb(a), dif(d), spe(s), sh(sn) {}
		Material() {}
	};

	SoTexturedTube();
	void init();
	void draw(GsMat& tr, GsMat& pr, const GsLight &l, float &col, GLuint *texture);
	void build(float len, float rt, float rb, float nfaces, GLuint * texture);


};

#endif //SO_CAPSULE_H#pragma once
