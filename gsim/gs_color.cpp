/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/
 
# include <gsim/gs_vec.h>
# include <gsim/gs_color.h>

//========================================= static =======================================

const GsColor GsColor::black     (0,0,0);
const GsColor GsColor::red       (255,0,0);
const GsColor GsColor::green     (0,255,0);
const GsColor GsColor::yellow    (255,255,0);
const GsColor GsColor::blue      (0,0,255);
const GsColor GsColor::magenta   (255,0,255);
const GsColor GsColor::cyan      (0,255,255);
const GsColor GsColor::white     (255,255,255);
const GsColor GsColor::gray      (127,127,127);
const GsColor GsColor::orange    (255,170,0);
const GsColor GsColor::brown     (127,50,0);
const GsColor GsColor::darkgray  (64,64,64);
const GsColor GsColor::lightgray (225,225,225);
const GsColor GsColor::darkred   (127,0,0);
const GsColor GsColor::darkgreen (0,127,0);
const GsColor GsColor::darkblue  (0,0,127);
const GsColor GsColor::lightblue (100,200,255);

//========================================= GsColor =======================================

void GsColor::set ( float x, float y, float z, float w )
 {
   r = (gsbyte) ( x*255.0f );
   g = (gsbyte) ( y*255.0f );
   b = (gsbyte) ( z*255.0f );
   a = (gsbyte) ( w*255.0f );
 }

bool GsColor::set ( const char* s )
 {
   if ( !s || !s[0] || !s[1] || !s[2] ) goto error;
   switch ( s[0] )
    { case 'b' : *this = s[2]=='a'?black : s[2]=='u'?blue : brown; return true;
      case 'r' : *this = red; return true;
      case 'g' : *this = s[2]=='e'? green:gray; return true;
      case 'y' : *this = yellow; return true;
      case 'm' : *this = magenta; return true;
      case 'c' : *this = cyan; return true;
      case 'w' : *this = white; return true;
      case 'o' : *this = orange; return true;
      case 'l' : *this = lightblue; return true;
      case 'd' : if ( !s[3] ) goto error;
                 switch ( s[4] )
                  { case 'r' : *this = darkred; return true;
                    case 'g' : *this = darkgreen; return true;
                    case 'b' : *this = darkblue; return true;
                  }
    }
   error:
   *this=blue; return false;
 }

// we dont do double versions to avoid automatic typecasts complications...
float* GsColor::get ( float f[4] ) const
 {
   f[0] = float(r) / 255.0f;
   f[1] = float(g) / 255.0f;
   f[2] = float(b) / 255.0f;
   f[3] = float(a) / 255.0f;
   return f;
 }

float* GsColor::get3 ( float f[3] ) const
 {
   f[0] = float(r) / 255.0f;
   f[1] = float(g) / 255.0f;
   f[2] = float(b) / 255.0f;
   return f;
 }

void GsColor::setrandom ( int quant )
 {
   int max = 255/quant;
   r = (gsbyte) gs_random(0,max)*quant;
   g = (gsbyte) gs_random(0,max)*quant;
   b = (gsbyte) gs_random(0,max)*quant;
 }

GsColor mix ( const GsColor &c1, const GsColor &c2, float t )
 {
   GsColor c;
   c.r = (gsbyte) (GS_MIX ( float(c1.r), float(c2.r), t ) + 0.5f);
   c.g = (gsbyte) (GS_MIX ( float(c1.g), float(c2.g), t ) + 0.5f);
   c.b = (gsbyte) (GS_MIX ( float(c1.b), float(c2.b), t ) + 0.5f);
   c.a = (gsbyte) (GS_MIX ( float(c1.a), float(c2.a), t ) + 0.5f);
   return c;
 }

GsColor mix ( const GsColor &c1, const GsColor &c2, const GsColor &c3, float w1, float w2, float w3 )
 {
   GsColor c;
   c.r = (gsbyte) ( float(c1.r)*w1 + float(c2.r)*w2 + float(c3.r)*w3 + 0.5f);
   c.g = (gsbyte) ( float(c1.g)*w1 + float(c2.g)*w2 + float(c3.g)*w3 + 0.5f);
   c.b = (gsbyte) ( float(c1.b)*w1 + float(c2.b)*w2 + float(c3.b)*w3 + 0.5f);
   c.a = (gsbyte) ( float(c1.a)*w1 + float(c2.a)*w2 + float(c3.a)*w3 + 0.5f);
   return c;
 }

float dist ( const GsColor &c1, const GsColor &c2 )
 { 
   return dist ( GsPnt(c1.r,c1.g,c1.b), GsPnt(c2.r,c2.g,c2.b) );
 }

// v: [0,1]=>[black,white]
// s: [0,1]=>[v, pure color]
// h: [0,1]=>[red,yellow], [1,2]=>[yellow,green], [2,3]=>[green,cyan], 
//    [3,4]=>[cyan,blue], [4,5]=>[blue,magenta], [5,6)=>[magenta,red]
void GsColor::hsv ( float& h, float& s, float& v ) const
 {
   float r = float(GsColor::r)/255.0f;
   float g = float(GsColor::g)/255.0f;
   float b = float(GsColor::b)/255.0f;

   float min = GS_MIN3(r,g,b);
   float max = GS_MAX3(r,g,b);

   v = max; // value in [0,1]
   if ( max==min ) { s=0; h=0; return; }

   float dif = float(max-min); // dif in [0,1]

   s = float(max-min)/float(max); // saturation in [0,1]

   if ( r==max ) // color between yellow and magenta
    { h = float(g-b)/dif; if(h<0) h+=6.0f; }
   else if ( g==max )
    { h = 2.0f + float(b-r)/dif; }
   else if ( b==max )
    { h = 4.0f + float(r-g)/dif; }
 } 

GsColor GsColor::interphue ( float t ) // static method
 {
   // First calculates RGB from HSV, saturation is set to 1.0
   struct HSV { float h, s, v; };
   struct COLOR { float r, g, b; };

   COLOR c2, sat;
   HSV c1;
   c1.h = (1-t)*240.0f; // re-mapping t to hue space of interest
   c1.s = 1.0f; c1.v = 1.0f;

   while ( c1.h<0.0f ) c1.h += 360.0f;
   while ( c1.h>360.0f ) c1.h -= 360.0f;

   if ( c1.h<120.0f )
    { sat.r = (120.0f-c1.h) / 60.0f;
      sat.g = c1.h / 60.0f;
      sat.b = 0.0f;
    }
   else if ( c1.h<240.0f )
    { sat.r = 0.0f;
      sat.g = (240.0f-c1.h) / 60.0f;
      sat.b = (c1.h-120.0f) / 60.0f;
    } 
   else
    { sat.r = (c1.h-240.0f) / 60.0f;
      sat.g = 0.0f;
      sat.b = (360.0f-c1.h) / 60.0f;
    }
   if ( sat.r>1 ) sat.r = 1;
   if ( sat.g>1 ) sat.g = 1;
   if ( sat.b>1 ) sat.b = 1;

   c2.r = ( 1 - c1.s + c1.s*sat.r ) * c1.v;
   c2.g = ( 1 - c1.s + c1.s*sat.g ) * c1.v;
   c2.b = ( 1 - c1.s + c1.s*sat.b ) * c1.v;

   return GsColor ( c2.r, c2.g, c2.b );
 } 

//=================================== End of File ==========================================
