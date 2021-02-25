#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


in vec3 vg_N[];
in vec3 vg_Pos[];
in vec2 vg_TexCoord[];

out vec3 gf_T;
out vec3 gf_B;
out vec3 gf_N;
out vec3 gf_Pos;
out vec2 gf_TexCoord;



void main(void)
{
  vec3 T = vec3(1.0);
  vec3 B = vec3(1.0);
  
  for(int i=0; i<3; ++i) {
    gf_T = T;
    gf_B = B;
    gf_TexCoord = vg_TexCoord[i];
    gf_N = vg_N[i];
    gf_Pos = vg_Pos[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }

}