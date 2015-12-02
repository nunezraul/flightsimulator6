/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

/** \file gs_material.h
 * specifies a material
 */

# ifndef GS_MATERIAL_H
# define GS_MATERIAL_H

# include <gsim/gs_color.h>
# include <gsim/gs_input.h>

/*! \class GsMaterial gs_material.h
    \brief specifies a material as defined by OpenGL

    Defines a material. */
class GsMaterial
 { public :
    GsColor ambient;   //!< default in float coords: 0.2, 0.2, 0.2, 1.0
    GsColor diffuse;   //!< default in float coords: 0.8, 0.8, 0.8, 1.0
    GsColor specular;  //!< default in float coords: 0.0, 0.0, 0.0, 1.0
    GsColor emission;  //!< default in float coords: 0.0, 0.0, 0.0, 1.0
    gsbyte  shininess; //!< default: 0, can be in : [0,128]
    gsint16 texid;     //!< a texture id or -1 if no texture

   public :
    
    /*! Initializes with the default values. */
    GsMaterial ();

    /*! Set again the default values. Note that .2 is mapped
        to 51, and .8 to 204 in the GsColor format. */
    void init ();

    /*! Exact comparison operator == */
    friend bool operator == ( const GsMaterial& m1, const GsMaterial& m2 );

    /*! Exact comparison operator != */
    friend bool operator != ( const GsMaterial& m1, const GsMaterial& m2 );

    /*! Outputs in format: "amb ar ag ab aa dif dr dg db da spe gs gs sb sa emi er eg eb ea shi s tid i" */
    friend GsOutput& operator<< ( GsOutput& o, const GsMaterial& m );

    /*! Inputs from the output format (the keywords (all) can be omitted) */
    friend GsInput& operator>> ( GsInput& in, GsMaterial& m );
 };

//================================ End of File =================================================

# endif // GS_MATERIAL_H
