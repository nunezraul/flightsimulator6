/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/
/** \file gs_vec2.h 
 * Two dimensional vec2tor */

# ifndef GS_VEC2_H
# define GS_VEC2_H

# include <gsim/gs.h> 
# include <gsim/gs_input.h> 

class GsVec2;

/*! This "point typedef" is used only to coherently describe variables,
    making possible to distiguish points from vectors parameters when
    interpreting the meaning of parameters. */
typedef GsVec2 GsPnt2;

/*! \class GsVec2 gs_vec2.h
    \brief Two dimensional vector. 

    Its two coordinates are of type float. */
class GsVec2
 { public :
    union { struct{float x, y;};
            float e[2];
          };
   public :
    static const GsVec2 null;     //!< (0,0) null vector
    static const GsVec2 one;      //!< (1,1) vector
    static const GsVec2 minusone; //!< (-1,-1) vector
    static const GsVec2 i;        //!< (1,0) vector
    static const GsVec2 j;        //!< (0,1) vector
   public :

    /*! Initializes GsVec2 as a null vector. Implemented inline. */
    GsVec2 () : x(0), y(0) {} 

    /*! Copy constructor. Implemented inline. */
    GsVec2 ( const GsVec2& v ) : x(v.x), y(v.y) {}

    /*! Initializes with the two given float coordinates. Implemented inline. */
    GsVec2 ( float a, float b ) : x(a), y(b) {}

    /*! Initializes with the two given int coordinates converted to floats. Implemented inline. */
    GsVec2 ( int a, int b ) : x(float(a)), y(float(b)) {}

    /*! Initializes with one int and one float. Implemented inline. */
    GsVec2 ( int a, float b ) : x(float(a)), y(b) {}

    /*! Initializes with one float and one int. Implemented inline. */
    GsVec2 ( float a, int b ) : x(a), y(float(b)) {}

    /*! Initializes with the two given double coordinates converted to floats. Implemented inline. */
    GsVec2 ( double a, double b ) : x(float(a)), y(float(b)) {}

    /*! Initializes from a float pointer. Implemented inline. */
    GsVec2 ( const float* p ) : x(p[0]), y(p[1]) {}

    /*! Set coordinates from the given vector. Implemented inline. */
    void set ( const GsVec2& v ) { x=v.x; y=v.y; }

    /*! Set coordinates from the two given float values. Implemented inline. */
    void set ( float a, float b ) { x=a; y=b; }

    /*! Set coordinates from the two given int values. Implemented inline. */
    void set ( int a, int b ) { x=float(a); y=float(b); }

    /*! Set coordinates from the two given double values. Implemented inline. */
    void set ( double a, double b ) { x=float(a); y=float(b); }

    /*! Set coordinates from an int and a float. Implemented inline. */
    void set ( int a, float b ) { x=float(a); y=b; }

    /*! Set coordinates from a float and an int. Implemented inline. */
    void set ( float a, int b ) { x=a; y=float(b); }

    /*! Apply a rotation to GsVec2 by angle a, where sa=sin(a), and ca=cos(a). */
    void rot ( float sa, float ca ) { set ( x*ca-y*sa, x*sa+y*ca ); }

    /*! Apply a rotation to GsVec2 by the given angle in radians. */
    void rot ( float radians );

    /*! Apply a rotation around cent by the given sin/cos of the angle. */
    void rot ( const GsVec2& cent, float sa, float ca );

    /*! Apply a rotation around cent, by the given angle in radians. */
    void rot ( const GsVec2& cent, float radians );

    /*! Makes each coordinate to have its absolute value. */
    void abs () { x=GS_ABS(x); y=GS_ABS(y); }

    /*! Normalize the vector. If its norm is 0 or 1, the vector is not changed. */
    void normalize ();

    /*! Sets the length to be n and returns the previous length.
        It this is a null vector, nothing is done. */
    float len ( float n );

    /*! Returns the length of the vector. Equivalent to norm() */
    float len () { return norm(); }

    /*! Returns the norm (distance to zero) of the vector. */
    float norm () const;

    /*! Returns the square of the norm (x*x + y*y). Implemented inline. */
    float norm2 () const { return x*x + y*y; }

    /*! Returns the norm, but using the max function of distance, which does not require sqrt() */
    float normax () const;

    /*! Returns the angle from the vector (0,0) to GsVec2. The angle will be a
        value in radians inside the interval [0,pi], and uses atan2. */
    float angle () const; // [0,pi]

    /*! Returns the angle from the vector (0,0) to GsVec2, but using the max norm. The angle 
        will then be a value inside the interval [0,8). */
    float angmax () const; // [0,8)

    /*! Returns the ccw orthogonal vector (-y,x). */
    GsVec2 ortho () const { return GsVec2(-y,x); }

    /*! Returns true if all coordinates are zero; false otherwise. */
    bool isnull () const { return x==0.0 && y==0.0; }

    /*! Returns true if the vector is inside the closed neighboorhood of point 0,0 and radius ds. */
    bool nextnull ( float ds ) const { return norm2()<=ds*ds; }

    /*! Allows member access like a vector */
    float& operator[] ( int i ) { return e[i]; }

    /*! Parenthesis allows const member access. */
    const float& operator() ( int i ) const { return e[i]; }

    /*! Convertion to a float pointer. */
    operator const float* () const { return e; }

    /*! Assignment operator from another GsVec2. Implemented inline. */
    void operator = ( const GsVec2& v ) { set(v); }

    /*! Assignment operator from a const float *. Implemented inline. */
    void operator = ( const float *v ) { x=(float)v[0]; y=(float)v[1]; }

    /*! -= operator with another GsVec2. Implemented inline. */
    void operator -= ( const GsVec2& v ) { x-=v.x; y-=v.y; }

    /*! += operator with another GsVec2. Implemented inline. */
    void operator += ( const GsVec2& v ) { x+=v.x; y+=v.y; }

    /*! *= operator with another GsVec2. Implemented inline. */
    void operator *= ( float r ) { x*=r; y*=r; }

    /*! /= operator with another GsVec2. Implemented inline. */
    void operator /= ( float r ) { x/=r; y/=r; }

    /*! Unary minus. Implemented inline. */
    friend GsVec2 operator - ( const GsVec2& v )
           { return GsVec2 ( -v.x, -v.y ); }

    /*! Adds two vectors. Implemented inline. */
    friend GsVec2 operator + ( const GsVec2& v1, const GsVec2& v2 ) 
           { return GsVec2 (v1.x+v2.x, v1.y+v2.y); }

    /*! Subtracts two vectors. Implemented inline. */
    friend GsVec2 operator - ( const GsVec2& v1, const GsVec2& v2 ) 
           { return GsVec2 (v1.x-v2.x, v1.y-v2.y); }

    /*! Comparison operator. Implemented inline. */
    friend bool operator == ( const GsVec2& v1, const GsVec2& v2 )
           { return v1.x==v2.x && v1.y==v2.y; }

    /*! Comparison operator. Implemented inline. */
    friend bool operator != ( const GsVec2& v1, const GsVec2& v2 )
           { return v1.x!=v2.x || v1.y!=v2.y; }

    /*! Multiplication by scalar. Implemented inline. */
    friend GsVec2 operator * ( const GsVec2& v, float r )
           { return GsVec2 (v.x*r, v.y*r); }

    /*! Division by scalar. Implemented inline. */
    friend GsVec2 operator / ( const GsVec2& v, float r )
           { return GsVec2 (v.x/r, v.y/r); }

    /*! Outputs in format: "x y". */
    friend GsOutput& operator<< ( GsOutput& o, const GsVec2& v ) { return o << v.x <<' '<< v.y; }

    /*! Inputs from format: "x y". */
    friend GsInput& operator>> ( GsInput& in, GsVec2& v ) { return in >> v.x >> v.y; }
 };

