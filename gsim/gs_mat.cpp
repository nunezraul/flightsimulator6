/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/gs_mat.h>
# include <math.h>

//================================== Static Data ===================================

const GsMat GsMat::null ( 0.0, 0.0, 0.0, 0.0, 
                          0.0, 0.0, 0.0, 0.0, 
                          0.0, 0.0, 0.0, 0.0, 
                          0.0, 0.0, 0.0, 0.0 );

const GsMat GsMat::id   ( 1.0, 0.0, 0.0, 0.0, 
                          0.0, 1.0, 0.0, 0.0, 
                          0.0, 0.0, 1.0, 0.0, 
                          0.0, 0.0, 0.0, 1.0 );

// we internally use these macros in case the representation 
// has to be changed in order to be ported to a new platform;
// as it already happened...
# define E11 e11
# define E12 e12
# define E13 e13
# define E14 e14
# define E21 e21
# define E22 e22
# define E23 e23
# define E24 e24
# define E31 e31
# define E32 e32
# define E33 e33
# define E34 e34
# define E41 e41
# define E42 e42
# define E43 e43
# define E44 e44

//==================================== GsMat ========================================

GsMat::GsMat ( float a, float b, float c, float d,
               float e, float f, float g, float h, 
               float i, float j, float k, float l,
               float m, float n, float o, float p )
 {
   setl1 ( a, b, c, d );
   setl2 ( e, f, g, h );
   setl3 ( i, j, k, l );
   setl4 ( m, n, o, p );
 }

void GsMat::set ( const float *p )
 {
   setl1 ( p[0],  p[1],  p[2],  p[3]  );
   setl2 ( p[4],  p[5],  p[6],  p[7]  );
   setl3 ( p[8],  p[9],  p[10], p[11] );
   setl4 ( p[12], p[13], p[14], p[15] );
 }

void GsMat::set ( const double *p )
 {
   setl1 ( (float)p[0],  (float)p[1],  (float)p[2],  (float)p[3]  );
   setl2 ( (float)p[4],  (float)p[5],  (float)p[6],  (float)p[7]  );
   setl3 ( (float)p[8],  (float)p[9],  (float)p[10], (float)p[11] );
   setl4 ( (float)p[12], (float)p[13], (float)p[14], (float)p[15] );
 }

void GsMat::round ( float prec )
 {
   int i;
   for ( i=0; i<16; i++ )
    e[i] = gs_round ( e[i], prec );
 }

void GsMat::transpose ()
 {
   float tmp;
   GS_SWAP ( E12, E21 );
   GS_SWAP ( E13, E31 );
   GS_SWAP ( E14, E41 );
   GS_SWAP ( E23, E32 );
   GS_SWAP ( E24, E42 );
   GS_SWAP ( E34, E43 );
 }

void GsMat::transpose3x3 ()
 {
   float tmp;
   GS_SWAP ( E12, E21 );
   GS_SWAP ( E13, E31 );
   GS_SWAP ( E23, E32 );
   GS_SWAP ( E34, E43 );
 }

void GsMat::translation ( float tx, float ty, float tz, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    { setl1 (  1,  0,  0,  0 );
      setl2 (  0,  1,  0,  0 );
      setl3 (  0,  0,  1,  0 );
      setl4 ( tx,  ty, tz, 1 );
    }
   else // line-major format
    { setl1 (  1,  0,  0, tx );
      setl2 (  0,  1,  0, ty );
      setl3 (  0,  0,  1, tz );
      setl4 (  0,  0,  0,  1 );
    }
 }

void GsMat::setrans ( float tx, float ty, float tz, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    {
      e[12]=tx; e[13]=ty; e[14]=tz;
    }
   else // line-major format
    {
      e[3]=tx; e[7]=ty; e[11]=tz;
    }
 }

void GsMat::getrans ( float& tx, float& ty, float& tz, char fmt ) const
 {
   if ( fmt=='C' ) // column-major format
    {
      tx=e[12]; ty=e[13]; tz=e[14];
    }
   else // line-major format
    {
      tx=e[3]; ty=e[7]; tz=e[11];
    }
 }

