/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/gs_strings.h>
# include <gsim/gs_string.h>
# include <gsim/gs_model.h>

//# define GS_USE_TRACE1    // keyword tracking
//# define GS_USE_TRACE2    // materials
//# define GS_USE_TRACE3    // cur material
# include <gsim/gs_trace.h>

# define GETID(n,A) in>>n; if (n>0) n--; else if (n<0) n+=A.size()
 
static void get_face ( GsInput& in, GsModel& m, int& vc, int& vt, int& vn )
 {
   vc = vt = vn = -1;
   GETID(vc,m.V);

   if ( in.check()!=GsInput::Delimiter ) return; // was only: vc
   in.get(); // get /

   if ( in.check()==GsInput::Number ) // get vt from: vc/vt or vc/vt/vn
    { GETID(vt,m.T);
    }

   if ( in.get()==GsInput::Delimiter ) // get vn from: vc/vt/vn or vc//vn
    { GETID(vn,m.N);
    }
 }

static GsColor read_color ( GsInput& in )
 {
   float r, g, b;
   in >> r >> g >> b;
   GsColor c(r,g,b);
   return c;
 }

static void read_materials ( GsModel& model,
                             GsArray<GsMaterial>& M,
                             GsStrings& mnames,
                             const GsString& file,
                             const GsStrings& paths )
 {
   GsString s;
   GsInput in;

   in.lowercase(false);
   in.init ( fopen(file,"rt") );
   int i=0;
   while ( !in.valid() && i<paths.size() )
    { s = paths[i++];
      s << file;
      std::cout << "Material file: "<< s.pt() << std::endl;
      in.init ( fopen(s,"rt") );
    }
   if ( !in.valid() ) return; // could not get materials

   while ( !in.end() )
    { in.get();
      if ( in.ltoken()=="newmtl" )
       { M.push().init();
         GsString temp;
         in.readline(temp);
         temp.trim();
         GS_TRACE2 ( "new material: "<<temp );
         mnames.push ( temp );
       }
      else if ( in.ltoken()=="Ka" )
       { M.top().ambient = read_color ( in );
       }
      else if ( in.ltoken()=="Kd" )
       { M.top().diffuse = read_color ( in );
       }
      else if ( in.ltoken()=="Ks" )
       { M.top().specular = read_color ( in );
       }
      else if ( in.ltoken()=="Ke" ) // not sure if this one exists
       { M.top().emission = read_color ( in );
       }
      else if ( in.ltoken()=="Ns" )
       { in >> i;
         M.top().shininess = i;
       }
      else if ( in.ltoken()=="map_Kd" ) // texture
       {
         GsString texfile;
         in.readline(texfile);
         texfile.len(texfile.len()-1); // removing '/n'
         M.top().texid = model.textures.size();
         model.textures.push();
         model.textures.top().glid=-1;
         model.textures.top().fullfname=0;
         model.textures.top().fname = gs_string_new ( texfile );
       }
      else if ( in.ltoken()=="map_Bump" ) // bump maps not loaded
       { in.skipline();
       }
      else if ( in.ltoken()=="illum" ) // dont know what is this one
       { in >> i;
       }
    }
 }

static bool process_line ( const GsString& line,
                           GsModel& m,
                           GsStrings& paths, GsStrings& mnames,
                           int& curmtl,
                           GsArray<int>& va, GsArray<int>& ta, GsArray<int>& na )
 {
   GsInput in;
   in.init (line);
   in.get();

   GS_TRACE1 ( "Processing: " << in.ltoken() );

   if ( in.ltoken().len()==0 ) return true;

   if ( in.ltoken()=="v" ) // v x y z [w]
    { GS_TRACE1 ( "v" );
      m.V.push();
      in >> m.V.top();
    }
   else if ( in.ltoken()=="vn" ) // vn i j k
    { GS_TRACE1 ( "vn" );
      m.N.push();
      in >> m.N.top();
    }
   else if ( in.ltoken()=="vt" ) // vt u v [w]
    { GS_TRACE1 ( "vt" );
      m.T.push();
      in >> m.T.top();
    }
   else if ( in.ltoken()=="g" )
    { GS_TRACE1 ( "g" );
      in.get(); // group name
      if ( in.ltoken().len()==1 && in.ltoken()[0]=='v' )
       { in.unget(); }
      else
       { curmtl = mnames.lsearch ( in.ltoken() );
         if ( curmtl>=0 )
          { GS_TRACE3 ( "g curmtl = " << curmtl << " (" << in.ltoken() << ")" );
            in.get(); // :
            in.get(); // id?
          }
       }
    }
   else if ( in.ltoken()=="f" ) // f v/t/n v/t/n v/t/n (or v/t or v//n or v)
    { GS_TRACE1 ( "f" );
      int i=0;
      va.size(0); ta.size(0); na.size(0);
      while ( true )
       { if ( in.get()==GsInput::End ) break;
         in.unget();
         get_face ( in, m, va.push(), ta.push(), na.push() );
       }
      if ( va.size()<3 ) return false;

      for ( i=2; i<va.size(); i++ )
       { m.F.push().set ( va[0], va[i-1], va[i] );
         m.Fm.push() = curmtl;
         if ( ta[0]>=0 && ta[1]>=0 && ta[i]>=0 )
           m.Ft.push().set ( ta[0], ta[i-1], ta[i] );
         if ( na[0]>=0 && na[1]>=0 && na[i]>=0 )
           m.Fn.push().set ( na[0], na[i-1], na[i] );
       }
    }
   else if ( in.ltoken()=="s" ) // smoothing groups not supported
    { GS_TRACE1 ( "s" );
      in.get();
    }
   else if ( in.ltoken()=="o" ) // object name
    { GS_TRACE1 ( "o" );
      in.get();
      m.name = in.ltoken();
    }
   else if ( in.ltoken()=="usemap" ) // usemap name/off
    { GS_TRACE1 ( "usemap" );
    }
   else if ( in.ltoken()=="usemtl" ) // usemtl name
    { GS_TRACE1 ( "usemtl" );
      in.get();
      curmtl = mnames.lsearch ( in.ltoken() );
      GS_TRACE3 ( "u curmtl = " << curmtl << " (" << in.ltoken() << ")" );
    }
   else if ( in.ltoken()=="mtllib" ) // mtllib file1 file2 ...
    { GS_TRACE1 ( "mtllib" );
      GsString file, fname, path;
      if(in.check()==GsInput::String) {
          in.readline(file);
          file.len(file.len()-1); // removing '/n'
          extract_filename(file, fname);
          if(has_path(file)) {
              get_path(file, path);
              paths.push(path);
          }
          read_materials ( m, m.M, mnames, fname, paths ); 
      }
    }

   return true;
 }

bool GsModel::load_obj ( const char* file )
 {
   GsInput in ( fopen(file,"r") );
   if ( !in.valid() ) return false;

   in.commentchar ( '#' );
   in.lowercase ( false );

   GsString path=file;
   GsString filename;
   extract_filename(path,filename);
   GsStrings paths;
   paths.push ( path );
   GS_TRACE1 ( "First path:" << path );
   int curmtl = -1;

   init ();
   name = filename;
   remove_extension ( name );

   GsString line;
   GsArray<int> v(0,8), t(0,8), n(0,8); // buffers
   while ( in.readline(line)>=0 )
    { if ( !process_line(line,*this,paths,mtlnames,curmtl,v,t,n) ) return false;
    }

   validate();
   compress ();

   GS_TRACE1("Ok!");
   return true;
 }

//============================ EOF ===============================
