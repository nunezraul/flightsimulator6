/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <stdlib.h>
# include <iostream>

# include <gsim/gs_model.h>
# include <gsim/gs_tree.h>
# include <gsim/gs_quat.h>
# include <gsim/gs_strings.h>

//# define GS_USE_TRACE1 // IO
//# define GS_USE_TRACE2 // Validation of normals materials, etc
//# define GS_USE_TRACE3 // validate()
//# define GS_USE_TRACE4 // add_model()
# include <gsim/gs_trace.h>

//=================================== GsModel =================================================

GsModel::GsModel ()
 {
   culling = true;
 }

GsModel::~GsModel ()
 {
   init ();
 }

void GsModel::init ()
 {
   culling = true;

   M.capacity ( 0 );
   V.capacity ( 0 );
   N.capacity ( 0 );
   T.capacity ( 0 );
   F.capacity ( 0 );
   Fm.capacity ( 0 );
   Fn.capacity ( 0 );
   Ft.capacity ( 0 );
   mtlnames.capacity ( 0 );
   name = "";

   while ( textures.size() )
    { gs_string_delete ( textures.top().fname );
      gs_string_delete ( textures.top().fullfname );
      textures.pop();
    }
   textures.capacity(0);
 }

void GsModel::compress ()
 {
   M.compress();
   V.compress();
   N.compress();
   T.compress();
   F.compress();
   Fm.compress();
   Fn.compress();
   Ft.compress();
   mtlnames.compress();
   name.compress();
   textures.compress();
 }

void GsModel::validate ()
 {
   int i, j;
   GsArray<int> iarray;
   GsStrings sarray;

   int fsize = F.size();

   // check if the model is empty
   if ( fsize==0 || V.size()==0 )
    { init ();
      compress ();
      GS_TRACE3("No Faces!");
      return;
    }

   // check size of Fn
   if ( N.size()==0 )
    { Fn.size(0);
    }
   else if ( Fn.size()!=fsize )
    { Fn.size(0);
      N.size(0);
      GS_TRACE3("Normals mismatch!");
    }

   // check validity of T
   if ( T.size()>0 && T.size()<V.size() && Ft.empty() )
    { T.size(0);
      GS_TRACE3("Texture coords mismatch!");
    }

   // check texture indices in materials
   for ( i=0; i<M.size(); i++ )
    { if ( M[i].texid>=0 && M[i].texid>=textures.size() )
       { M[i].texid = -1;
         GS_TRACE3("Texture id corrected to -1 in material "<<i);
       }
    }

   // check size of Fm
   if ( M.size()==0 && Fm.size()>0 )
    { Fm.size(0);
      GS_TRACE3("Materials mismatch!");
    }
   else if ( Fm.size()!=fsize )
    { j = Fm.size();
      Fm.size ( fsize );
      for ( i=j; i<fsize; i++ ) Fm[i]=0;
      GS_TRACE3("Materials corrected!");
    }
   GS_TRACE3("Validate finished.");
 }

void GsModel::remove_redundant_materials ()
 {
   int i, j, k;
   GsArray<int> iarray;

   int fsize = F.size();

   if ( M.size()==0 )
    { Fm.size(0);
    }
   else 
    { j = Fm.size();
      Fm.size ( fsize );
      for ( i=j; i<fsize; i++ ) Fm[i]=0;

      // remove references to duplicate materials
      int msize = M.size();
      for ( i=0; i<msize; i++ ) 
       { for ( j=i+1; j<msize; j++ ) 
          { if ( M[i]==M[j] )
             { GS_TRACE2 ( "Detected material "<<i<<" equal to "<<j );
               for ( k=0; k<fsize; k++ ) // replace references to j by i
                 if ( Fm[k]==j ) Fm[k]=i;
             }
          }
       }

      // check for nonused materials
      iarray.size ( M.size() );
      iarray.setall ( -1 );

      for ( i=0; i<fsize; i++ ) 
       { if ( Fm[i]>=0 && Fm[i]<M.size() )
          iarray[Fm[i]] = 0; // mark used materials
         else Fm[i] = -1;
       }

      int toadd = 0;
      for ( i=0; i<iarray.size(); i++ )
       { if ( iarray[i]<0 )
          { GS_TRACE2 ( "Detected unused material "<<i );
            //sarray.set ( i, 0 ); 
            toadd++;
          }
         else
          iarray[i] = toadd;
       }

      for ( i=0; i<fsize; i++ ) // update indices
       { if ( Fm[i]<0 ) continue;
         Fm[i] -= iarray[Fm[i]];
       }
      for ( i=0,j=0; i<iarray.size(); i++ ) // compress materials
       { if ( iarray[i]<0 )
          { M.remove(j);
            mtlnames.remove(j);
          }
         else
          { j++; }
       }
    }
 }

