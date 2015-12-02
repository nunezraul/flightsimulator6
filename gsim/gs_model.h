/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_MODEL_H
# define GS_MODEL_H

/** \file gs_model.h
 * 3d model with triangular faces
 */

class SnCylinder;
class SnSphere;
class GsImage;
class GsPolygon;

# include <gsim/gs.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_quat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_string.h>
# include <gsim/gs_strings.h>
# include <gsim/gs_material.h>

/*! \class GsModel gs_model.h
    \brief a model composed of triangular faces

    GsModel keeps arrays of vertices, normals, texture coordinates,
    materials, and face elements.
    These arrays are public and can be directly manipulated by the user.
    However please note that is the user responsability to maintain indices
    in a coherent way.
    Texture support requires OpenGL to be initialized at load time. */
class GsModel
 { public :
    /*! The Face structure keeps 3 indices used to associate the
        information in V, N, and T to each face */
    struct Face 
     { int a, b, c;
       Face () {}
       Face ( int i, int j, int k ) { a=i; b=j; c=k; }
       void set ( int i, int j, int k ) { a=i; b=j; c=k; }
       void validate () { if (a<0) a=-a; if (b<0) b=-b; if (c<0) c=-c; }
       friend GsOutput& operator<< ( GsOutput& o, const Face& f ) { return o<<f.a<<gspc<<f.b<<gspc<<f.c; }
       friend GsInput& operator>> ( GsInput& i, Face& f ) { return i>>f.a>>f.b>>f.c; }
     };

    GsArray<GsMaterial> M;  //!< List of materials
    GsArray<GsPnt>      V;  //!< List of vertex coordinates
    GsArray<GsPnt2>     T;  //!< List of texture coordinates
    GsArray<GsVec>      N;  //!< List of normals
    GsArray<Face>       F;  //!< Triangular faces indices to V
    GsArray<int>       Fm;  //!< Indices to the materials in M (size can be<F.size())
    GsArray<Face>      Fn;  //!< Indices to the normals in N (size can be<F.size())
    GsArray<Face>      Ft;  //!< Indices to the texture coords in T (size can be<F.size())

    /*! Will be set to true (the default) if back face culling
        should be applied, and false othrwise */
    bool culling;

    /*! Stores a name description for the materials in M.
        Will have size 0 if no names defined, otherwise will have size M.size() */
    GsStrings mtlnames;
    
    /*! May contain any desired name for the model. */
    GsString name;

    /*! Will contain the filename of the last file loaded or saved.
        This information is not saved in the model file itself, but 
        maintained as load and save methods are called. */
    GsString filename;

    /*! Structure to keep texture information */
     struct Texture
     { int glid;        //!< the OGL texture id, or: -1 if id not yet set, -2 if could not load texture
       char* fname;     //!< texture filename as read from model file
       char* fullfname; //!< full path of loaded/"tried to load" filename, or empty if not loaded
     };

    /*! Contain the list of textures being used */
    GsArray<Texture> textures;

   public :

    /*! Constructor lets all internal arrays as empty and culling is set to true */
    GsModel ();

    /*! Virtual Destructor */
    virtual ~GsModel ();

    /*! Returns true if the model has no faces, and false otherwise */
    bool empty () { return F.empty(); }

    /*! Sets to an empty model. Used memory is freed. */
    void init ();

    /*! Compress all internal array buffers. */
    void compress ();

    /*! Ensures that data arrays have correct sizes and set them to 0 if not. */
    void validate ();
    
    /*! Removes unreferenced or duplicated materials. */
    void remove_redundant_materials ();

    /*! Removes redundant normals, which are very close or equal to each other. */
    void remove_redundant_normals ( float prec=gstiny );

    /*! Check and remove redundant vertices */
    void merge_redundant_vertices ( float prec=gstiny );

    /*! Checks the extension to be "obj" or "3ds", calling the apropiate importer,
        or otherwise it will load a GsModel in .m (or old .srm) format.
        The given filename is stored and can be accessed later on
        with filename() */
    bool load ( const char* filename );

    /*! Reads a .m format (old .srm format also supported). 
        Method in.filename() is needed for shared materials to work. */
    bool load ( GsInput& in );

    /*! This method imports a model in .obj format. If the import
        is succesfull, true is returned, otherwise false is returned. */
    bool load_obj ( const char* file );

    /*! Returns 3F/2, which is the number of edges for "well connected" manifold meshes */
    int numedges () const { return 3*F.size()/2; }
   
    /*! Count and return the mean number of edges adjacent to a vertex in the model. */
    float count_mean_vertex_degree ();

    /*! Returns the number of common vertices between the two faces indices. */
    int common_vertices_of_faces ( int i, int j );

    /*! Clear the N and Fn arrays, with compression. */
    void flat ();

    /*! Clear materials and then set M and Fm so that all triangles use the
        same material m, with compression. */
    void set_one_material ( const GsMaterial& m );

    /*! Clear material names, and the M and Fm array, with compression. */
    void clear_materials ();

    /*! Clear the T and Ft array (with compression). */
    void clear_textures ();

    /*! Normals will be used if Fn is set, or if every vertex in V has exactly one normal in N.
        This method tests this and returns true or false. Implemented inline. */
    bool hasnormals () { return Fn.size()>0 || (V.size()>0 && V.size()==N.size()) ; }

    /*! Sequentially stores, for all faces, the 3 vertices of each face in the given array. */
    void get_face_vertices ( GsArray<GsVec>& fv ) const;

    /*! Sequentially stores, for all faces, the 3 normals for each vertex of each face in the given array. 
        If Fn.size()>0 its indices are use to retrive the normals per face from N, if Fn.size()==0,
        then, if N.size()==V.size() the normals in N are used, otherwise, flat normals are returned.
        The returned array will always contain F.size()*3 normals. */
    void get_face_normals ( GsArray<GsVec>& fn ) const;

    /*! Computes the normals of all faces and store them with the given number of repetitions in the given array. */
    void get_flat_normals ( GsArray<GsVec>& fn, int repspernormal=1 ) const;

    /*! Calculates and returns the normalized normal of the given face index. */
    GsVec face_normal ( int f ) const;

    /*! Calculates and returns the center point of face f. */
    GsVec face_center ( int f ) const;

    /*! Inverts faces orientations by swaping b and c indices, 
        and does the same to the normals. */
    void invert_faces ();

    /*! Generates normals smoothly, respecting the given crease
        angle in radians. Compression is called in the end.
        If the crease angle is <0, it is not considered and
        only an overall smooth is done. */
    void smooth ( float crease_angle=GS_TORAD(35.0f) );

    /*! Multiply all normals in N by -1 */
    void invert_normals ();

    /*! Translate the position of each vertex of the model.
        For primitive models, the primitive center is also translated.  */
    void translate ( const GsVec &tr );

    /*! Scale each vertex of the model. 
        For primitive models, each primitive radius is also scaled.  */
    void scale ( float factor );

    /*! Multiply array V and N by the given transformation matrix.
        N is transformed without translation and with renormalization.
        If parameter primtransf is true, only the rotation and translation
        in mat are aplied to the model and the primitive. If false,
        the current primitive information, if any, is lost. */
    void transform ( const GsMat& mat, bool primtransf=false );

    /*! Apply rotation in quaternion q to all vertices and normals.
        For primitive models, the primitive orientation is also rotate.  */
    void rotate ( const GsQuat& q );
 };

//================================ End of File =================================================

# endif // GS_MODEL_H
