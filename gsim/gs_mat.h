/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_MAT_H
# define GS_MAT_H

/** \file gs_mat.h 
 * A 4x4 matrix */

# include <gsim/gs_vec.h>

/*! \class GsMat gs_mat.h
    \brief A 4x4 matrix

    GsMat is a 4x4 matrix used to store geometric transformations.
    It is up to the user to decide if the matrix is used in
    column-major or line-major format. If line-major
    format is required, parameter fmt should be set to 'L' in
    the methods that need to know the format in use.
    If column-major representation is used, a vector v to be 
    transformed by matrix M should be a line vector at the left
    side of the matrix multiplication, ie vM. Otherwise it is
    a column vector at the right side (Mv) as in the usual 
    mathematical notation. In column-major format, the first
    three elements of the last line represent the translation
    vector (m[12],m[13],m[14]); in line-major format, the 
    translation vector is in the last column (m[3],m[7],m[11]). */
class GsMat
 { public :
    /*! A union is used so that the elements of the matrix can
        be accessed either with array e, or by floats e11, e12, etc */
    union { struct { float e11, e12, e13, e14,
                           e21, e22, e23, e24,
                           e31, e32, e33, e34,
                           e41, e42, e43, e44; };
            float e[16];
          };

    static const GsMat null;        //!< A null matrix
    static const GsMat id;          //!< An identity matrix
    enum InitializeMode { NoInit }; //!< Used by the "no init" constructor

   public :

    /*! Default constructor initializes GsMat as identity. */
    GsMat () { set(id.e); }

    /*! Copy constructor. */
    GsMat ( const GsMat& m ) { set(m.e); }

    /*! Constructor without initialization. GsMat values are simply not
        initialized when declaring GsMat m(GsMat::NoInit) */
    GsMat ( InitializeMode /*m*/ ) {}

    /*! Constructor from a float[16] pointer. */
    GsMat ( const float* p ) { set(p); }

    /*! Constructor from a double[16] pointer. */
    GsMat ( const double* p ) { set(p); }

    /*! Constructor from 16 values. */
    GsMat ( float a, float b, float c, float d, float e, float f, float g, float h, 
            float i, float j, float k, float l, float m, float n, float o, float p );

    /*! Copy operator. */
    void operator= ( const GsMat& m ) { set(m.e); }

    /*! float pointer type cast operator */
    operator float*() { return e; }

    /*! const float pointer type cast operator */
    operator const float*() const { return e; }

    /*! Returns a float pointer to the GsMat element with given index. */
    float* pt ( int i ) { return &e[i]; }

    /*! Returns a const float pointer to the GsMat element with given index. */
    const float* cpt ( int i ) const { return &e[i]; }

    /*! Access to an element of the matrix given an index from 0 to 15,
        ie, considering the matrix as a vector of 16 elements.
        No chekings are done on the index range. Implemented inline. */
    float& operator[] ( int i ) { return e[i]; }
    
    /*! operator() is the const version of operator[] */
    float operator() ( int i ) const { return e[i]; }

    /*! get() is the same as operator[], implemented inline */
    float& get ( int i ) { return e[i]; }

    /*! cget() is the same as operator(), implemented inline */
    float cget ( int i ) const { return e[i]; }

    /*! Set element i in [0,15], no checkings done on i; implemented inline */
    void set ( int i, float v ) { e[i]=v; }

    /*! Permits access to an element of the matrix using line and column
        indices that have to be in [0,3]. No indices chekings are done. */
    float& get ( int i, int j ) { return e[i*4+j]; }

    /*! const version of get(i,j), implemented inline */
    float cget ( int i, int j ) const { return e[i*4+j]; }

    /*! Sets all elements of GsMat from the given float[16] pointer. */
    void set ( const float *p );

    /*! Sets all elements of GsMat from the given double[16] pointer. */
    void set ( const double *p );

    /*! Sets the four elements of 1st line. Implemented inline. */
    void setl1 ( float x, float y, float z, float w ) { e[0]=x; e[1]=y; e[2]=z; e[3]=w; }
    
    /*! Sets the four elements of 2nd line. Implemented inline. */
    void setl2 ( float x, float y, float z, float w ) { e[4]=x; e[5]=y; e[6]=z; e[7]=w; }
    
    /*! Sets the four elements of 3rd line. Implemented inline. */
    void setl3 ( float x, float y, float z, float w ) { e[8]=x; e[9]=y; e[10]=z; e[11]=w; }
    
    /*! Sets the four elements of 4th line. Implemented inline. */
    void setl4 ( float x, float y, float z, float w ) { e[12]=x; e[13]=y; e[14]=z; e[15]=w; }

    /*! Sets the first three elements of 1st line. Implemented inline. */
    void setl1 ( const GsVec& v ) { e[0]=v.x; e[1]=v.y; e[2]=v.z; }
    
    /*! Sets the first three elements of 2nd line. Implemented inline. */
    void setl2 ( const GsVec& v ) { e[4]=v.x; e[5]=v.y; e[6]=v.z; }
    
    /*! Sets the first three elements of 3rd line. Implemented inline. */
    void setl3 ( const GsVec& v ) { e[8]=v.x; e[9]=v.y; e[10]=v.z; }
    
    /*! Sets the first three elements of 4th line. Implemented inline. */
    void setl4 ( const GsVec& v ) { e[12]=v.x; e[13]=v.y; e[14]=v.z; }

    /*! Sets the four elements of column 1. Implemented inline. */
    void setc1 ( float x, float y, float z, float w ) { e[0]=x; e[4]=y; e[8]=z; e[12]=w; }
    
    /*! Sets the four elements of column 2. Implemented inline. */
    void setc2 ( float x, float y, float z, float w ) { e[1]=x; e[5]=y; e[9]=z; e[13]=w; }
    
    /*! Sets the four elements of column 3. Implemented inline. */
    void setc3 ( float x, float y, float z, float w ) { e[2]=x; e[6]=y; e[10]=z; e[14]=w; }
    
    /*! Sets the four elements of column 4. Implemented inline. */
    void setc4 ( float x, float y, float z, float w ) { e[3]=x; e[7]=y; e[11]=z; e[15]=w; }

    /*! Sets the first three elements of column 1. Implemented inline. */
    void setc1 ( const GsVec& v ) { e[0]=v.x; e[1]=v.y; e[2]=v.z; }
    
    /*! Sets the first three elements of column 2. Implemented inline. */
    void setc2 ( const GsVec& v ) { e[4]=v.x; e[5]=v.y; e[6]=v.z; }
    
    /*! Sets the first three elements of column 3. Implemented inline. */
    void setc3 ( const GsVec& v ) { e[8]=v.x; e[9]=v.y; e[10]=v.z; }
    
    /*! Sets the first three elements of column 4. Implemented inline. */
    void setc4 ( const GsVec& v ) { e[12]=v.x; e[13]=v.y; e[14]=v.z; }

    /*! Returns true if all elements are equal to 0.0, false otherwise. */
    bool isnull () const { return *this==null; }

    /*! Returns true if the matrix is identical to GsMat::id, false otherwise. */
    bool isid () const { return *this==id;   }

    /*! Makes GsMatrix be a null matrix (all elements are zero). */
    void zero () { *this=null; }

    /*! Round elements according to given precision, for ex 0.01 for 2 decimal digits */
    void round ( float prec );

    /*! Makes GsMatrix be an identity matrix. */
    void identity () { *this=id; }

    /*! Transpose GsMatrix. */
    void transpose ();

    /*! Transpose the 3x3 sub matrix. */
    void transpose3x3 ();

    /*! Makes GsMat be a translation transformation of tx,ty,tz;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void translation ( float tx, float ty, float tz, char fmt='L' );

    /*! Make GsMat be a translation transformation of v;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void translation ( const GsVec& v, char fmt='L' ) { translation(v.x,v.y,v.z,fmt); }

    /*! Set the translation fields of the matrix to be tx, ty, tz;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void setrans ( float tx, float ty, float tz, char fmt='L' );

    /*! Set the translation fields of the matrix to be v.x, v.y, v.z;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void setrans ( const GsVec& v, char fmt='L' ) { setrans(v.x,v.y,v.z,fmt); }

    /*! Get the translation fields of the matrix and put in tx, ty, tz;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void getrans ( float& tx, float& ty, float& tz, char fmt='L' ) const;

    /*! Get the translation fields of the matrix and put in v.x, v.y, v.z;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void getrans ( GsVec& v, char fmt='L' ) const { getrans(v.x,v.y,v.z,fmt); }

    /*! Pre-multiplies GsMat with a translation matrix of translation vector v;
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void lcombtrans ( const GsVec& v, char fmt='L' );

    /*! Pos-multiplies GsMat with a translation matrix constructed with the
        vector v. Here the first three values of the 4th line are added by v. */
    void rcombtrans ( const GsVec& v, char fmt='L' );

    /*! Pos-multiplies GsMat with a scaling matrix of factors sx, sy and sz. */
    void rcombscale ( float sx, float sy, float sz );

    /*! Pos-multiplies GsMat with a scaling matrix of factors sx, sy and sz. */
    void lcombscale ( float sx, float sy, float sz );

    /*! Makes GsMat be a scaling transformation matrix. */
    void scale ( float sx, float sy, float sz );

    /*! Makes GsMat be a scaling transformation matrix. */
    void scale ( float s ) { scale(s,s,s); }

    /*! Makes GsMat be a rotation transformation around x axis. The given parameters 
        are the sine and cosine of the desired angle of rotation. Parameter
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void rotx ( float sa, float ca, char fmt='L' );

    /*! Makes GsMat be a rotation transformation around y axis. The given parameters 
        are the sine and cosine of the desired angle of rotation. Parameter
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void roty ( float sa, float ca, char fmt='L' );

    /*! Makes GsMat be a rotation transformation around z axis. The given parameters 
        are the sine and cosine of the desired angle of rotation. Parameter
        fmt can be 'C' or 'L' for column (default) or line major format. */
    void rotz ( float sa, float ca, char fmt='L' );

    /*! Makes a rotation transformation around x axis of given angle in radians.
        Mode can be 'C' or 'L' for column (default) or line major format. */
    void rotx ( float radians, char fmt='L' );

    /*! Makes a rotation transformation around y axis of given angle in radians.
        Mode can be 'C' or 'L' for column (default) or line major format. */
    void roty ( float radians, char fmt='L' );

    /*! Makes a rotation transformation around z axis of given angle in radians.
        Mode can be 'C' or 'L' for column (default) or line major format. */
    void rotz ( float radians, char fmt='L' );

    /*! Rotation around an axis of given angle, given by its sinus and cosinus.
        The given axis of rotation vec is not required to be normalized.
        Mode is 'C' for column-major (default) and 'L' for line-major format. */
    void rot ( const GsVec& vec, float sa, float ca, char fmt='L' );

    /*! Rotation around an axis given an angle in radians.
        The given axis of rotation vec is not required to be normalized.
        Mode is 'C' for column-major (default) and 'L' for line-major format. */
    void rot ( const GsVec& vec, float radians, char fmt='L' );

    /*! Gives the rotation matrix that rotates one vector to another.
        Vectors are not required to be normalized.
        Mode is 'C' for column-major (default) and 'L' for line-major format. */
    void rot ( const GsVec& from, const GsVec& to, char fmt='L' );

    /*! Set GsMat to be the rigid transformation matrix that maps the three
        given vertices into the plane xy. The transformation can then be applied
        to each vertex with vM. After transformed, v1 will go to the origin,
        and v2 will lye in the X axis. 
        Mode is 'C' for column-major (default) and 'L' for line-major format. */
    void projxy ( GsVec v1, GsVec v2, GsVec v3, char fmt='L' );

    /*! Set the matrix to be the OpenGL glut-like camera transformation.
        Result is in column-major format and fovy parameter is in radians.
        Note: znear and zfar must be positive. */
    void perspective ( float fovy, float aspect, float znear, float zfar );

    /*! Set the matrix to the OpenGL gluLookAt camera transformation.
        Result is in column-major format. */
    void lookat ( const GsVec& eye, const GsVec& center, const GsVec& up );

    /*! Set the transformation for mapping points to window coordinates with given width and height
        Result is in column-major format. */
    void orthowin ( float w, float h );

    /*! Set the matrix to the OpenGL glOrtho matrix.
        Result is in column-major format. */
    void ortho ( float left, float right, float bottom, float top, float near, float far );

    /*! Fast invertion by direct calculation, no loops, no gauss, no pivot searching, 
        but with more numerical errors. The result is returned in the 'inv' parameter. */
    void inverse ( GsMat& inv ) const;

    /*! Returns the inverse in a new matrix returned by value, callinf the inverse(GsMat&) method*/
    GsMat inverse () const  { GsMat inv(NoInit); inverse(inv); return inv; }

    /*! Makes GsMat to be its inverse, calling the inverse() method. */
    void invert () { *this=inverse(); }

    /*! Fast 4x4 determinant by direct calculation, no loops, no gauss. */
    float det () const;

    /*! Fast 3x3 determinant by direct calculation, no loops, no gauss. */
    float det3x3 () const;

    /*! Considers the matrix as a 16-dimensional vector and returns its norm raised
        to the power of two. */
    float norm2 () const;

    /*! Considers the matrix as a 16-dimensional vector and returns its norm. */
    float norm () const;

    /*! Set GsMat to be the result of the multiplication of m1 with m2.
        This method is safe if one of the given parameters is equal to 'this'. */
    void mult ( const GsMat& m1, const GsMat& m2 );

    /*! Sets GsMat to be the addition of m1 with m2. */
    void add ( const GsMat& m1, const GsMat& m2 );

    /*! Sets GsMat to be the difference m1-m2. */
    void sub ( const GsMat& m1, const GsMat& m2 );

    /*! Distance between two matrices, considering them as a 16-dimensional vector. */
    friend float dist ( const GsMat& a, const GsMat& b );

    /*! Distance between two matrices raised to two, considering them as a 16-dimensional vector. */
    friend float dist2 ( const GsMat& a, const GsMat& b );

    /*! Returns true if dist2(*this,m)<=ds*ds, and false otherwise. */
    friend bool next ( const GsMat& a, const GsMat& b, float ds ) { return dist2(a,b)<=ds*ds? true:false; }

    /*! Operator to multiply GsMat by a scalar. */
    void operator *= ( float r );

    /*! Operator to (right) multiply GsMat by another GsMat m. */
    void operator *= ( const GsMat& m );

    /*! Operator to add to GsMat another GsMat. */
    void operator += ( const GsMat& m );

    /*! Operator to multiply a GsMat to a scalar, returning another GsMat. */
    friend GsMat operator * ( const GsMat& m, float r );

    /*! Operator to multiply a scalar to a GsMat, returning another GsMat. */
    friend GsMat operator * ( float r, const GsMat& m );

    /*! Operator to multiply a GsMat to a GsVec, returning another GsMat. */
    friend GsVec operator * ( const GsMat& m,  const GsVec& v  );

    /*! Operator to multiply a GsVec to a GsMat, returning another GsMat. */
    friend GsVec operator * ( const GsVec& v,  const GsMat& m  );

    /*! Operator to multiply two GsMats, returning another GsMat. */
    friend GsMat operator * ( const GsMat& m1, const GsMat& m2 );

    /*! Operator to add two GsMats, returning another GsMat. */
    friend GsMat operator + ( const GsMat& m1, const GsMat& m2 );

    /*! Operator to compute the difference of two GsMats, returning another GsMat. */
    friend GsMat operator - ( const GsMat& m1, const GsMat& m2 );

    /*! Comparison operator to check if two GsMats are (exactly) equal. */
    friend bool operator == ( const GsMat& m1, const GsMat& m2 );

    /*! Comparison operator to check if two GsMat are different. */
    friend bool operator != ( const GsMat& m1, const GsMat& m2 );

    /*! Outputs 16 floats, 4 elements per line. */
    friend GsOutput& operator<< ( GsOutput& o, const GsMat& m );

    /*! Reads 16 float numbers from the input. */
    friend GsInput&  operator>> ( GsInput& in, GsMat& m );
  };

//============================== end of file ===============================

# endif // GS_MAT_H