void GsModel::remove_redundant_normals ( float prec )
 {
   int i, j, k;
   GsArray<int> iarray;

   prec = prec*prec;

   int fsize = F.size();
   int nsize = N.size();

   if ( nsize==0 || Fn.size()!=fsize )
    { N.size(0); 
      Fn.size(0);
    }
   else if ( nsize==1 )
    { // nothing to test, only 1 normal
    }
   else
    { // remove references to duplicated normals
      iarray.size ( nsize );
      iarray.setall ( -1 );
      for ( i=0; i<nsize; i++ ) 
       { for ( j=i+1; j<nsize; j++ ) 
          { if ( iarray[j]>=0 ) continue;
            if ( dist2(N[i],N[j])<prec )
             { GS_TRACE2 ( "Detected normal "<<i<<" close to "<<j );
               iarray[j]=i;
               for ( k=0; k<fsize; k++ ) // replace references to j by i
                { if ( Fn[k].a==j ) Fn[k].a=i;
                  if ( Fn[k].b==j ) Fn[k].b=i;
                  if ( Fn[k].c==j ) Fn[k].c=i;
                }
             }
          }
       }

      int toadd = 0;
      for ( i=0; i<iarray.size(); i++ ) 
       { if ( iarray[i]>=0 )
          { GS_TRACE2 ( "Detected unused normal "<<i );
            toadd++;
            iarray[i]=-1; // unused mark is now -1
          }
         else
          iarray[i] = toadd;
       }

      for ( i=0; i<fsize; i++ ) // update indices
       { Fn[i].a -= iarray[Fn[i].a];
         Fn[i].b -= iarray[Fn[i].b];
         Fn[i].c -= iarray[Fn[i].c];
       }

      for ( i=0,j=0; i<iarray.size(); i++ ) // compress N
       { if ( iarray[i]<0 )
          { N.remove(j); }
         else
          { j++; }
       }
    }
 }

void GsModel::merge_redundant_vertices ( float prec )
 {
   prec = prec*prec;
   
   int fsize = F.size();
   int vsize = V.size();
   int i, j;

   // build iarray marking replacements:
   GsArray<int> iarray;
   iarray.size ( vsize );
   for ( i=0; i<vsize; i++ ) iarray[i]=i;
   
   for ( i=0; i<vsize; i++ )
    for ( j=0; j<vsize; j++ )
     { if ( i==j ) break; // keep i < j
       if ( dist2(V[i],V[j])<prec ) // equal
        { iarray[j]=i;
        }
     }

   // fix face indices:
   for ( i=0; i<fsize; i++ )
    { F[i].a = iarray[ F[i].a ];
      F[i].b = iarray[ F[i].b ];
      F[i].c = iarray[ F[i].c ];
    }

   // compress indices:   
   int ind=0;
   bool newv;
   for ( i=0; i<vsize; i++ )
    { newv = iarray[i]==i;
      V[ind] = V[i];
      iarray[i] = ind;
      if ( newv ) ind++;
    }
   V.size ( ind );

   // fix face indices again:
   for ( i=0; i<fsize; i++ )
    { F[i].a = iarray[ F[i].a ];
      F[i].b = iarray[ F[i].b ];
      F[i].c = iarray[ F[i].c ];
    }
 }

bool GsModel::load ( const char* fname )
 {
   GsInput in;
   filename = fname;
   GsString file(fname);

   if ( has_extension(filename,"m") )
    { if ( !in.open(fname) ) return false;
      return load ( in );
    }
   if ( has_extension(filename,"obj") )
    { return load_obj(fname);
    }
   else
    { if ( !in.open(fname) ) return false;
      return load ( in );
    }
 }

