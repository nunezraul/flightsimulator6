
// Ensure the header file is included only once in multi-file projects
#ifndef SO_CURVE_H
#define SO_CURVE_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"


GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts);
GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts);
float binomial_coff(float n, float k);
int factorial(int n);
GsVec crspline(float t, const GsArray<GsVec>& pnts);
GsVec bospline(float t, const GsArray<GsVec>& pnts);
GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts);
static float N(int i, int k, float u);




#endif // SO_POLY_H

