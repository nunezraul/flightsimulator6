/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/gs_quat.h>

//============================== Static Data ====================================

const GsQuat GsQuat::null ( 1.0f, 0, 0, 0 );

//============================ public members ====================================

void GsQuat::normalize ()
 {
   float f = sqrtf( norm2() );
   if ( f==0 ) return;
   w/=f; x/=f; y/=f; z/=f;
   if ( w<0 ) { w=-w; x=-x; y=-y; z=-z; }
 }

// from: Kuffner, "Effective Sampling and Distance Metrics for 3D Rigid Body Path Planning"
void GsQuat::random ()
 {
   float s = gs_random();
   float s1 = sqrtf ( 1-s );
   float s2 = sqrtf ( s );
   float t1 = gs2pi * gs_random();
   float t2 = gs2pi * gs_random();
   w = cosf(t2) * s2;
   x = sinf(t1) * s1;
   y = cosf(t1) * s1;
   z = sinf(t2) * s2;
 }

inline float GSVECANGLE ( const GsVec& v1, const GsVec& v2 ) { return ::angle(v1,v2); }

void GsQuat::set ( const GsVec& v1, const GsVec& v2 )
 {
/* TO TEST ONE DAY:
   instead of calling acos in angle, do:
   v1.normalize();
   v2.normalize();
   r = cross(v1,v2);
   s = sqrt ( 2 * ( 1+dot(v1,v2) ) );
   q = ( 2s, r/s ); */
   float ang = GSVECANGLE(v1,v2);
   if ( GS_NEXT(ang,gspi,gstiny) )
    set ( GsVec::i, gspi );
   else
    set ( cross(v1,v2), ang );
 }

void GsQuat::set ( const GsVec& axis, float radians )
 { 
   float f;
   
   // normalize axis:
   x=axis.x; y=axis.y; z=axis.z;
   f = x*x + y*y + z*z;

   if ( f==0||radians==0 ) { *this=null; return; }

   if ( f>0 )
    { f = sqrtf ( f );
      x/=f; y/=f; z/=f;
    }
    
   // set the quaternion:
   radians/=2;
   f = sinf ( radians );
   x*=f; y*=f; z*=f;
   w = cosf ( radians );
 }

void GsQuat::set ( const GsVec& axisangle )
 { 
   float ang;

   // normalize axis ang extract angle:
   x=axisangle.x; y=axisangle.y; z=axisangle.z;
   ang = x*x + y*y + z*z;
   if ( ang>0 )
    { ang = sqrtf ( ang );
      x/=ang; y/=ang; z/=ang;
    }

   // set the quaternion:
   ang/=2;
   w = cosf ( ang );
   ang = sinf ( ang );
   x*=ang; y*=ang; z*=ang;
 }

void GsQuat::get ( GsVec& axis, float& radians ) const
 {
   // if GsQuat==(1,0,0,0), the axis will be null, so we
   // set the axis to (1,0,0) (GsVec::i); the angle will be 0.
   // this is also done in GsQuat::axis()
   axis.set ( x, y, z );
   float n = axis.norm();
   if ( n==0 ) axis=GsVec::i; else axis/=n;
   radians = 2.0f * acosf ( w );
 }

void GsQuat::get ( GsVec& axisangle ) const
 {
   axisangle.set ( x, y, z );
   axisangle.len ( 2.0f * acosf ( w ) );
 }

void GsQuat::set ( const GsMat& m, char fmt )
 {
   mat2quat(m,*this,fmt);
 }

GsMat& GsQuat::get ( GsMat& m, char fmt ) const
 { 
   quat2mat(*this,m,fmt);
   return m;
 }

GsVec GsQuat::axis () const
 {
   GsVec axis ( x, y, z );
   float n = axis.norm();
   if ( n==0 ) axis=GsVec::i; else axis/=n;
   return axis;
 }

float GsQuat::angle () const
 {
   return 2.0f * acosf ( w );
 }