bool GsModel::load( GsInput &in )
 {
   if ( !in.valid() ) return false;
   in.commentchar ( '#' ); // ensure proper comment style

   // check signature
   in.get();
   GsString s = in.ltoken();
   if ( s=="newgeo" ) { in.get(); in.get(); s=in.ltoken(); } // just skip this entry
   if ( s!="GsModel" && s!="SrModel" ) return false;

   // clear arrays and set culling to true
   int i;
   init ();
   GsString mtlpath;

   while ( true )
    { 
      in.get();
      if ( in.ltype()==GsInput::End ) break;
  
      // parse old srm format as well:
      if ( in.ltoken()=="<" )
       { in.get();
         if ( in.ltoken()=="/" ) { in.get(); in.get(); continue; }
         s=in.ltoken();
         in.get();
       }
      else
       { s = in.ltoken();
       }
      GS_TRACE1 ( "[" << s << "] keyword found..." );

      if ( s=="end" ) // stop reading data
       { break;
       }
      else if ( s=="culling" ) // read culling state
       { in >> i; 
         culling = i? true:false;
       }
      else if ( s=="name" ) // read name (is a GsInput::String type )
       { in.get();
         name = in.ltoken();
       }
      else if ( s=="vertices" ) // read vertices: x y z
       { in >> i; V.size(i);
          for ( i=0; i<V.size(); i++ ) // fscanf call is equiv to: in >> V[i];
           { if ( fscanf ( in.filept(), "%f %f %f", &V[i].x, &V[i].y, &V[i].z )!=3 ) return false; }
       }
      else if ( s=="faces" ) // read F: a b c
       { in >> i; F.size(i);
         for ( i=0; i<F.size(); i++ ) // fscanf call is equiv to: in >> F[i];
          { if ( fscanf ( in.filept(), "%d %d %d", &F[i].a, &F[i].b, &F[i].c )!=3 ) return false;
            F[i].validate(); 
          }
       }
      else if ( s=="fnormals" ) // read Fn: a b c
       { in >> i; Fn.size(i);
         for ( i=0; i<Fn.size(); i++ ) // fscanf call is equiv to: in >> Fn[i];
          { if ( fscanf ( in.filept(), "%d %d %d", &Fn[i].a, &Fn[i].b, &Fn[i].c )!=3 ) return false; }
       }
      else if ( s=="ftextcoords" ) // read Ft: a b c
       { in >> i; Ft.size(i);
         for ( i=0; i<Ft.size(); i++ ) // fscanf call is equiv to: in >> Ft[i];
          { if ( fscanf ( in.filept(), "%d %d %d", &Ft[i].a, &Ft[i].b, &Ft[i].c )!=3 ) return false; }
       }
      else if ( s=="fmaterials" ) // read Fm: i
       { in >> i; Fm.size(i);
         for ( i=0; i<Fm.size(); i++ ) // fscanf call is equiv to: in >> Fm[i];
          { if ( fscanf ( in.filept(), "%d", &Fm[i] )!=1 ) return false; }
       }
      else if ( s=="normals" ) // read N: x y z
       { in >> i; N.size(i);
         for ( i=0; i<N.size(); i++ ) // fscanf call is equiv to: in >> N[i];
          { if ( fscanf ( in.filept(), "%f %f %f", &N[i].x, &N[i].y, &N[i].z )!=3 ) return false; }
       }
      else if ( s=="textures" ) // read list of textures 
       { textures.size(in.geti());
         for ( i=0; i<textures.size(); i++ )
          { textures[i].glid = -1;
            textures[i].fname = gs_string_new ( in.gets() );
            textures[i].fullfname = 0;
          }
       }
      else if ( s=="textcoords" ) // read T: u v
       { in >> i; T.size(i);
         for ( i=0; i<T.size(); i++ ) // fscanf call is equiv to: in >> T[i];
          { if ( fscanf ( in.filept(), "%f %f", &T[i].x, &T[i].y )!=2 ) return false; }
       }
      else if ( s=="materials" ) // read M: mtls
       { in >> i; M.size(i);
         for ( i=0; i<M.size(); i++ ) in >> M[i];
       }
      else if ( s=="mtlnames" ) // read materials
       { GsString buf1, buf2;
         mtlnames.capacity ( 0 ); // clear all
         mtlnames.size ( M.size() ); // realloc
         while ( true )
          { if ( in.get()!=GsInput::Number ) { in.unget(); break; }
            i = in.ltoken().atoi();
            in.get();
            mtlnames.set ( i, in.ltoken() );
          }
         if ( mtlnames.size()>0 && in.filename() && has_extension(mtlnames[i],"mt") )
          { mtlpath=in.filename();
            extract_filename(mtlpath,s);
          }
       }
      else if ( s=="vertices_per_face" ) in.unget("faces");
      else if ( s=="normals_per_face" ) in.unget("fnormals");
      else if ( s=="materials_per_face" ) in.unget("fmaterials");
      else if ( s=="material_names" ) in.unget("mtlnames");
    }

   if ( mtlpath.len()>0 )
    { GsInput minp;
      for ( i=0; i<mtlnames.size(); i++ )
       { if ( i>=M.size() ) break; // protection
         s = mtlpath;
         s.append ( mtlnames[i] );
         if ( minp.open(s) ) minp >> M[i]; // GsInput will take care of closing the file
          else std::cout<<"Could not load shared material "<<s.pt()<<"!\n";
       }
    }

   if ( N.size()>0 && Fn.size()!=F.size() ) Fn=F;

   GS_TRACE1 ( "OK.\n" );

   return true;
 }

