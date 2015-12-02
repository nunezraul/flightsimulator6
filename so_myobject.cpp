
# include "so_myobject.h"

SoMyObject::SoMyObject()
 {
   _numelements=0;
 }

// init programs here, this will be done only once:
void SoMyObject::init ( const GlProgram& prog )
 {
   // Build program:
   _vsh.load_and_compile ( GL_VERTEX_SHADER, "../shaders/vsh_mcol_flat.glsl" );
   _fsh.load_and_compile ( GL_FRAGMENT_SHADER, "../shaders/fsh_flat.glsl" );
   _prog.init_and_link ( _vsh, _fsh );

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use at least 1 vertex array
   gen_buffers ( 2 );       // will use at least 1 buffer
   //_prog.uniform_locations ( .. ); // declare here uniforms
   //_prog.uniform_location ( 0, "vTransf" ); // each name must appear in the shader
   //_prog.uniform_location ( 1, "vProj" );
   //...
 }

// fill your arrays here, may be called each time the object has to change:
void SoMyObject::build ( float parameter1, float parameter2 )
 {
   // start by filling your data in arrays:
   // P.push().set ( .. );
   // ...

   // then send data to OpenGL buffers:
   glBindVertexArray ( va[0] ); 
   glEnableVertexAttribArray ( 0 ); // for each buffer there will be one vertex atribute
   glEnableVertexAttribArray ( 1 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 ); // tell what 

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   // etc

   glBindVertexArray(0); // break the existing vertex array object binding.

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numelements = P.size();

   // free non-needed memory:
   P.capacity(0);

 }

// call this everytome you need to draw:
void SoMyObject::draw ( GsMat& tr, GsMat& pr )
 {
   // Prepare program:
   glUseProgram ( _prog.id );
   // set unifrm values here
   // ...

   // Draw:
   glBindVertexArray ( va[0] );
   // call a draw function for the arrays here
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