GsVec GsQuat::apply ( const GsVec &v ) const
 {
   /* TO TEST:
      return (1-w*w)p + 2(dot(v,p))v + dot(2w,cross(v,p)) */
   GsQuat qv ( 0, v.x, v.y, v.z );
   qv = (*this) * qv * conjugate();
   return GsVec ( qv.x, qv.y, qv.z );
 }

//=================================== Friend Functions ===================================

GsQuat operator * ( const GsQuat &q1, const GsQuat &q2 )
 {
   GsQuat q;

   // the following is the same as:
   // ( w1*w2-dot(v1,v2), w1*v1 + w2*v2 + cross (v1,v2) )
   q.w = (q1.w*q2.w) - (q1.x*q2.x + q1.y*q2.y + q1.z*q2.z); // w1*w2-dot(v1,v2)
   q.x = q1.y*q2.z - q1.z*q2.y; // cross (q1.v,q2.v)
   q.y = q1.z*q2.x - q1.x*q2.z;
   q.z = q1.x*q2.y - q1.y*q2.x;
   q.x += (q1.x*q2.w) + (q2.x*q1.w); // += w1*v1 + w2*v2 
   q.y += (q1.y*q2.w) + (q2.y*q1.w);
   q.z += (q1.z*q2.w) + (q2.z*q1.w);

   return q;
 }

bool operator == ( const GsQuat &q1, const GsQuat &q2 )
 { 
   return q1.w==q2.w && q1.x==q2.x && q1.y==q2.y && q1.z==q2.z ? true:false; 
 }

bool operator != ( const GsQuat &q1, const GsQuat &q2 )
 { 
   return q1.w==q2.w && q1.x==q2.x && q1.y==q2.y && q1.z==q2.z ? false:true; 
 }

//=================================== Global Functions ===================================

void swap ( GsQuat &q1, GsQuat &q2 )
 {
   float tmp;
   GS_SWAP(q1.w,q2.w);
   GS_SWAP(q1.x,q2.x);
   GS_SWAP(q1.y,q2.y);
   GS_SWAP(q1.z,q2.z);
 }

void gslerp ( const float* q1const, const float* q2, float t, float* q )
 { 
   float* q1 = (float*) q1const; // q1 is actually a "mutable const"
   float dot = q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2] + q1[3]*q2[3];
   if ( dot < 0 ) 
    { // the quaternions are pointing in opposite directions, so
      // use the equivalent alternative representation for q1
      q1[0]=-q1[0]; q1[1]=-q1[1]; q1[2]=-q1[2]; q1[3]=-q1[3];
      dot = -dot;
    }

   // interpolation factors
   float r, s;
   
   // decide according to an epsilon (30fps motions are of E-6 order)
   // this IS needed for baked motions in order to avoid dealing with the E-6 values in floats
   if ( 1.0f-dot < 0.01f )
    { // the quaternions are nearly parallel, just use linear interpolation
      r = 1-t;
      s = t;
    }
   else
    { // calculate spherical linear interpolation factors
      float a = acosf(dot);
      float g = 1.0f / sinf(a);
      r = sinf ( (1-t)*a ) * g;
      s = sinf ( t*a ) * g;
    }
    
   // set the interpolated quaternion
   q[0] = r*q1[0] + s*q2[0];
   q[1] = r*q1[1] + s*q2[1];
   q[2] = r*q1[2] + s*q2[2];
   q[3] = r*q1[3] + s*q2[3];

   // check if we should normalize it, typically f will be inside [0.99,1.01],
   // here we choose the level to renormalize the result:
   float f = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
   if ( (f>0.999f&&f<1.001f) || f==0 ) return;
   f = sqrtf(f);
   q[0]/=f; q[1]/=f; q[2]/=f; q[3]/=f;
   if ( q[0]<0 ) { q[0]=-q[0]; q[1]=-q[1]; q[2]=-q[2]; q[3]=-q[3]; }
 }