float GsModel::count_mean_vertex_degree ()
 {
   int i;
   if ( F.empty() ) return 0.0f;

   GsArray<int> vi(V.size());
   for ( i=0; i<vi.size(); i++ ) vi[i]=0;

   for ( i=0; i<F.size(); i++ )
    { vi[F[i].a]++; vi[F[i].b]++; vi[F[i].c]++; }

   double k=0;
   for ( i=0; i<vi.size(); i++ ) k += double(vi[i]);
   return float( k/double(vi.size()) );

   /* old way:
   GsTree<gsEdgeNode> t;
   
   for ( i=0; i<F.size(); i++ )
    { t.insert_or_del ( new gsEdgeNode ( F[i].a, F[i].b ) );
      t.insert_or_del ( new gsEdgeNode ( F[i].b, F[i].a ) );
      t.insert_or_del ( new gsEdgeNode ( F[i].b, F[i].c ) );
      t.insert_or_del ( new gsEdgeNode ( F[i].c, F[i].b ) );
      t.insert_or_del ( new gsEdgeNode ( F[i].c, F[i].a ) );
      t.insert_or_del ( new gsEdgeNode ( F[i].a, F[i].c ) );
    }

   return (float)t.size() / (float)V.size(); 
   */
 }


int GsModel::common_vertices_of_faces ( int i1, int i2 )
 {
   int i, j, c=0;
   int *f1 = &(F[i1].a);
   int *f2 = &(F[i2].a);
   for ( i=0; i<3; i++ )
    { for ( j=0; j<3; j++ )
       { if ( f1[i]==f2[j] ) c++; //gsout<<i<<","<<j<<gspc;
       }
    }
   return c;
 }

void GsModel::flat ()
 {
   N.size(0);
   Fn.size(0);
   compress ();
 }

void GsModel::set_one_material ( const GsMaterial& m )
 {
   int i;
   mtlnames.capacity(1);
   mtlnames.size(1);
   M.size(1);
   Fm.size ( F.size() );
   M[0] = m;
   for ( i=0; i<Fm.size(); i++ ) Fm[i]=0;
   compress ();
 }

void GsModel::clear_materials ()
 {
   mtlnames.capacity (0);
   M.size (0);
   Fm.size (0);
   compress ();
 }

void GsModel::clear_textures ()
 {
   T.size(0);
   T.compress();
 }

