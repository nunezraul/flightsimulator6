/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

/** \file gs_vec.h 
 * Three dimensional vector */

# ifndef GS_VEC_H
# define GS_VEC_H

# include <gsim/gs_vec2.h> 

class GsVec;

/*! This "point typedef" is used mainly to coherently describe variables,
    making possible to distiguish points from vector parameters. */
typedef GsVec GsPnt;

/*! \class GsVec gs_vec.h
    \brief Three dimensional vector. 

    The vector three coordinates are of type float. For most convenience, its
    data members can be accessed both as a float pointer or as three float 
    data members by using a union of both types. */
class GsVec
 { public :
    union { struct{float x, y, z;};
            float e[3];
          };

   public :
    static const GsVec null;     //!< (0,0,0) null vector
    static const GsVec one;      //!< (1,1,1) vector
    static const GsVec minusone; //!< (-1,-1,-1) vector
    static const GsVec i;        //!< (1,0,0) vector
    static const GsVec j;        //!< (0,1,0) vector
    static const GsVec k;        //!< (0,0,1) vector

   public :

    /*! Initializes GsVec as a null vector. Implemented inline. */
    GsVec () : x(0), y(0), z(0) {}

    /*! Initializes GsVec from an int pointer. Implemented inline. */
    GsVec ( const int *v ) : x((float)v[0]), y((float)v[1]), z((float)v[2]) {}

    /*! Initializes from a float pointer. Implemented inline. */
    GsVec ( const float *v ) : x(v[0]), y(v[1]), z(v[2]) {}

    /*! Initializes from a double pointer. Implemented inline. */
    GsVec ( const double *v ) : x((float)v[0]), y((float)v[1]), z((float)v[2]) {}

    /*! Initializes from a 2d vector with z coordinate is set to 0. Implemented inline. */
    GsVec ( const GsVec2& v ) : x(v.x), y(v.y), z(0) {}

    /*! Copy constructor. Implemented inline. */
    GsVec ( const GsVec& v ) : x(v.x), y(v.y), z(v.z) {}

    /*! Initializes with the three given float coordinates. Implemented inline. */
    GsVec ( float a, float b, float c ) : x(a), y(b), z(c) {}

    /*! Initializes with the given int coordinates converted to floats. Implemented inline. */
    GsVec ( int a, int b, int c ) : x(float(a)), y(float(b)), z(float(c)) {}

    /*! Initializes with the given double coordinates converted to floats. Implemented inline. */
    GsVec ( double a, double b, double c ) : x(float(a)), y(float(b)), z(float(c)) {}

    /*! All combinations of int and float typecasts are provided for the constructor */
    GsVec ( int a,   int b,   float c ) : x(float(a)), y(float(b)), z(c) {}
    GsVec ( int a,   float b, int c   ) : x(float(a)), y(b), z(float(c)) {}
    GsVec ( float a, int b,   int c   ) : x(a), y(float(b)), z(float(c)) {}
    GsVec ( float a, float b, int c   ) : x(a), y(b), z(float(c)) {}
    GsVec ( int a,   float b, float c ) : x(float(a)), y(b), z(c) {}
    GsVec ( float a, int b,   float c ) : x(a), y(float(b)), z(c) {}

    /*! Set coordinates from an int pointer. Implemented inline. */
    void set ( const int *v ) { x=(float)v[0]; y=(float)v[1]; z=(float)v[2]; }

    /*! Set coordinates from a float pointer. Implemented inline. */
    void set ( const float *v ) { x=v[0]; y=v[1]; z=v[2]; }

    /*! Set coordinates from a double pointer. Implemented inline. */
    void set ( const double *v ) { x=(float)v[0]; y=(float)v[1]; z=(float)v[2]; }

    /*! Set coordinates from the three given float values. Implemented inline. */
    void set ( float a, float b, float c ) { x=a; y=b; z=c; }

    /*! Set coordinates from the given int values. Implemented inline. */
    void set ( int a, int b, int c ) { x=float(a); y=float(b); z=float(c); }

    /*! All combinations of int and float typecasts are provided for the set() method */
    void set ( int a,   int b,   float c ) { x=float(a); y=float(b); z=c;        }
    void set ( int a,   float b, int c   ) { x=float(a); y=b;        z=float(c); }
    void set ( float a, int b,   int c   ) { x=a;        y=float(b); z=float(c); }
    void set ( float a, float b, int c   ) { x=a;        y=b;        z=float(c); }
    void set ( int a,   float b, float c ) { x=float(a); y=b;        z=c;        }
    void set ( float a, int b,   float c ) { x=a;        y=float(b); z=c;        }

    /*! Set coordinates from the given double values. Implemented inline. */
    void set ( double a, double b, double c ) { x=float(a); y=float(b); z=float(c); }

    /*! Set coordinates from another GsVec. Implemented inline. */
    void set ( const GsVec& v ) { x=v.x; y=v.y; z=v.z; }

    /*! Set x,y from a 2d vector and set z to 0. Implemented inline. */
    void set ( const GsVec2& v ) { x=v.x; y=v.y; z=0; }

    /*! Put in f the coordinate values. */
    void get ( float f[3] ) const { f[0]=x; f[1]=y; f[2]=z; }

    /*! Apply a rotation to GsVec around the X axis with
        the angle 'a' where sa=sin(a) and ca=cos(a). */
    void rotx ( float sa, float ca ) { set ( x, y*ca-z*sa, y*sa+z*ca ); }

    /*! Apply a rotation to GsVec around the Y axis with 
        the angle 'a' where sa=sin(a) and ca=cos(a). */
    void roty ( float sa, float ca ) { set ( x*ca+z*sa, y, -x*sa+z*ca ); }

    /*! Apply a rotation to GsVec around the Z axis with
        the angle 'a' where sa=sin(a) and ca=cos(a). */
    void rotz ( float sa, float ca ) { set ( x*ca-y*sa, x*sa+y*ca, z ); }

    /*! Same as the other rotx() method, but with one parameter
        angle in radians. */
    void rotx ( float radians );

    /*! Same as the other roty() method, but with one parameter
        angle in radians. */
    void roty ( float radians );

    /*! Same as the other rotz() method, but with one parameter
        angle in radians. */
    void rotz ( float radians );

    /*! Makes each coordinate to have its absolute value. */
    void abs () { x=GS_ABS(x); y=GS_ABS(y); z=GS_ABS(z); }

    /*! Normalizes the vector. If its norm is 0, the vector is not changed. */
    void normalize ();
    
    /*! Sets the length to be n and returns the previous length.
        If the norm is 0, the vector is not changed and 0 is returned. */
    float len ( float n );

    /*! Returns the length of the vector. Same as norm(). */
    float len () const { return norm(); }

    /*! Returns the norm (distance to zero) of the vector. If the vector is
        null, zero is returned. */
    float norm () const;

    /*! Returns the square of the norm (x*x + y*y + z*z). */
    float norm2 () const { return x*x + y*y + z*z; }
    
    /*! Returns the "max norm", ie, the maximum of the absolute values of the coordinates */
    float normax () const;

    /*! Returns true if all coordinates are zero; false otherwise. */
    bool isnull () const { return x==0.0 && y==0.0 && z==0.0; }

    /*! Returns true if the vector is inside the closed neighboorhood of point 0,0,0 and radius ds. */
    bool nextnull ( float ds ) const { return norm2()<=ds*ds; }

    /*! Allows member access like a vector. */
    float& operator[] ( int i ) { return e[i]; }

    /*! Parenthesis allows const member access. */
    const float& operator() ( int i ) const { return e[i]; }

    /*! Convertion to a float pointer. */
    operator const float* () const { return e; }

    /*! Copy operator from another GsVec. Implemented inline. */
    void operator = ( const GsVec& v ) { set(v); }

    /*! Copy operator from a 2d vector. Implemented inline. */
    void operator = ( const GsVec2& v ) { set(v); }

    /*! Copy operator from an int pointer. Implemented inline. */
    void operator = ( const int *v ) { set(v); }
    
    /*! Copy operator from a float pointer. Implemented inline. */
    void operator = ( const float *v ) { set(v); }
    
    /*! Copy operator from a double pointer. Implemented inline. */
    void operator = ( const double *v ) { set(v); }

    /*! Operator-=() with another GsVec. Implemented inline. */
    void operator -= ( const GsVec& v ) { x-=v.x; y-=v.y; z-=v.z; }

    /*! Operator+=() with another GsVec. Implemented inline. */
    void operator += ( const GsVec& v ) { x+=v.x; y+=v.y; z+=v.z; }

    /*! Operator*=() with a float. Implemented inline. */
    void operator *= ( float r ) { x*=r; y*=r; z*=r; }

    /*! Operator/=() with a float. Implemented inline. */
    void operator /= ( float r ) { x/=r; y/=r; z/=r; }

    /*! Member version of the cross product. 
        Attention: v1 or v2 cannot be the same as GsVec. */
    void cross ( const GsVec& v1, const GsVec& v2 );

    /*! Returns the cross product of v1 and v2. */
    friend GsVec cross ( const GsVec& v1, const GsVec& v2 );

    /*! Returns the dot product of v1 and v2 (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z).
        Its interpretation is the length of the projection of v1 onto v2
        ( v1.v2 = |v1||v2|cos(ang) ). */
    friend float dot ( const GsVec& v1, const GsVec& v2 )
           { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

    /*! Unary minus. Implemented inline. */
    friend GsVec operator - ( const GsVec& v )
           { return GsVec ( -v.x, -v.y, -v.z ); }

    /*! Adds two vectors. Implemented inline. */
    friend GsVec operator + ( const GsVec& v1, const GsVec& v2 ) 
           { return GsVec (v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); }

    /*! Subtracts two vectors. Implemented inline. */
    friend GsVec operator - ( const GsVec& v1, const GsVec& v2 ) 
           { return GsVec (v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); }

    /*! Comparison operator. Implemented inline. */
    friend bool operator == ( const GsVec& v1, const GsVec& v2 )
           { return v1.x==v2.x && v1.y==v2.y && v1.z==v2.z; }

    /*! Comparison operator. Implemented inline. */
    friend bool operator != ( const GsVec& v1, const GsVec& v2 )
           { return v1.x!=v2.x || v1.y!=v2.y || v1.z!=v2.z; }

    /*! Pos multiplication by scalar. Implemented inline. */
    friend GsVec operator * ( const GsVec& v, float r )
           { return GsVec (v.x*r, v.y*r, v.z*r); }

    /*! Pre multiplication by scalar. Implemented inline. */
    friend GsVec operator * ( float r, const GsVec& v )
           { return GsVec (v.x*r, v.y*r, v.z*r); }

    /*! Division by scalar. Implemented inline. */
    friend GsVec operator / ( const GsVec& v, float r )
           { return GsVec (v.x/r, v.y/r, v.z/r); }

    /*! Swaps the contents of v1 with v2. */
    friend void swap ( GsVec& v1, GsVec& v2 );

    /*! Returns true if dist(v1,v2)<=ds, otherwise returns false. Implemented inline. */
    friend bool next ( const GsVec& v1, const GsVec& v2, float ds )
           { return dist2(v1,v2)<=ds*ds; }

    /*! Returns the distance between v1 and v2 using norm_max(). */
    friend float distmax ( const GsVec& v1, const GsVec& v2 );

    /*! Returns the distance between v1 and v2. */
    friend float dist ( const GsVec& v1, const GsVec& v2 );

    /*! Returns the square of the distance between v1 and v2. */
    friend float dist2 ( const GsVec& v1, const GsVec& v2 );

    /*! Returns the angle between v1 and v2, that is inside the interval [0,pi],
        using acosf. The returned angle is in radians. */
    friend float angle ( const GsVec& v1, const GsVec& v2 ); 

    /*! Returns the angle between v1 and v2, but considering that
        v1 and v2 are already normalized, ie, returns acosf(dot(v1,v2)). */
    friend float anglenorm ( const GsVec& v1, const GsVec& v2 ); 

    /* Returns the normalized normal of triangle (a,b,c) */
    friend GsVec normal ( const GsPnt& a, const GsPnt& b, const GsPnt& c );

    /*! Returns -1,0,1 depending if v1 is less, equal or greater than v2, 
        using a comparison by coordinates, ie lexicographical. */
    static int compare ( const GsVec& v1, const GsVec& v2 );

    /*! Same as the other lexicographical compare method but with parameters as pointers. */
    inline static int compare ( const GsVec* v1, const GsVec* v2 ) { return gs_compare(*v1,*v2); }

    /*! Returns the linear interpolation of v1 v2 in parameter t ( v1(1-t)+v2(t) ). */
    friend GsVec mix ( const GsVec& v1, const GsVec& v2, float t )
           { return v1*(1.0f-t) + v2*t; }

    /*! Outputs in format: "x y z". */
    friend GsOutput& operator<< ( GsOutput& o, const GsVec& v )
           { return o << v.x <<' '<< v.y <<' '<< v.z; }

    /*! Inputs from format: "x y z". */
    friend GsInput& operator>> ( GsInput& in, GsVec& v )
           { return in >> v.x >> v.y >> v.z; }
 };

// g++ likes friends declared as well outside the class...:
void swap ( GsVec& v1, GsVec& v2 );
bool next ( const GsVec& v1, const GsVec& v2, float ds );
float distmax ( const GsVec& v1, const GsVec& v2 );
float dist ( const GsVec& v1, const GsVec& v2 );
float dist2 ( const GsVec& v1, const GsVec& v2 );
float angle ( const GsVec& v1, const GsVec& v2 ); 
float anglenorm ( const GsVec& v1, const GsVec& v2 ); 
GsVec normal ( const GsPnt& a, const GsPnt& b, const GsPnt& c );
GsVec barycentric ( const GsPnt& a, const GsPnt& b, const GsPnt& c, const GsVec &p );
void transbarycentric ( GsPnt& a, GsPnt& b, GsPnt& c, const GsVec &k, const GsVec& v );
int compare ( const GsVec& v1, const GsVec& v2 );
int compare ( const GsVec* v1, const GsVec* v2 );
GsVec mix ( const GsVec& v1, const GsVec& v2, float t );

//============================== end of file ===============================

# endif // GS_VEC_H