void GsMat::lcombtrans ( const GsVec& v, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    {
      E41 += v.x*E11 + v.y*E21 + v.z*E31;
      E42 += v.x*E12 + v.y*E22 + v.z*E32;
      E43 += v.x*E13 + v.y*E23 + v.z*E33;
    }
   else
    {
      E14 += v.x;
      E24 += v.y;
      E34 += v.z;
    }
 }

void GsMat::rcombtrans ( const GsVec& v, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    {
      E41 += v.x;
      E42 += v.y;
      E43 += v.z;
    }
   else
    {
      E14 += v.x*E11 + v.y*E12 + v.z*E13;
      E24 += v.x*E21 + v.y*E22 + v.z*E23;
      E34 += v.x*E31 + v.y*E32 + v.z*E33;
    }
 }

void GsMat::rcombscale ( float sx, float sy, float sz )
 {
   E11*=sx; E12*=sy; E13*=sz;
   E21*=sx; E22*=sy; E23*=sz;
   E31*=sx; E32*=sy; E33*=sz;
 }

void GsMat::lcombscale ( float sx, float sy, float sz )
 {
   E11*=sx; E12*=sx; E13*=sx;
   E21*=sy; E22*=sy; E23*=sy;
   E31*=sz; E32*=sz; E33*=sz;
 }

void GsMat::scale ( float sx, float sy, float sz )
 {
   setl1 (  sx,   0,   0,   0 );
   setl2 (   0,  sy,   0,   0 );
   setl3 (   0,   0,  sz,   0 );
   setl4 (   0,   0,   0,   1 );
 }

void GsMat::rotx ( float sa, float ca, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    {
      setl1 (  1,   0,   0,  0 );
      setl2 (  0,  ca,  sa,  0 );
      setl3 (  0, -sa,  ca,  0 );
      setl4 (  0,   0,   0,  1 );
    }
   else // line-major format
    {
      setl1 (  1,   0,   0,  0 );
      setl2 (  0,  ca, -sa,  0 );
      setl3 (  0,  sa,  ca,  0 );
      setl4 (  0,   0,   0,  1 );
    }
 }

void GsMat::roty ( float sa, float ca, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    {
      setl1 (  ca,  0, -sa,  0 );
      setl2 (   0,  1,   0,  0 );
      setl3 (  sa,  0,  ca,  0 );
      setl4 (   0,  0,   0,  1 );
    }
   else // line-major format
    {
      setl1 (  ca,  0,  sa,  0 );
      setl2 (   0,  1,   0,  0 );
      setl3 ( -sa,  0,  ca,  0 );
      setl4 (   0,  0,   0,  1 );
    }
 }

void GsMat::rotz ( float sa, float ca, char fmt )
 {
   if ( fmt=='C' ) // column-major format
    {
      setl1 (  ca,  sa,  0,  0 );
      setl2 ( -sa,  ca,  0,  0 );
      setl3 (   0,   0,  1,  0 );
      setl4 (   0,   0,  0,  1 );
    }
   else // line-major format
    {
      setl1 (  ca, -sa,  0,  0 );
      setl2 (  sa,  ca,  0,  0 );
      setl3 (   0,   0,  1,  0 );
      setl4 (   0,   0,  0,  1 );
    }
 }

void GsMat::rotx ( float radians, char fmt )
 {
   rotx ( sinf(radians), cosf(radians), fmt );
 }

void GsMat::roty ( float radians, char fmt )
 {
   roty ( sinf(radians), cosf(radians), fmt );
 }

void GsMat::rotz ( float radians, char fmt )
 {
   rotz ( sinf(radians), cosf(radians), fmt );
 }

