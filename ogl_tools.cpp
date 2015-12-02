/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include "ogl_tools.h"

# include <fstream>
# include <string>
# include <cerrno>

//================================== GlShader =========================================

GlShader::GlShader ()
 {
   id = 0;
 };

GlShader::~GlShader ()
 {
   if ( id>0 ) glDeleteShader ( id );
 };

bool GlShader::load_and_compile ( GLenum type, const char* filename )
 {
   if ( !load ( type, filename ) ) { std::cout<<"Could not load "<<filename<<"!\n"; return false; }
   if ( !compile() )  { std::cout<<"Could not compile "<<filename<<"!\n"; return false; }
   return true;
}

bool GlShader::load ( GLenum type, const char* filename )
 {
   std::ifstream in ( filename, std::ios::in | std::ios::binary );
   if ( !in.is_open() ) { std::cout<<"Could not load shader ["<<filename<<"] !\n"; gs_exit(1); }

   std::string s;
   in.seekg ( 0, std::ios::end );
   s.resize ( (size_t) in.tellg() );
   in.seekg ( 0, std::ios::beg );
   in.read ( &s[0], s.size() );
   set ( type, &s[0] );
   return true;
}

void GlShader::set ( GLenum type, const char* source )
 {
   if ( id>0 ) glDeleteShader ( id );
   id = glCreateShader ( type );
   glShaderSource ( id, 1, (const GLchar**)&source, NULL );
 }

bool GlShader::compile ()
 {
   GLint compiled;
   if ( !id ) return true;
   glCompileShader ( id );
   glGetShaderiv ( id, GL_COMPILE_STATUS, &compiled );
   if ( !compiled )
    { std::cout << "\nShader Compilation Error:\n";
      GLint size;
      glGetShaderiv ( id, GL_INFO_LOG_LENGTH, &size );
      std::string msg; msg.resize(size);
      glGetShaderInfoLog ( id, size, NULL, &msg[0] );
      std::cout << msg << gsnl;
      return false;
    }
   return true;
 }

//=================================== GlProgram ====================================

GlProgram::GlProgram ()
 {
   id = 0;
   uniloc = 0;
   nu = 0;
 };

GlProgram::~GlProgram ()
 {
   if ( id ) glDeleteProgram ( id );
   delete [] uniloc; 
   nu=0; uniloc=0;
 };

bool GlProgram::init_and_link ( const GlShader& sh1, const GlShader& sh2 )
 {
   if ( id ) glDeleteProgram ( id );
   id = glCreateProgram();
   attach ( sh1 );
   attach ( sh2 );
   if ( !link() ) return false;
   return true;
 }

bool GlProgram::link ()
 {
   glLinkProgram ( id );
   GLint linked;
   glGetProgramiv ( id, GL_LINK_STATUS, &linked );
   if ( !linked )
    { std::cout << "\nShader Linker Error:\n";
	  GLint size;
	  glGetProgramiv ( id, GL_INFO_LOG_LENGTH, &size );
      std::string msg; msg.resize(size);
	  glGetProgramInfoLog ( id, size, NULL, &msg[0] );
	  std::cout << msg << gsnl;
	  return false;
    }
   return true;
 }

void GlProgram::uniform_locations ( int n )
 {
   delete [] uniloc;
   uniloc = new GLint[n];
   nu = (gsbyte)n;
 }

void GlProgram::uniform_location ( int loc, const char* varname )
 {
   if ( !id ) std::cout << "Program undefined in uniform location call!\n";
   GLint location = glGetUniformLocation ( id, varname );
   if ( location<0 ) std::cout << "Uniform location for [" << varname << "] not found!\n";

   uniloc[loc] = location;
 }

//================================= GlObjects =========================================

void GlObjects::delete_objects () 
 {
   if ( va )
    { glDeleteVertexArrays ( (GLsizei)na, va );
      delete [] va;
      na=0; va=0;
    }

   if ( buf )
    { glDeleteBuffers ( (GLsizei)nb, buf );
      delete [] buf; 
      nb=0; buf=0;
    }
 }

//================================= utilities ====================================

void glClearColor ( const GsColor& c )
 {
   # ifdef GS_OPENGL
   glClearColor ( float(c.r)/255.0f,
                  float(c.g)/255.0f,
                  float(c.b)/255.0f,
                  float(c.a)/255.0f );
   # endif
 }

//=================================== info ====================================

bool glChkError ( const char* msg, bool printmsg )
 { 
   GLenum e = glGetError();
   if ( msg ) std::cout<<msg<<": ";
   if ( e==GL_NO_ERROR ) { if ( printmsg ) std::cout<<"GL_NO_ERROR.\n"; return false; }
   switch ( e )
    { case GL_INVALID_ENUM : std::cout<<"GL_INVALID_ENUM !\n"; break;
      case GL_INVALID_VALUE : std::cout<<"GL_INVALID_VALUE !\n"; break;
      case GL_INVALID_OPERATION : std::cout<<"GL_INVALID_OPERATION !\n"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION : std::cout<<"GL_INVALID_FRAMEBUFFER_OPERATION !\n"; break;
      case GL_OUT_OF_MEMORY : std::cout<<"GL_OUT_OF_MEMORY !\n"; break;
      case GL_STACK_UNDERFLOW : std::cout<<"GL_STACK_UNDERFLOW !\n"; break;
      case GL_STACK_OVERFLOW : std::cout<<"GL_STACK_OVERFLOW !\n"; break;
      default: std::cout<<"Undefined error "<<e<<"!\n"; break;
    }
   return true;
 }

void glPrintInfo ( GsOutput* out )
 {
   GsOutput& o = out? *out:std::cout;
   const char* end = "\"\n";
   o << "  Vendor: "   << "\"" << (char*) glGetString (GL_VENDOR) << end;
   o << "Renderer: " << "\"" << (char*) glGetString (GL_RENDERER) << end;
   o << "  OpenGL: "  <<  glVersion() << ", \""<< (const char*)glGetString(GL_VERSION) << end;
   o << "    GLSL: "  << glslVersion() << ", \""<< (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << end;
 }

static int glversion ( const GLubyte* s )
 {
   int v=0, m=100;
   for ( ; *s; s++ )
    { if ( *s>='0' && *s<='9' ) { v += m*(*s-'0'); if(m==1) break; m/=10; } }
   return v;
 }

static int WsiOglVersion = -1;
static int WsiGlslVersion = -1;

int glVersion ()
 { 
   if (WsiOglVersion<0) WsiOglVersion=glversion ( glGetString(GL_VERSION) );
   return WsiOglVersion;
 }

int glslVersion ()
 { 
   if (WsiGlslVersion<0) WsiGlslVersion=glversion ( glGetString(GL_SHADING_LANGUAGE_VERSION) );
   return WsiGlslVersion;
 }

//================================ End of File ========================================
