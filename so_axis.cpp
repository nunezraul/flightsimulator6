
# include "so_axis.h"

SoAxis::SoAxis()
 {
   _numpoints = 0;
 }

// init is called only once:
void SoAxis::init ()
 {
   // Build program:
   _vsh.load_and_compile ( GL_VERTEX_SHADER, "../shaders/vsh_mcol_flat.glsl" );
   _fsh.load_and_compile ( GL_FRAGMENT_SHADER, "../shaders/fsh_flat.glsl" );
   _prog.init_and_link ( _vsh, _fsh );

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   _prog.uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   _prog.uniform_location ( 0, "vTransf" );
   _prog.uniform_location ( 1, "vProj" );
 }

// build may be called everytime the object changes (not the case for this axis object):
void SoAxis::build ( float r )
 {
   int i;
   const float d=r/20.0f;
   
   P.size(0); C.size(0); // set size to zero
   P.reserve(18); C.reserve(18); // reserve some space to avoid re-allocations below

   P.push().set ( -r, 0, 0 ); P.push().set ( r, 0, 0 );
   P.push().set ( r-d, -d, 0 ); P.push().set ( r, 0, 0 );
   P.push().set ( r-d, d, 0 ); P.push().set ( r, 0, 0 );
   for ( i=0; i<6; i++ ) C.push() = GsColor::red; // recall do a GsColor has r,g,b,a values

   P.push().set ( 0, -r, 0 ); P.push().set ( 0, r, 0 );
   P.push().set ( 0, r-d, -d ); P.push().set ( 0, r, 0 );
   P.push().set ( 0, r-d, d ); P.push().set ( 0, r, 0 );
   for ( i=0; i<6; i++ ) C.push() = GsColor::green;

   glColor3f ( 0, 0, 1 );		
   P.push().set ( 0, 0, -r ); P.push().set ( 0, 0, r );
   P.push().set ( 0, -d, r-d ); P.push().set ( 0, 0, r );
   P.push().set ( 0, d, r-d ); P.push().set ( 0, 0, r );
   for ( i=0; i<6; i++ ) C.push() = GsColor::blue;

   // send data to OpenGL buffers:
   glBindVertexArray ( va[0] );
   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, 3*sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   glBufferData ( GL_ARRAY_BUFFER, 4*sizeof(gsbyte)*C.size(), C.pt(), GL_STATIC_DRAW );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glBindVertexArray(0); // break the existing vertex array object binding.

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.capacity(0); C.capacity(0);
 }

// draw will be called everytime we need to display this object:
void SoAxis::draw ( GsMat& tr, GsMat& pr )
 {
   // Prepare program:
   glUseProgram ( _prog.id );
   glUniformMatrix4fv ( _prog.uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( _prog.uniloc[1], 1, GL_FALSE, pr.e );

   // Draw:
   glBindVertexArray ( va[0] );
   glDrawArrays ( GL_LINES, 0, _numpoints );
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