void GsMat::rot ( const GsVec& vec, float sa, float ca, char fmt )
 {
   double x, y, z, norm;
   double xx, yy, zz, xy, yz, zx, xs, ys, zs, oc;

   x=vec.x; y=vec.y; z=vec.z;

   norm = x*x + y*y + z*z;
   if ( norm!=1.0 )
    { norm = sqrt(norm);
      if (norm!=0) { x/=norm; y/=norm; z/=norm; }
    }

   // If in column-major format, just get the negative angle:
   if ( fmt=='C' ) sa*=-1.0;

   xx = x * x;   yy = y * y;   zz = z * z;
   xy = x * y;   yz = y * z;   zx = z * x;
   xs = x * sa;  ys = y * sa;  zs = z * sa;

   oc = 1.0 - ca;

   setl1 ( float(oc * xx + ca), float(oc * xy - zs), float(oc * zx + ys), 0 );
   setl2 ( float(oc * xy + zs), float(oc * yy + ca), float(oc * yz - xs), 0 );
   setl3 ( float(oc * zx - ys), float(oc * yz + xs), float(oc * zz + ca), 0 );
   setl4 (                   0,                   0,                   0, 1 );
 }

void GsMat::rot ( const GsVec& vec, float radians, char fmt )
 { 
   rot ( vec, sinf(radians), cosf(radians), fmt );
 }

void GsMat::rot ( const GsVec& from, const GsVec& to, char fmt )
 {
   float radians = angle(from,to);
   rot ( cross(from,to), sinf(radians), cosf(radians), fmt );
 }

void GsMat::projxy ( GsVec p1, GsVec p2, GsVec p3, char fmt )
 {
   # define PROJERR(d) { std::cout<<"Not a triangle in GsMat::projxy() step "<<d<<"!\n"; return; }

   GsMat m(GsMat::NoInit);
   float ca, sa, v;

   translation ( -p1.x, -p1.y, -p1.z, fmt ); // p1 goes to origin
   p2=p2-p1; p3=p3-p1;

   v = sqrtf(p2.y*p2.y + p2.z*p2.z); if (v==0) PROJERR(1);
   sa=p2.y/v; ca=p2.z/v; // rotate by x : p1p2 to xz
   m.rotx(sa,ca,fmt); p2.rotx(sa,ca); p3.rotx(sa,ca); 

   if ( fmt=='C' ) mult ( m, *this ); else mult ( *this, m );
  
   v = sqrtf(p2.x*p2.x + p2.z*p2.z); if (v==0) PROJERR(2);
   sa=p2.z/v; ca=p2.x/v; // rotate by y: p1p2 to x axis
   m.roty(sa,ca,fmt); p3.roty(sa,ca); 

   if ( fmt=='C' ) mult ( m, *this ); else mult ( *this, m );

   v = sqrtf(p3.y*p3.y + p3.z*p3.z); if (v==0) PROJERR(3);
   sa=-p3.z/v; ca=p3.y/v; // rotate by x: p3 to xy
   m.rotx(sa,ca,fmt);

   if ( fmt=='C' ) mult ( m, *this ); else mult ( *this, m );

   # undef PROJERR
 }

void GsMat::perspective ( float fovy, float aspect, float znear, float zfar )
 {
   float top = znear * tanf ( fovy/2.0f );
   float left = -top * aspect;

   float w = -left-left; // width
   float h = top+top;    // height
   float z = zfar-znear; // znear and zfar are > 0

   setl1 (    (2*znear)/w,           0,                 0,  0 );
   setl2 (              0, (2*znear)/h,                 0,  0 );
   setl3 (              0,           0,   -(zfar+znear)/z, -1 );
   setl4 (              0,           0, -(2*zfar*znear)/z,  0 );

   // Notes: 
   // The OpenGL docs show the matrix in line-major format not column-major as here

   transpose(); // back to line-major format...
 }

void GsMat::lookat ( const GsVec& eye, const GsVec& center, const GsVec& up )
 {
   GsVec f = center-eye; f.normalize(); // forward vector
   GsVec s = cross ( f, up ); s.normalize(); // side vector
   GsVec u = cross ( s, f );  // recompute up

   setl1 ( s.x, u.x, -f.x, 0 );
   setl2 ( s.y, u.y, -f.y, 0 );
   setl3 ( s.z, u.z, -f.z, 0 );
   setl4 ( dot(s,-eye), dot(u,-eye), dot(f,eye), 1.0f );

   transpose(); // back to line-major format...
 }

