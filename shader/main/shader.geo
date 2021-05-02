/**
 * Hirsch3D Renderengine v.0.10.1
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/main/shader.geo
 * @ref Hirsch3d/shader/main
 *
 * @author Emanuel Zache
 * @version 0.9.7
 * @copyright (C) 2021 Emanuel Zache
 *
 *
 * 
 * 
 * 
 * 
 *
 * 
 *
 */

#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


in vec3 vg_N[];
in vec3 vg_Pos[];
in vec2 vg_TexCoord[];
in mat4 vg_model[];
in vec3 vg_Vertex_Pos[];
in vec3 vg_cameraPos[];
in vec3 vg_rawNormal[];
in vec4 vg_LightSpaceFragPos0[];
in vec4 vg_LightSpaceFragPos1[];
in vec4 vg_LightSpaceFragPos2[];
in vec4 vg_LightSpaceFragPos3[];

out vec3 gf_T;
out vec3 gf_B;
out vec3 v_normal;
out vec3 v_position;
out vec2 v_uv;
out mat4 v_model;
out vec3 v_vertex_pos;
out vec3 v_camera_pos;
out vec3 v_rawNormal;

out vec4 v_lightSpaceFragPos0;
out vec4 v_lightSpaceFragPos1;
out vec4 v_lightSpaceFragPos2;
out vec4 v_lightSpaceFragPos3;

vec3 GetTangent(vec3 A, vec3 B, vec3 C,  vec2 Auv, vec2 Buv, vec2 Cuv)
{
  float Bv_Cv = Buv.y - Cuv.y;
  if(Bv_Cv == 0.0)
    return (B-C)/(Buv.x-Cuv.x);
  
  float Quotient = (Auv.y - Cuv.y)/(Bv_Cv);
  vec3 D   = C   + (B  -C)   * Quotient;
  vec2 Duv = Cuv + (Buv-Cuv) * Quotient;
  return (D-A)/(Duv.x - Auv.x);
}
vec3 GetBitangent(vec3 A, vec3 B, vec3 C,  vec2 Auv, vec2 Buv, vec2 Cuv)
{
  return GetTangent(A, C, B,  Auv.yx, Cuv.yx, Buv.yx);
}

void main(void)
{
    vec3 T = GetTangent(vg_Pos[0], vg_Pos[1], vg_Pos[2],
                      vg_TexCoord[0], vg_TexCoord[1], vg_TexCoord[2]);
    vec3 B = GetBitangent(vg_Pos[0], vg_Pos[1], vg_Pos[2],
                        vg_TexCoord[0], vg_TexCoord[1], vg_TexCoord[2]);

    for(int i=0; i<3; ++i) {
        gf_T = T;
        gf_B = B;
        v_uv = vg_TexCoord[i];
        v_normal = vg_N[i];
        v_position = vg_Pos[i];
        v_model = vg_model[i];
        v_vertex_pos = vg_Vertex_Pos[i];
        v_lightSpaceFragPos0 = vg_LightSpaceFragPos0[i];
        v_lightSpaceFragPos1 = vg_LightSpaceFragPos1[i];
        v_lightSpaceFragPos2 = vg_LightSpaceFragPos2[i];
        v_lightSpaceFragPos3 = vg_LightSpaceFragPos3[i];
        v_camera_pos = vg_cameraPos[i];
        v_rawNormal = vg_rawNormal[i];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

}