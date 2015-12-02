/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_COLOR_H
# define GS_COLOR_H

/** \file gs_color.h 
 * A color definition
 */

# include <gsim/gs.h>
# include <gsim/gs_input.h>
# include <stdint.h>

/*! \class GsColor gs_color.h
    \brief specifies a color

     GsColor specifies a color using 8 bits for each basic color (red,green,blue)
     and more 8 bits for the alpha (the opacity). In this way, each component can
     have a value from 0 to 255 and the total class has a sizeof of 4 bytes.
     The default constructor initializes with values (r,g,b,a)=(127,127,127,255). */
class GsColor
 { public :
    static const GsColor black,   //!< black color (0,0,0)
                         red,     //!< red color (255,0,0)
                         green,   //!< green color (0,255,0)
                         yellow,  //!< yellow color (255,255,0)
                         blue,    //!< blue color (0,0,255)
                         magenta, //!< magenta color (255,0,255)
                         cyan,    //!< cyan color (0,255,255)
                         white,   //!< white color (255,255,255)
                         gray,    //!< gray color (127,127,127)
                         orange,  //!< orange color (255,170,0)
                         brown,   //!< brown color (127,50,0)
                         darkgray,  //!< dark gray color (64,64,64)
                         lightgray, //!< light gray color (225,225,225)
                         darkred, //!< dark red color (127,0,0)
                         darkgreen, //!< dark green color (0,127,0)
                         darkblue,  //!< dark blue color (0,0,127)
                         lightblue; //!< light blue color (0,127,255)
 
    union { struct { gsbyte r; //!< red component, 0<=r<=255, default is 127
                     gsbyte g; //!< green component, 0<=g<=255, default is 127
                     gsbyte b; //!< blue component, 0<=b<=255, default is 127
                     gsbyte a; //!< alpha component, 0<=a<=255, default is 255 == full opacity
                   };
            gsbyte e[4];
            gsuint32 intcode;
          };

   public :

    /*! Default constructor. Initializes with color gray. */
    GsColor () { *this=gray; }

    /*! Copy constructor. */
    GsColor ( const GsColor& c ) { intcode=c.intcode; }

    /*! Constructor setting all components. */
    GsColor ( gsbyte x, gsbyte y, gsbyte z, gsbyte w=255 ) : r(x),g(y),b(z),a(w) {}

    /*! Constructor setting all components. */
    GsColor ( int x, int y, int z, int w=255 ) : r(gsbyte(x)),g(gsbyte(y)),b(gsbyte(z)),a(gsbyte(w)) {}

    /*! Constructor setting all components with float types. */
    GsColor ( float x, float y, float z, float w=1.0f ) { set(x,y,z,w); }

    /*! Constructor from a 4 dimension float pointer. */
    GsColor ( const float v[4] ) { set(v); }

    /*! Sets the components of the color, the alpha value has a default parameter of 255. */
    void set ( gsbyte x, gsbyte y, gsbyte z, gsbyte w=255 ) { r=x; g=y; b=z; a=w; }

    /*! Sets the components of the color with integer values also betwenn 1 and 255,
        the alpha value has a default parameter of 255. */
    void set ( int x, int y, int z, int w=255 )
         { r=(gsbyte)x; g=(gsbyte)y; b=(gsbyte)z; a=(gsbyte)w; }

    /*! Sets the components of the color with float values, each one inside [0.0,1.0],
        the alpha value has a default parameter of 1.0. */
    void set ( float x, float y, float z, float w=1.0f );

    /*! Sets the components from and array of four floats. */
    void set ( const float v[4] ) { set(v[0],v[1],v[2],v[3]); }

    /*! Sets the color given a string containing one of the following names:
        black, red, green, yellow, blue, magenta, cyan, white, gray.
        Only the first 2 letters of the name are checked. Case-sensitive. */
    bool set ( const char* s );

    /*! Put the four components in f[], transforming each one to range [0.0,1.0] */
    float* get ( float f[4] ) const; 

    /*! Put the first 3 components in f[], transforming each one to range [0.0,1.0] */
    float* get3 ( float f[3] ) const; 

    /*! Put the four components in i[], each component varying from 0 to 255. */
    void get ( int i[4] ) const { i[0]=(int)r; i[1]=(int)g; i[2]=(int)b; i[3]=(int)a; }

    /*! Put the four components in b[], each component varying from 0 to 255. */
    void get ( gsbyte x[4] ) const { x[0]=r; x[1]=g; x[2]=b; x[3]=a; }

    /*! Convertion to a unsigned char pointer. */
    operator const gsbyte* () const { return e; }

    /*! Set random values in r g b components; to avoid sampling colors which are
        to similar to each other, 255/quant slots are created per r,g,b component */
    void setrandom ( int quant=5 );

    /*! Returns the HSV components of the color: 
        v in [0(black),1(white)], s in [0(v), 1(pure color)],
        h in [0,6), meaning 0:red, 1:yellow, 2:green, 3:cyan, 4:blue, 5:magenta, almost 6: almost red. */
    void hsv ( float& h, float& s, float& v ) const;

    /*! Returns a random color */
    static inline GsColor random ( int quant=5 ) { GsColor c; c.setrandom(quant); return c; }

    /*! Returns a color across the hue space from given parameter in [0,1] */
    static GsColor interphue ( float t );

    /*! Comparison equal operator. */
    friend bool operator == ( const GsColor &c1, const GsColor &c2 )
           { return c1.intcode==c2.intcode? true:false; }

    /*! Comparison difference operator. */
    friend bool operator != ( const GsColor &c1, const GsColor &c2 )
           { return c1.intcode!=c2.intcode? true:false; }

    /*! Interpolates two colors. */
    friend GsColor mix ( const GsColor &c1, const GsColor &c2, float t );

    /*! Weighted sum of three colors */
    friend GsColor mix ( const GsColor &c1, const GsColor &c2, const GsColor &c3, float w1, float w2, float w3 );

    /*! Euclidean distance in RGB space with each component in [0,255] range */
    friend float dist ( const GsColor &c1, const GsColor &c2 );

    /*! Outputs in format 'r g b a'. */
    friend GsOutput& operator<< ( GsOutput& o, const GsColor& c )
           { return o << int(c.r) <<' '<< int(c.g) <<' '<< int(c.b) <<' '<< int(c.a); }

    /*! Reads from format 'r g b a'. */
    friend GsInput& operator>> ( GsInput& in, GsColor& c )
          { return in >> c.r >> c.g >> c.b >> c.a; }
 };

//================================ End of File =================================================

# endif  // GS_COLOR_H