void GsMat::orthowin ( float w, float h )
 {
   // equiv to ortho ( 0, w-1, h-1, 0, -1, 1 )
   const float t = 2.0f;
   setl1 ( t/(w-1),       0,  0,  0 );
   setl2 (       0, t/(1-h),  0,  0 );
   setl3 (       0,       0, -1,  0 );
   setl4 (      -1,       1,  0,  1 );

   transpose(); // back to line-major format...
 }

void GsMat::ortho ( float left, float right, float bottom, float top, float near, float far )
 {
   const float t = 2.0f;
   float rml=right-left, tmb=top-bottom, nmf=near-far;
   setl1 (             t/rml,                 0,              0,  0 );
   setl2 (                 0,             t/tmb,              0,  0 );
   setl3 (                 0,                 0,          t/nmf,  0 );
   setl4 ( -(right+left)/rml, -(top+bottom)/tmb, (far+near)/nmf,  1 );

   transpose(); // back to line-major format...
 }

void GsMat::inverse ( GsMat& inv ) const
 {
   float d = det();
   if (d==0.0) return;
   d = 1.0f/d;

   float m12 = E21*E32 - E22*E31;
   float m13 = E21*E33 - E23*E31;
   float m14 = E21*E34 - E24*E31;
   float m23 = E22*E33 - E23*E32;
   float m24 = E22*E34 - E24*E32;
   float m34 = E23*E34 - E24*E33;
   inv.E11 = (E42*m34 - E43*m24 + E44*m23) * d;
   inv.E21 = (E43*m14 - E41*m34 - E44*m13) * d;
   inv.E31 = (E41*m24 - E42*m14 + E44*m12) * d;
   inv.E41 = (E42*m13 - E41*m23 - E43*m12) * d;
   inv.E14 = (E13*m24 - E12*m34 - E14*m23) * d;
   inv.E24 = (E11*m34 - E13*m14 + E14*m13) * d;
   inv.E34 = (E12*m14 - E11*m24 - E14*m12) * d;
   inv.E44 = (E11*m23 - E12*m13 + E13*m12) * d;

   m12 = E11*E42 - E12*E41;
   m13 = E11*E43 - E13*E41;
   m14 = E11*E44 - E14*E41;
   m23 = E12*E43 - E13*E42;
   m24 = E12*E44 - E14*E42;
   m34 = E13*E44 - E14*E43;
   inv.E12 = (E32*m34 - E33*m24 + E34*m23) * d;
   inv.E22 = (E33*m14 - E31*m34 - E34*m13) * d;
   inv.E32 = (E31*m24 - E32*m14 + E34*m12) * d;
   inv.E42 = (E32*m13 - E31*m23 - E33*m12) * d;
   inv.E13 = (E23*m24 - E22*m34 - E24*m23) * d;
   inv.E23 = (E21*m34 - E23*m14 + E24*m13) * d;
   inv.E33 = (E22*m14 - E21*m24 - E24*m12) * d;
   inv.E43 = (E21*m23 - E22*m13 + E23*m12) * d;
 }

float GsMat::det () const
 {
   float m12 = E21*E32 - E22*E31;
   float m13 = E21*E33 - E23*E31;
   float m14 = E21*E34 - E24*E31;
   float m23 = E22*E33 - E23*E32;
   float m24 = E22*E34 - E24*E32;
   float m34 = E23*E34 - E24*E33;
   float d1 = E12*m34 - E13*m24 + E14*m23;
   float d2 = E11*m34 - E13*m14 + E14*m13;
   float d3 = E11*m24 - E12*m14 + E14*m12;
   float d4 = E11*m23 - E12*m13 + E13*m12;
   return -E41*d1 + E42*d2 - E43*d3 + E44*d4;
 }

float GsMat::det3x3 () const
 {
   return   E11 * (E22*E33-E23*E32)
          + E12 * (E23*E31-E21*E33)
          + E13 * (E21*E32-E22*E31);
 }

