/*=======================================================================
   Copyright 2007 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_IMAGE_H
# define GS_IMAGE_H

# include <gsim/gs_color.h>

//=========================== GsImage ============================

/*! \class GsImage gs_image.h
    \brief Non compressed 32 bit depth rgba image
    GsImage stores pixel data as a sequence of rgba pixels. */
class GsImage
 { private :
    int _w, _h;     // total image size is 4*_w*_h bytes
    GsColor* _img;  // array of rgba values

   public :

    /*! Constructs an empty image */ 
    GsImage ();

    /*! Destructor */ 
   ~GsImage ();

    /*! Alloc the desired size in pixels of the image.
        A total of w*h pixels (ie w*h*4 bytes) are allocated.
        Invalid dimensions deletes the image data */
    void init ( int w, int h );

    /*! Returns the width in pixels of the image */
    int w () const { return _w; }

    /*! Returns the height in pixels of the image */
    int h () const { return _h; }
 
    /*! Returns a pointer to the base image data */
    GsColor* buffer () { return _img; }

    /*! Returns a reference to the color of pixel at position (l,c) */
    GsColor& operator() ( int l, int c ) { return pixel(l,c); }

    /*! Returns a reference to the color of pixel at position (l,c) */
    GsColor& pixel ( int l, int c ) { return _img[l*_w+c]; }

    /*! Returns a const reference to the color of pixel at position (l,c) */
    const GsColor& cpixel ( int l, int c ) const { return _img[l*_w+c]; }

    /*! Returns a pointer to the pixel color (4 bytes) at position (l,c) */
    GsColor* ptpixel ( int l, int c ) { return &_img[l*_w+c]; }

    /*! Returns a reference to the red component of the pixel at position (l,c) */
    gsbyte& r ( int l, int c ) { return pixel(l,c).r; }

    /*! Returns a reference to the green component of the pixel at position (l,c) */
    gsbyte& g ( int l, int c ) { return pixel(l,c).g; }

    /*! Returns a reference to the blue component of the pixel at position (l,c) */
    gsbyte& b ( int l, int c ) { return pixel(l,c).b; }
 
    /*! Returns a reference to the alpha component of the pixel at position (l,c) */
    gsbyte& a ( int l, int c ) { return pixel(l,c).a; }

    /*! Returns the base pointer of the line l of the image */
    GsColor* line ( int l ) { return &_img[l*_w]; }

    /*! Returns the base pointer of the image data */
    GsColor* data () { return &_img[0]; }

    /*! Saves the image in a bmp file. Returns true if could write file or false otherwise. */
    bool save ( const char* filename );

    /*! Load a bmp image. Returns true if could load or false otherwise; 24-bit RGB moe only. */
    bool load ( const char* filename );
 };

//============================= end of file ==========================

# endif // GS_IMAGE_H
