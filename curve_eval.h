// Ensure the header file is included only once in multi-file projects
#ifndef SO_CURVEVAL_H
#define SO_CURVEVAL_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <math.h>
# include <vector>


int factorial(int n);
float binomial_coff(float n, float k);
GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts);
GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts);
double lagrange(int n, double t);
GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts);
float N(int i, int k, float u);
GsVec crspline(float t, const const GsArray<GsVec>& pnts);
GsVec bospline(float t, const GsArray<GsVec>& pnts);

#endif