float GsMat::norm2 () const
 {
   return E11*E11 + E12*E12 + E13*E13 + E14*E14 +
          E21*E21 + E22*E22 + E23*E23 + E24*E24 +
          E31*E31 + E32*E32 + E33*E33 + E34*E34 +
          E41*E41 + E42*E42 + E43*E43 + E44*E44;
 }

float GsMat::norm () const
 {
   // for better precision this should be replaced by doubles:
   return sqrtf ( norm2() );
 }

void GsMat::mult ( const GsMat& m1, const GsMat& m2 )
 {
   GsMat* m = (this==&m1||this==&m2)? new GsMat(GsMat::NoInit) : this;

   m->setl1 ( m1.E11*m2.E11 + m1.E12*m2.E21 + m1.E13*m2.E31 + m1.E14*m2.E41,
              m1.E11*m2.E12 + m1.E12*m2.E22 + m1.E13*m2.E32 + m1.E14*m2.E42,
              m1.E11*m2.E13 + m1.E12*m2.E23 + m1.E13*m2.E33 + m1.E14*m2.E43,
              m1.E11*m2.E14 + m1.E12*m2.E24 + m1.E13*m2.E34 + m1.E14*m2.E44 );

   m->setl2 ( m1.E21*m2.E11 + m1.E22*m2.E21 + m1.E23*m2.E31 + m1.E24*m2.E41,
              m1.E21*m2.E12 + m1.E22*m2.E22 + m1.E23*m2.E32 + m1.E24*m2.E42,
              m1.E21*m2.E13 + m1.E22*m2.E23 + m1.E23*m2.E33 + m1.E24*m2.E43,
              m1.E21*m2.E14 + m1.E22*m2.E24 + m1.E23*m2.E34 + m1.E24*m2.E44 );

   m->setl3 ( m1.E31*m2.E11 + m1.E32*m2.E21 + m1.E33*m2.E31 + m1.E34*m2.E41,
              m1.E31*m2.E12 + m1.E32*m2.E22 + m1.E33*m2.E32 + m1.E34*m2.E42,
              m1.E31*m2.E13 + m1.E32*m2.E23 + m1.E33*m2.E33 + m1.E34*m2.E43,
              m1.E31*m2.E14 + m1.E32*m2.E24 + m1.E33*m2.E34 + m1.E34*m2.E44 );

   m->setl4 ( m1.E41*m2.E11 + m1.E42*m2.E21 + m1.E43*m2.E31 + m1.E44*m2.E41,
              m1.E41*m2.E12 + m1.E42*m2.E22 + m1.E43*m2.E32 + m1.E44*m2.E42,
              m1.E41*m2.E13 + m1.E42*m2.E23 + m1.E43*m2.E33 + m1.E44*m2.E43,
              m1.E41*m2.E14 + m1.E42*m2.E24 + m1.E43*m2.E34 + m1.E44*m2.E44 );

   if ( m!=this ) { *this=*m; delete m; }
 }

void GsMat::add ( const GsMat& m1, const GsMat& m2 )
 {
   setl1 ( m1.E11+m2.E11, m1.E12+m2.E12, m1.E13+m2.E13, m1.E14+m2.E14 );
   setl2 ( m1.E21+m2.E21, m1.E22+m2.E22, m1.E23+m2.E23, m1.E24+m2.E24 );
   setl3 ( m1.E31+m2.E31, m1.E32+m2.E32, m1.E33+m2.E33, m1.E34+m2.E34 );
   setl4 ( m1.E41+m2.E41, m1.E42+m2.E42, m1.E43+m2.E43, m1.E44+m2.E44 );
 }

void GsMat::sub ( const GsMat& m1, const GsMat& m2 )
 {
   setl1 ( m1.E11-m2.E11, m1.E12-m2.E12, m1.E13-m2.E13, m1.E14-m2.E14 );
   setl2 ( m1.E21-m2.E21, m1.E22-m2.E22, m1.E23-m2.E23, m1.E24-m2.E24 );
   setl3 ( m1.E31-m2.E31, m1.E32-m2.E32, m1.E33-m2.E33, m1.E34-m2.E34 );
   setl4 ( m1.E41-m2.E41, m1.E42-m2.E42, m1.E43-m2.E43, m1.E44-m2.E44 );
 }

