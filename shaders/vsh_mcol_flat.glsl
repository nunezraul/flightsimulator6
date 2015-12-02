# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColor;
uniform mat4 vTransf;
uniform mat4 vProj;
flat out vec4 Color;

void main ()
 {
   Color = vColor / 255.0;
   gl_Position = vec4 ( vPos.x, vPos.y, vPos.z, 1.0 ) * vTransf * vProj;
 }
