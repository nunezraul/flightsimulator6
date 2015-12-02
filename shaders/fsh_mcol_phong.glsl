# version 400

uniform vec3 lPos;
uniform vec4 la;
uniform vec4 ld;
uniform vec4 ls;
uniform vec4 ka;
uniform vec4 ks;
uniform float sh;

in vec3 Pos;
in vec3 Norm;
in vec4 DifColor;
out vec4 fColor;

vec4 shade ( vec3 p )
 {
   vec3 n = Norm; // normalize ( mat3(vTransf)*vNorm ); // vertex normal
   vec3 l = normalize ( lPos-p );          // light direction
   vec3 r = reflect ( -l, n );                 // reflected ray
   vec3 v = vec3 ( 0, 0, 1.0 );                // view point

   vec4 kd = DifColor;
   vec4 amb = la*ka;
   vec4 dif = ld*kd*max(dot(l,n),0.0);
   vec4 spe = ls*ks*pow(max(r.z,0.0),sh);      // r.z==dot(v,r)

   if ( dot(l,n)<0 ) spe=vec4(0.0,0.0,0.0,1.0);

   return amb + dif + spe;
 }

void main() 
 { 
   fColor = shade ( Pos );
 } 