GsOutput& operator<< ( GsOutput& out, const GsQuat& q )
 {
   return out << "axis " << q.axis() << " ang " << GS_TODEG(q.angle());
 }

//=============================== swing-twist conversions ======================================

void st2quat ( float sx, float sy, float tw, GsQuat& q )
 {
   q.set ( GsVec(sx,sy,0.0f) ); // swing component in axis-angle

   if ( tw!=0 ) // compose twist component
    { GsQuat twist ( GsVec::k, tw );
      q = q * twist;
    }
 }

void ts2quat ( float tw, float sx, float sy, GsQuat& q )
 {
   q.set ( GsVec(sx,sy,0.0f) ); // swing component in axis-angle

   if ( tw!=0 ) // compose twist component
    { GsQuat twist ( GsVec::k, tw );
      q = twist * q;
    }
 }

bool quat2st ( const GsQuat& q, float& sx, float& sy, float& tw )
 {
   // Decompose q into swing-twist (from Paolo's thesis).
   // First test if the swing is in the singularity:
   if ( GS_NEXTZ(q.z,gstiny) && GS_NEXTZ(q.w,gstiny) ) { sx=sy=gspi; tw=0; return false; }

   // Get q in double and normalize q to always have qw>0 :
   double qw, qx, qy, qz;
   if ( q.w<0 )
    { qw=-q.w; qx=-q.x; qy=-q.y; qz=-q.z; }
   else
    { qw=q.w; qx=q.x; qy=q.y; qz=q.z; }

   // Get the twist t:
   double t = 2.0 * atan2(qz,qw);

   double bet = atan2( sqrt(qx*qx+qy*qy), sqrt(qz*qz+qw*qw) );
   double gam = t/2.0;
   double sinc = GS_NEXTZ(bet,gstiny)? 1.0 : sin(bet)/bet;
   double singam = sin(gam);
   double cosgam = cos(gam);
   
   sx = float( (2.0/sinc) * (cosgam*qx - singam*qy) );
   sy = float( (2.0/sinc) * (singam*qx + cosgam*qy) );
   tw = float( t );
   
   return true;
 }

bool quat2ts ( const GsQuat& q, float& tw, float& sx, float& sy )
 {
   // First test if the swing is in the singularity:
   if ( GS_NEXTZ(q.z,gstiny) && GS_NEXTZ(q.w,gstiny) ) { sx=sy=gspi; tw=0; return false; }

   // Decompose q into twist-swing
   // by solving the equation Qtwist(t*2) * Qswing(s*2) = q
   // note: (x,y) is the normalized swing axis (x*x+y*y=1)
   // ( Ct 0 0 St ) * ( Cs xSs ySs 0 ) = ( qw qx qy qz )
   // ( CtCs  xSsCt-yStSs  xStSs+ySsCt  StCs ) = ( qw qx qy qz )  (1)

   // From (1) CtCs / StCs = qw/qz => Ct/St = qw/qz => tan(t) = qz/qw (2)
   // The swing rotation/2 s comes from:
   // From (1) (CtCs)^2 + (StCs)^2 = qw^2 + qz^2 =>  Cs = sqrt ( qw^2 + qz^2 ) (3)
   // From (1) (xSsCt-yStSs)^2 + (xStSs+ySsCt)^2 = qx^2 + qy^2 => Ss = sqrt ( qx^2 + qy^2 ) (4)
   // From (1) : |SsCt -StSs| |x| = |qx|
   //            |StSs +SsCt| |y|   |qy| (5)

   double qw, qx, qy, qz;
   if ( q.w<0 )
    { qw=-q.w; qx=-q.x; qy=-q.y; qz=-q.z; }
   else
    { qw=q.w; qx=q.x; qy=q.y; qz=q.z; }
      
   double t = atan2 ( qz, qw ); // from (2)
   double s = atan2( sqrt(qx*qx+qy*qy), sqrt(qz*qz+qw*qw) ); // from (3) and (4)

   double x=0, y=0, sins=sin(s);

   if ( !GS_NEXTZ(sins,gstiny) )
    { double sint = sin(t);
      double cost = cos(t);

      // by solving the linear system in (5):
      y = (-qx*sint + qy*cost)/sins;
      x = ( qx*cost + qy*sint)/sins;
    }

   tw = float(2.0*t);
   sx = float(x*2.0*s);
   sy = float(y*2.0*s);
   return true;
 }