//v:current vtx, vi:vertices around v indicating the faces around v, vec:just a buffer
static void gen_normal ( int v, GsArray<GsVec>& vec, GsArray<int>& vi, GsModel *self, float crease_angle )
 {
   int i, j, tmp;
   float ang;

   vec.size(vi.size());

   //gsout<<"original:\n";
   //for ( i=0; i<vi.size(); i++ ) gsout<<self->F[vi[i]].a<<","<<self->F[vi[i]].b<<","<<self->F[vi[i]].c<<gsnl;

   // order faces around vertex (could use qsort in GsArray):
   for ( i=0; i<vi.size(); i++ )
    { for ( j=i+1; j<vi.size(); j++ )
       { if ( self->common_vertices_of_faces(vi[i],vi[j])==2 ) // share an edge
	      { GS_SWAP(vi[i+1],vi[j]);
            break;
	      }
       }
    }

   // gen normals for each face around v:
   for ( i=0; i<vi.size(); i++ ) 
    { vec[i]= self->face_normal ( vi[i] ); }

   // search for the first edge with a big angle and rearrange array, so
   // that the array starts with a "crease angled edge":
   bool angfound = false;
   for ( i=0; i<vec.size(); i++ )
    { ang = angle ( vec[i], vec[(i+1)%vec.size()]);
      if ( ang>crease_angle ) 
       { for ( j=0; j<=i; j++ ) { vec.push()=vec[j]; vi.push()=vi[j]; }
	     vec.remove ( 0, i+1 );
	     vi.remove ( 0, i+1 );
         angfound = true;
         break;
       }
    }
   if ( !angfound ) return; // no crease angles in this face cluster

   // Finally set the normals:
   GsVec n;
   float x=1.0f;
   int init=0;
   GsArray<GsVec>& N = self->N;
   for ( i=0; i<vec.size(); i++ )
    { ang = angle ( vec[i], vec[(i+1)%vec.size()]);

      if ( ang>crease_angle )
       { n = GsVec::null;
         x = 0.0f;
         for ( j=init; j<=i; j++ ) { n+=vec[j]; x=x+1.0f; }
         n /= x; // n is the mean normal of the previous set of smoothed faces around v

         for ( j=init; j<=i; j++ ) 
          { GsModel::Face &fn=self->Fn[vi[j]];
            /*if ( f.n<0 ) 
              { f.n=N.size(); N.insert(N.size(),3); N[f.n]=N[f.a]; N[f.n+1]=N[f.b]; N[f.n+2]=N[f.c]; }
            if ( v==f.a ) N[f.n]=n;
	         else if ( v==f.b ) N[f.n+1]=n;
	          else N[f.n+2]=n;
            */
            if ( v==fn.a ) fn.a = N.size();
	         else if ( v==fn.b ) fn.b = N.size();
	          else fn.c = N.size();
            N.push() = n;
          }

         init = i+1;
       }
    } 
 }

void GsModel::get_face_vertices ( GsArray<GsVec>& fv ) const
 { 
   fv.size ( F.size()*3 );
   int n=0;
   for ( int f=0; f<F.size(); f++ ) 
    { fv[n++] = V[F[f].a];
      fv[n++] = V[F[f].b];
      fv[n++] = V[F[f].c];
    }
 }

void GsModel::get_face_normals ( GsArray<GsVec>& fn ) const
 {
   int n=0;
   fn.size ( F.size()*3 );
   if ( Fn.size()>0 ) // mixed normals
    { for ( int f=0; f<F.size(); f++ ) 
       { fn[n++] = N[Fn[f].a];
         fn[n++] = N[Fn[f].b];
         fn[n++] = N[Fn[f].c];
       }
    }
   else if ( N.size()==V.size() ) // smooth normals
    { for ( int f=0; f<F.size(); f++ ) 
       { fn[n++] = N[F[f].a];
         fn[n++] = N[F[f].b];
         fn[n++] = N[F[f].c];
       }
    }
   else // flat normals
    { get_flat_normals(fn,3); }
 }

void GsModel::get_flat_normals ( GsArray<GsVec>& fn, int repspernormal ) const
 { 
   fn.size ( F.size()*repspernormal );
   GsVec normal;
   int r, n=0;
   for ( int f=0; f<F.size(); f++ ) 
   { normal = face_normal(f);
     for ( r=0; r<repspernormal; r++ ) 
      fn[n++] = normal;
   }
 }

GsVec GsModel::face_normal ( int f ) const
 { 
   GsVec n; 
   const Face& fac = F[f];
   n.cross ( V[fac.b]-V[fac.a], V[fac.c]-V[fac.a] ); 
   n.normalize(); 
   return n; 
 }

GsVec GsModel::face_center ( int f ) const
 { 
   GsVec c; 
   const Face& fac = F[f];
   return ( V[fac.a] + V[fac.b] + V[fac.c] ) / 3.0f;
 }

void GsModel::invert_faces ()
 {
   int i, tmp;
   for ( i=0; i<F.size(); i++ ) GS_SWAP ( F[i].b, F[i].c );
   for ( i=0; i<Fn.size(); i++ ) GS_SWAP ( Fn[i].b, Fn[i].c );
   for ( i=0; i<Ft.size(); i++ ) GS_SWAP ( Ft[i].b, Ft[i].c );
 }

void GsModel::invert_normals ()
 {
   int i;
   for ( i=0; i<N.size(); i++ ) N[i]*=-1.0;
 }