/*! Swaps the contents of v1 with v2. */
void swap ( GsVec2& v1, GsVec2& v2 );

/*! Returns the distance between v1 and v2 using norm_max(). */
float distmax ( const GsVec2& v1, const GsVec2& v2 );

/*! Returns the distance between p1 and p2. */
float dist ( const GsVec2& p1, const GsVec2& p2 );

/*! Returns the square of the distance between p1 and p2. */
float dist2 ( const GsVec2& p1, const GsVec2& p2 );

/*! Returns true if dist(v1,v2)<=ds, otherwise returns false. Implemented inline. */
inline bool next ( const GsVec2& v1, const GsVec2& v2, float ds )
       { return dist2(v1,v2)<=ds*ds; }

/*! Returns true if dist2(v1,v2)<=ds2, otherwise returns false. Implemented inline. */
inline bool next2 ( const GsVec2& v1, const GsVec2& v2, float ds2 )
       { return dist2(v1,v2)<=ds2; }

/*! Returns the angle in radians between v1 and v2,
    which is inside the interval [0,pi] */
float angle ( const GsVec2& v1, const GsVec2& v2 );

/*! Returns the angle between v1 and v2, but considering that
    v1 and v2 are already normalized, ie, returns acosf(dot(v1,v2)). */
float anglenorm ( const GsVec2 &v1, const GsVec2 &v2 );

