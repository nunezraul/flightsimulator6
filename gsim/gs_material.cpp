/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/gs_material.h>
# include <gsim/gs_string.h>

//================================ GsMaterial =================================================

GsMaterial::GsMaterial () :
            ambient  (  51,  51,  51, 255 ),
            diffuse  ( 204, 204, 204, 255 ),
            specular (   0,   0,   0, 255 ),
            emission (   0,   0,   0, 255 )
 {
   shininess = 0;
   texid = -1;
 }

void GsMaterial::init () 
 { 
   ambient.set  (  51,  51,  51, 255 );
   diffuse.set  ( 204, 204, 204, 255 );
   specular.set (   0,   0,   0, 255 );
   emission.set (   0,   0,   0, 255 );
   shininess = 0;
   texid = -1;
 }

bool operator == ( const GsMaterial& m1, const GsMaterial& m2 )
 {
   return ( m1.ambient==m2.ambient &&
            m1.diffuse==m2.diffuse &&
            m1.specular==m2.specular &&
            m1.emission==m2.emission &&
            m1.shininess==m2.shininess &&
            m1.texid==m2.texid );
 }

bool operator != ( const GsMaterial& m1, const GsMaterial& m2 )
 {
   return m1==m2? false:true;
 }

GsOutput& operator<< ( GsOutput& o, const GsMaterial& m )
 {
   o << "amb " << m.ambient  << 
        " dif " << m.diffuse  << 
        " spe " << m.specular << 
        " emi " << m.emission << 
        " shi " << int(m.shininess);
   if ( m.texid>=0 ) o << " tid " << int(m.texid);
   return o;
 }

GsInput& operator>> ( GsInput& in, GsMaterial& m )
 {
   in.get();

   if ( in.ltype()!=GsInput::String ) // compact format, no texid
    { in.unget();
      in >> m.ambient >> m.diffuse  >>
            m.specular >> m.emission >>  m.shininess;
    }
   else
    { in >> m.ambient;
      in.get(); in >> m.diffuse;
      in.get(); in >> m.specular;
      in.get(); in >> m.emission;
      in.get(); in >> m.shininess;
      in.get();
      if ( in.ltoken()=="tid" )
       { m.texid = (gsint16)in.geti(); }
      else
       { in.unget(); m.texid=-1; }
    }

   return in;
 }

//================================ End of File =================================================