//================================= friends ========================================

float dist ( const GsMat& a, const GsMat& b )
 {
   return sqrtf ( dist2(a,b) );
 }

float dist2 ( const GsMat& a, const GsMat& b )
 {
   return (a-b).norm2();
 }

//================================= operators ========================================

void GsMat::operator *= ( float r )
 { 
   E11*=r; E12*=r; E13*=r; E14*=r;
   E21*=r; E22*=r; E23*=r; E24*=r;
   E31*=r; E32*=r; E33*=r; E34*=r;
   E41*=r; E42*=r; E43*=r; E44*=r;
 }

void GsMat::operator *= ( const GsMat& m )
 {
   GsMat* t = (this==&m)? new GsMat(GsMat::NoInit) : this;

   t->setl1 ( E11*m.E11 + E12*m.E21 + E13*m.E31 + E14*m.E41,
              E11*m.E12 + E12*m.E22 + E13*m.E32 + E14*m.E42,
              E11*m.E13 + E12*m.E23 + E13*m.E33 + E14*m.E43,
              E11*m.E14 + E12*m.E24 + E13*m.E34 + E14*m.E44 );

   t->setl2 ( E21*m.E11 + E22*m.E21 + E23*m.E31 + E24*m.E41,
              E21*m.E12 + E22*m.E22 + E23*m.E32 + E24*m.E42,
              E21*m.E13 + E22*m.E23 + E23*m.E33 + E24*m.E43,
              E21*m.E14 + E22*m.E24 + E23*m.E34 + E24*m.E44 );

   t->setl3 ( E31*m.E11 + E32*m.E21 + E33*m.E31 + E34*m.E41,
              E31*m.E12 + E32*m.E22 + E33*m.E32 + E34*m.E42,
              E31*m.E13 + E32*m.E23 + E33*m.E33 + E34*m.E43,
              E31*m.E14 + E32*m.E24 + E33*m.E34 + E34*m.E44 );

   t->setl4 ( E41*m.E11 + E42*m.E21 + E43*m.E31 + E44*m.E41,
              E41*m.E12 + E42*m.E22 + E43*m.E32 + E44*m.E42,
              E41*m.E13 + E42*m.E23 + E43*m.E33 + E44*m.E43,
              E41*m.E14 + E42*m.E24 + E43*m.E34 + E44*m.E44 );

   if ( t!=this ) { *this=*t; delete t; }
 }

void GsMat::operator += ( const GsMat& m )
 {
   E11+=m.E11; E12+=m.E12; E13+=m.E13; E14+=m.E14;
   E21+=m.E21; E22+=m.E22; E23+=m.E23; E24+=m.E24;
   E31+=m.E31; E32+=m.E32; E33+=m.E33; E34+=m.E34;
   E41+=m.E41; E42+=m.E42; E43+=m.E43; E44+=m.E44;
 }

GsMat operator * ( const GsMat& m, float r )
 {
   return GsMat ( m.E11*r, m.E12*r, m.E13*r, m.E14*r,
                  m.E21*r, m.E22*r, m.E23*r, m.E24*r,
                  m.E31*r, m.E32*r, m.E33*r, m.E34*r,
                  m.E41*r, m.E42*r, m.E43*r, m.E44*r );
 }

GsMat operator * ( float r, const GsMat& m )
 {
   return GsMat ( m.E11*r, m.E12*r, m.E13*r, m.E14*r,
                  m.E21*r, m.E22*r, m.E23*r, m.E24*r,
                  m.E31*r, m.E32*r, m.E33*r, m.E34*r,
                  m.E41*r, m.E42*r, m.E43*r, m.E44*r );
 }

GsVec operator * ( const GsMat& m, const GsVec& v )
 {
   GsVec r ( m.E11*v.x + m.E12*v.y + m.E13*v.z + m.E14,
             m.E21*v.x + m.E22*v.y + m.E23*v.z + m.E24,
             m.E31*v.x + m.E32*v.y + m.E33*v.z + m.E34  );

   float w = m.E41*v.x + m.E42*v.y + m.E43*v.z + m.E44;
   if ( w!=0.0 && w!=1.0 ) r/=w;
   return r;
 }

