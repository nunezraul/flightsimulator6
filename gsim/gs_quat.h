/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_QUAT_H
# define GS_QUAT_H

/** \file gs_quat.h 
 * Quaternion for rotations */

# include <math.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_mat.h> 

/*! \class GsQuat gs_quat.h
    \brief Unit quaternion for rotations.

    A unit quaternion is represented as a four dimensional vector.
    Here we keep one scalar float element w, and three floats for
    the imaginary vector, the quaternion is then: w + x i + y j + z k */
class GsQuat
 { public :
    /*! We use a union for data access both as pointer or as floats w,x,y,z */
    union { struct { float w, x, y, z; };
            float e[4];
          };

   public :
    float norm2 () const { return w*w+x*x+y*y+z*z; } // same as q*q.conjugate()
    float norm () const  { return sqrtf(norm2()); } 
    GsQuat conjugate () const { return GsQuat(w,-x,-y,-z); }
    GsQuat inverse () const { return conjugate(); } // if not unit quat, shoud be conjugate()/norm2()
    void invert () { x=-x; y=-y; z=-z; } // unit quat version
    void normalize (); //!< Normalizes the quaternion and ensures w>=0 
    void operator *= ( float r ) { w*=r; x*=r; y*=r; z*=r; }
    GsQuat operator * ( float r ) const { return GsQuat(w*r,x*r,y*r,z*r); }
    GsQuat operator / ( float r ) const { return GsQuat(w/r,x/r,y/r,z/r); }
    GsQuat operator + ( const GsQuat &q ) const { return GsQuat(w+q.w,x+q.x,y+q.y,z+q.z); }
    float dot ( const GsQuat &q ) const { return w*q.w + x*q.x + y*q.y + z*q.z; }

   public :

    /*! A null rotation that represents a rotation around the axis (1,0,0)
        with angle 0, that generates the internal representation (1,0,0,0). */
    static const GsQuat null;

   public :

    /*! Initializes GsQuat as a null rotation. Inline. */
    GsQuat () : w(1.0f), x(0), y(0), z(0) {}

    /*! Constructor from 4 floats (qw,qx,qy,qz). Inline. */
    GsQuat ( float qw, float qx, float qy, float qz ) : w(qw), x(qx), y(qy), z(qz) {}

    /*! Constructor from 4 floats (w,x,y,z) from a float array. Inline. */
    GsQuat ( const float* f ) : w(f[0]), x(f[1]), y(f[2]), z(f[3]) {}

    /*! Copy constructor. Implemented inline. */
    GsQuat ( const GsQuat& q ) : w(q.w), x(q.x), y(q.y), z(q.z) {}

    /*! Initializes GsQuat with the rotation around the given axis and angle in
        radians. The similar method set() is called inline in this constructor. */
    GsQuat ( const GsVec& axis, float radians ) { set(axis,radians); }

    /*! Initializes GsQuat with the given axis-angle rotation, where the vector
        is the rotation axis, and its norm is the angle of rotation.
        The similar method set() is called inline in this constructor. */
    GsQuat ( const GsVec& axisangle ) { set(axisangle); }

    /*! Initializes GsQuat with the rotation from v1 to v2.
        The similar method set() is called inline in this constructor. */
    GsQuat ( const GsVec& v1, const GsVec& v2 ) { set(v1,v2); }

    /*! Initializes GsQuat extracting the rotation in matrix m.
        (this constructor *has* to be here, otherwise the float* constructor
        is called with the matrix and of course with the wrong result!)*/
    GsQuat ( const GsMat& m, char fmt='L' ) { set(m,fmt); }

    /*! Set a random quaternion, using a uniform distribution method */
    void random ();
    
    /*! Set the four values */
    void set ( float qw, float qx, float qy, float qz ) { w=qw; x=qx; y=qy; z=qz; }

    /*! Set the four values from a float pointer */
    void set ( const float* f ) { w=f[0]; x=f[1]; y=f[2]; z=f[3]; }

    /*! Defines GsQuat as the rotation from v1 to v2. */
    void set ( const GsVec& v1, const GsVec& v2 );

    /*! Defines GsQuat as the rotation around axis of the given angle in radians.
        Axis is not required to be normalized. */
    void set ( const GsVec& axis, float radians );

    /*! Initializes GsQuat with the given axis-angle rotation, where the given
        vector is the rotation axis, and its norm is the angle of rotation. */
    void set ( const GsVec& axisangle );

    /*! Defines GsQuat from the rotation in the given rotation matrix;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void set ( const GsMat& m, char fmt='L' );

    /*! Get the equivalent transformation matrix in m (translation in m is set to 0);
        fmt can be 'C' or 'L' for column (default) or line major format. */
    GsMat& get ( GsMat& m, char fmt='L' ) const;

    /*! Get the current axis and angle of rotation (in radians) that GsQuat defines. */
    void get ( GsVec& axis, float& radians ) const;

    /*! Returns GsQuat as an axis-angle rotation
        (vector is the rotation axis, and its norm is the angle of rotation). */
    void get ( GsVec& axisangle ) const;

    /*! Returns the four GsQuat values in the given f buffer */
    void get ( float* f ) const { f[0]=w; f[1]=x; f[2]=y; f[3]=z; }

    /*! Returns the rotation axis. */
    GsVec axis () const;

    /*! Returns the angle in radians. */
    float angle () const;

    /*! Returns the result of applying the rotation in quaternion q to v, ie
        in mathematical notation this is equivalent to q v q^-1 */
    GsVec apply ( const GsVec &v ) const;

    /*! rotation q1 followed by rotation q2 is equal to q2*q1 . */
    friend GsQuat operator * ( const GsQuat &q1, const GsQuat &q2 );

    /*! Comparison operator makes an exact comparison of the quaternion components. */
    friend bool operator == ( const GsQuat &q1, const GsQuat &q2 );

    /*! Comparison operator makes an exact comparison, of the quaternion components. */
    friend bool operator != ( const GsQuat &q1, const GsQuat &q2 );

    /*! Outputs data in "axis x y z ang a" format, a in degrees. */
    friend GsOutput& operator<< ( GsOutput& out, const GsQuat& q );
 };