/*! Returns the oriented angle in radians between v1 and v2,
    which is inside the interval (-pi,pi] */
float oriangle ( const GsVec2& v1, const GsVec2& v2 );

/*! Returns the angle between v1 and v2, but using the max norm.
    The angle will be a value inside the interval [0,4]. */
float angmax ( const GsVec2& v1, const GsVec2& v2 ); // [0,4]

/*! Returns the oriented angle between v1 and v2, but using the max norm.
    The angle will be a value inside the interval (-4,4]. */
float oriangmax ( const GsVec2 &v1, const GsVec2 &v2 ); // (-4,4]

/*! Returns the cross product z coordinate of v1 and v2: a zero value means
    that the vectors are colinear, otherwise, a positive number means that
    v2 is on the left side of v1, and a negative number means that v2 is on
    the right side of v1. */
inline float cross ( const GsVec2& v1, const GsVec2& v2 ) { return v1.x*v2.y - v1.y*v2.x; }

/*! Returns the dot product of v1 and v2 (v1.x*v2.x + v1.y*v2.y). */
inline float dot ( const GsVec2& v1, const GsVec2& v2 ) { return v1.x*v2.x + v1.y*v2.y; }

/*! Returns the linear interpolation of v1 v2 in parameter t ( v1(1-t)+v2(t) ). */
inline GsVec2 mix ( const GsVec2& v1, const GsVec2& v2, float t ) { return v1*(1.0f-t) + v2*t; }

/*! Returns -1,0,1 depending if v1 is less, equal or greater than v2, 
    using a comparison by coordinates, ie, lexicographical. */
int compare ( const GsVec2& v1, const GsVec2& v2 );

/*! Pointer version of the lexicographical compare function. */
int compare ( const GsVec2* v1, const GsVec2* v2 );

/*! Returns -1,0,1 depending if v1 is less, equal or greater than v2, 
    using a 2d polar comparison in relation to (1,0) (using angmax()). */
int comparepol ( const GsVec2& v1, const GsVec2& v2 );

/*! Pointer version of the compare_polar function. */
int comparepol ( const GsVec2* v1, const GsVec2* v2 );

/*! Returns u,v,w==1-u-v, u+v+w==1, such that p1*u + p2*v + p3*w == p */
void barycentric ( const GsPnt2& p1, const GsPnt2& p2, const GsPnt2& p3, const GsPnt2& p,
                   float& u, float& v, float& w );

/*! Wrapper for the gs_ccw() function in gs_geo2.h (ccw returns 2 times the triangle area) */
double ccw ( const GsVec2& p1, const GsVec2& p2, const GsVec2& p3 );

/*! Returns the triangle area by calling ccw()/2 */
inline float area ( const GsVec2& p1, const GsVec2& p2, const GsVec2& p3 ) { return float(ccw(p1,p2,p3)/2.0); }

//============================== end of file ===============================

# endif // GS_VEC2_H
