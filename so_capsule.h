
#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"


class SoCapsule : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	GlShader _vsh, _fsh;
	GlProgram _prog;
	int _numpoints;         // saves the number of points

public:
	SoCapsule();
	void init();
	void draw(GsMat& tr, GsMat& pr);
	void build(float len, float sunx, float suny, float sunz);

};

#endif //SO_CAPSULE_H