/*! Swaps the contents of q1 with q2. */
void swap ( GsQuat &q1, GsQuat &q2 );

/*! Returns the distance between the two unit quaternions as the length
    of the geodesic arc betweeen the two points in the 4D unit sphere.
    (The same as the angle in the unit sphere) */
inline float dist ( const GsQuat& q1, const GsQuat& q2 )
       { return acosf ( q1.dot(q2) ); }

/*! Alternate distance implementation returning the weighted rotation
    distance component on the range [0,w]. */
inline float dist ( const GsQuat& q1, const GsQuat& q2, float w )
       { float d=q1.dot(q2); return w*(1.0f-GS_ABS(d)); } // d is in [-1,1]

/*! Float version of slerp(), result is placed in q.
    Note: althout parameter q1 is const, it may be automatically re-normalized.  */
void gslerp ( const float* q1, const float* q2, float t, float* q );

/*! Returns the interpolation between q1 and q2 with parameter t.
    Note: althout parameter q1 is const, it may be automatically re-normalized. */
inline GsQuat slerp ( const GsQuat &q1, const GsQuat &q2, float t )
       { GsQuat q; gslerp ( q1.e, q2.e, t, q.e ); return q; }

/*! Returns in q the interpolation between q1 and q2 with parameter t.
    Note: althout parameter q1 is const, it may be automatically re-normalized. */
inline void slerp ( const GsQuat &q1, const GsQuat &q2, float t, GsQuat &q )
       { gslerp ( q1.e, q2.e, t, q.e ); }

/*! Converts given swing-twist (sx,sy,tw) in quaternion format */
void st2quat ( float sx, float sy, float tw, GsQuat& q );

/*! Converts given twist-swing (tw,sx,sy) in quaternion format */
void ts2quat ( float tw, float sx, float sy, GsQuat& q );

/*! Converts the given quaternion q in the swing and twist parameterization.
    True is returned in case of success and the values are put in sx, sy, and tw.
    False is returned in case the converted swing reaches the singularity, and
    in such case the returned values are sx=sy=pi, and tw=0. */
bool quat2st ( const GsQuat& q, float& sx, float& sy, float& tw );
    
/*! Converts given quaternion in a twist-swing (tw,sx,sy) representation.
    Similar to quat2st(), but here it puts the twist *before* the swing. */
bool quat2ts ( const GsQuat& q, float& tw, float& sx, float& sy );

/*! Converts the rotation in the given rotation matrix to quaternion format;
    fmt can be 'C' or 'L' for column (default) or line major format. */
void mat2quat ( const GsMat& m, GsQuat& q, char fmt='L' );

/*! Equivalent to quat2mat(q,m,fmt), but retrieves only the 3x3 portion
    of the transformation matrix; fmt can be 'C' or 'L' for column (default)
    or line major format. */
void quat2mat ( const GsQuat& q, 
                float& x1, float& y1, float& z1,
                float& x2, float& y2, float& z2,
                float& x3, float& y3, float& z3, char fmt='L' );

/*! Converts the rotation encoded in the quaternion q to the transformation
    matrix in m (translation in m is set to 0); fmt can be 'C' or 'L' for 
    column (default) or line major format. */
void quat2mat ( const GsQuat& q, GsMat& m, char fmt='L' );

/*! Converts the rotation encoded in the quaternion q to the transformation
    matrix in m with given translation of t, in column major format only. */
void compose ( const GsQuat& q, const GsVec& t, GsMat& m );

/*! Removes the quaternion and translation from a matrix, in column major format only. */
inline void decompose ( const GsMat& m, GsQuat& q, GsVec& t )
     { mat2quat ( m, q ); t.set ( m.e41, m.e42, m.e43 ); }

//============================== end of file ===============================

#endif // GS_QUAT_H