void mat2quat ( const GsMat& m, GsQuat& q, char fmt )
 {
   # define E(i)   m(i)
   # define M(i,j) m.cget(i,j)
   # define Q(i)   q.e[i+1] // Q(0)=x, Q(1)=y, Q(2)=z 

   float s;
   float tr = E(0) + E(5) + E(10);

   if ( tr>0 )
    { s = sqrtf ( 1.0f + tr );
      q.w = s / 2.0f;
      s = 0.5f / s;
      q.x = (E(6) - E(9)) * s;
      q.y = (E(8) - E(2)) * s;
      q.z = (E(1) - E(4)) * s;
    }
   else
    { int i, j, k;

      i = M(1,1)>M(0,0)? 1:0;
      if ( M(2,2)> M(i,i) ) i=2;
      j = (i+1)%3;
      k = (j+1)%3;

      s = sqrtf ( (M(i,i) - (M(j,j)+M(k,k))) + 1.0f );
      Q(i) = s * 0.5f;

      if ( s!=0 ) // s should never be equal to 0 if matrix is orthogonal 
       s = 0.5f / s;

      q.w  = (M(j,k) - M(k,j)) * s;
      Q(j) = (M(i,j) + M(j,i)) * s;
      Q(k) = (M(i,k) + M(k,i)) * s;
    }

   if ( fmt=='L' ) { q.x=-q.x; q.y=-q.y; q.z=-q.z; } // invert (for unit quat)

   # undef E
   # undef M
   # undef Q
 }

void quat2mat ( const GsQuat& q, 
                   float& x1, float& y1, float& z1,
                   float& x2, float& y2, float& z2,
                   float& x3, float& y3, float& z3, char fmt )
 {
   x2  = q.x+q.x;
   float x2x = x2*q.x;
   float x2y = x2*q.y;
   float x2z = x2*q.z;
   float x2w = x2*q.w;
   y2  = q.y+q.y;
   float y2y = y2*q.y;
   float y2z = y2*q.z;
   float y2w = y2*q.w;
   z2  = q.z+q.z;
   float z2z = z2*q.z;
   float z2w = z2*q.w;

   if ( fmt=='C' )
    {
      x1 = 1.0f - y2y - z2z; y1 = x2y + z2w;        z1 = x2z - y2w;
      x2 = x2y - z2w;        y2 = 1.0f - x2x - z2z; z2 = y2z + x2w;
      x3 = x2z + y2w;        y3 = y2z - x2w;        z3 = 1.0f - x2x - y2y;
    }
   else
    { 
      x1 = 1.0f - y2y - z2z; y1 = x2y - z2w;        z1 = x2z + y2w;
      x2 = x2y + z2w;        y2 = 1.0f - x2x - z2z; z2 = y2z - x2w;
      x3 = x2z - y2w;        y3 = y2z + x2w;        z3 = 1.0f - x2x - y2y;
    }
 }

void quat2mat ( const GsQuat& q, GsMat& m, char fmt )
 {
   quat2mat ( q, m[0], m[1], m[2],
                 m[4], m[5], m[6],
                 m[8], m[9], m[10], fmt );

   m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
   m[15] = 1.0f;
 }

void compose ( const GsQuat& q, const GsVec& t, GsMat& m )
 {
   quat2mat ( q, m[0], m[1], m[2],
                 m[4], m[5], m[6],
                 m[8], m[9], m[10], 'C' );

   m[3] = m[7] = m[11] = 0.0f;
   m.setl4 ( t.x, t.y, t.z, 1.0f );
 }

//================================== End of File =======================================

