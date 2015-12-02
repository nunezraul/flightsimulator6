
// Ensure the header file is included only once in multi-file projects
#ifndef GS_LIGHT_H
#define GS_LIGHT_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>

class GsLight
 { public :
    GsVec pos;
    GsColor amb, dif, spe; // ambient, diffuse, specular
    GsLight () {}
    GsLight ( const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s )
      : pos(p), amb(a), dif(d), spe(s) {}
    void set ( const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s )
      { pos=p; amb=a; dif=d; spe=s; }
 };

#endif // GS_LIGHT_H