struct VertexNode : public GsTreeNode // only internally used
 { int v, i, f;
   VertexNode ( int a, int b, int c ) : v(a), i(b), f(c) {}
   VertexNode () { v=i=f=0; }
   VertexNode ( const VertexNode& x ) : GsTreeNode(), v(x.v), i(x.i), f(x.f) {}
  ~VertexNode () {}
   friend GsOutput& operator<< ( GsOutput& out, const VertexNode& /*v*/ ) { return out; };
   friend GsInput& operator>> ( GsInput& inp, VertexNode& /*v*/ ) { return inp; }
   static inline int compare ( const VertexNode* v1, const VertexNode* v2 )
    { return v1->v!=v2->v ? v1->v-v2->v   // vertices are different
                          : v1->i-v2->i;  // vertices are equal: use index i
    }
 };

static void insertv ( GsTree<VertexNode>& t, GsArray<int>& vi, int v, int f )
 {
   // array vi is only used to generated a suitable tree key sorting the vertices.
   VertexNode *n = new VertexNode(v,++vi[v],f);
   if ( !t.insert(n) ) std::cout<<"Wrong faces in GsModel::smooth ()!\n";
 }

// there is certainly a more efficient way to implement the function below...:
void GsModel::smooth ( float crease_angle )
 {
   int v, i;
   GsTree<VertexNode> t;
   GsArray<int> vi;
   GsArray<GsVec> vec; // this is just a buffer to be used in gen_normal()

   if ( !V.size() || !F.size() ) return;

   Fn.size ( F.size() );

   vi.size(V.size());
   for ( i=0; i<vi.size(); i++ ) vi[i]=0;

   for ( i=0; i<F.size(); i++ )
    { insertv ( t, vi, F[i].a, i );
      insertv ( t, vi, F[i].b, i );
      insertv ( t, vi, F[i].c, i );
      Fn[i].a = F[i].a;
      Fn[i].b = F[i].b;
      Fn[i].c = F[i].c;
    }

   // first pass will interpolate face normals around each vertex:
   N.size ( V.size() );
   vi.size(0);
   t.gofirst ();
   while ( t.cur()!=GsTreeNode::null )
    { v = t.cur()->v;
      vi.push() = t.cur()->f;
      t.gonext();
      if ( t.cur()==GsTreeNode::null || v!=t.cur()->v )
       { GsVec n = GsVec::null;
         for ( i=0; i<vi.size(); i++ ) n += face_normal ( vi[i] );
         N[v] = n / (float)vi.size();
         vi.size(0);
       }
    }

   if ( crease_angle<0 ) return; // only smooth everything

   // second pass will solve crease angles:
   vi.size(0);
   t.gofirst();
   while ( t.cur()!=GsTreeNode::null )
    { v = t.cur()->v;
      vi.push() = t.cur()->f;
      t.gonext();
      if ( t.cur()==GsTreeNode::null || v!=t.cur()->v )
       { gen_normal ( v, vec, vi, this, crease_angle );
         vi.size(0);
       }
    }
//std::cout<<"A\n";
   remove_redundant_normals ();
//std::cout<<"B\n";
   compress ();
//std::cout<<"C\n";
 }

void GsModel::translate ( const GsVec &tr )
 {
   int i, s=V.size();
   for ( i=0; i<s; i++ ) V[i]+=tr;
 }

void GsModel::scale ( float factor )
 {
   int i, s=V.size();
   for ( i=0; i<s; i++ ) V[i]*=factor;
 }

void GsModel::transform ( const GsMat& mat, bool primtransf )
 {
   int i, size;
   GsMat m = mat;

   if ( primtransf )
    { GsQuat q(mat);
      GsVec t(&mat[12]);
      rotate ( q );
      translate ( t );
      return;
    }

   size = V.size();
   for ( i=0; i<size; i++ ) V[i] = m * V[i];

   size = N.size();
   if ( size<=0 ) return;
   
   // ok, apply to N:
   m.setl4 ( 0, 0, 0, 1 ); // remove translation
   for ( i=0; i<size; i++ ) { N[i]= m*N[i]; N[i].normalize(); }
 }
 
void GsModel::rotate ( const GsQuat& q )
 {
   int i, size;
   size = V.size();
   for ( i=0; i<size; i++ ) V[i] = q.apply(V[i]);
   size = N.size();
   for ( i=0; i<size; i++ ) N[i] = q.apply(N[i]);
 }

//================================ End of File =================================================