GsVec operator * ( const GsVec& v, const GsMat& m )
 {
   GsVec r ( m.E11*v.x + m.E21*v.y + m.E31*v.z + m.E41,
             m.E12*v.x + m.E22*v.y + m.E32*v.z + m.E42,
             m.E13*v.x + m.E23*v.y + m.E33*v.z + m.E43  );

   float w = m.E14*v.x + m.E24*v.y + m.E34*v.z + m.E44;
   if ( w!=0.0 && w!=1.0 ) r/=w;
   return r;
 }

GsMat operator * ( const GsMat& m1, const GsMat& m2 )
 {
   GsMat mat(GsMat::NoInit);
   mat.mult ( m1, m2 );
   return mat;
 }

GsMat operator + ( const GsMat& m1, const GsMat& m2 )
 {
   GsMat mat(GsMat::NoInit);
   mat.add ( m1, m2 );
   return mat;
 }

GsMat operator - ( const GsMat& m1, const GsMat& m2 )
 {
   GsMat mat(GsMat::NoInit);
   mat.sub ( m1, m2 );
   return mat;
 }

bool operator == ( const GsMat& m1, const GsMat& m2 )
 {
   return  m1.E11==m2.E11 && m1.E12==m2.E12 && m1.E13==m2.E13 && m1.E14==m2.E14 &&
           m1.E21==m2.E21 && m1.E22==m2.E22 && m1.E23==m2.E23 && m1.E24==m2.E24 &&
           m1.E31==m2.E31 && m1.E32==m2.E32 && m1.E33==m2.E33 && m1.E34==m2.E34 &&
           m1.E41==m2.E41 && m1.E42==m2.E42 && m1.E43==m2.E43 && m1.E44==m2.E44 ? true:false;
 }

bool operator != ( const GsMat& m1, const GsMat& m2 )
 {
   return  m1.E11!=m2.E11 || m1.E12!=m2.E12 || m1.E13!=m2.E13 || m1.E14!=m2.E14 ||
           m1.E21!=m2.E21 || m1.E22!=m2.E22 || m1.E23!=m2.E23 || m1.E24!=m2.E24 ||
           m1.E31!=m2.E31 || m1.E32!=m2.E32 || m1.E33!=m2.E33 || m1.E34!=m2.E34 ||
           m1.E41!=m2.E41 || m1.E42!=m2.E42 || m1.E43!=m2.E43 || m1.E44!=m2.E44 ? true:false;
 }

GsOutput& operator<< ( GsOutput& o, const GsMat& m )
 { 
   o << m.E11 <<' '<< m.E12 <<' '<< m.E13 <<' '<< m.E14 << gsnl;
   o << m.E21 <<' '<< m.E22 <<' '<< m.E23 <<' '<< m.E24 << gsnl;
   o << m.E31 <<' '<< m.E32 <<' '<< m.E33 <<' '<< m.E34 << gsnl;
   o << m.E41 <<' '<< m.E42 <<' '<< m.E43 <<' '<< m.E44 << gsnl;
   return o;
 }

GsInput& operator>> ( GsInput& in, GsMat& m )
 { 
   return in >> m.E11 >> m.E12 >> m.E13 >> m.E14
             >> m.E21 >> m.E22 >> m.E23 >> m.E24
             >> m.E31 >> m.E32 >> m.E33 >> m.E34
             >> m.E41 >> m.E42 >> m.E43 >> m.E44;
 }

//================================== End of File ===========================================

/* also works with:
# define E11 e[0]
# define E12 e[1]
# define E13 e[2]
# define E14 e[3]
# define E21 e[4]
# define E22 e[5]
# define E23 e[6]
# define E24 e[7]
# define E31 e[8]
# define E32 e[9]
# define E33 e[10]
# define E34 e[11]
# define E41 e[12]
# define E42 e[13]
# define E43 e[14]
# define E44 e[15]*/
