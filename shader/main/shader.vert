/**
 * Hirsch3D Renderengine v.0.9.7
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/main/shader.vert
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

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 in_normal;


uniform vec3 u_position;
uniform vec3 c_position;

uniform mat4 u_model;
uniform mat4 u_view;

uniform mat4 u_modelViewProj; // = proj * view * model
uniform mat4 u_modelView; // = view * model
uniform mat4 u_invModelView;

uniform mat4 u_lightspace;

/*
out vec2 v_uv;
out vec3 v_normal;
out vec3 v_position;
out mat4 v_model;
*/
out vec3 vg_N;
out vec3 vg_Pos;
out vec2 vg_TexCoord;
out mat4 vg_model;
out vec3 vg_Vertex_Pos;
out vec3 vg_cameraPos;
out vec3 vg_rawNormal;
out vec4 vg_LightSpaceFragPos;

void main() {

    gl_Position = u_modelViewProj * vec4(position, 1.0);

    vg_TexCoord = uv;
    vg_N = mat3(u_model) * in_normal;
    vg_Pos = vec3(u_modelView * vec4(position, 1.0));
    vg_model = u_model;
    vec3 fpos = vec3(u_model * vec4(position, 1.0));
    vg_Vertex_Pos = fpos;;
    vec4 worldpos = u_model * vec4(position, 1.0);
    vg_cameraPos = worldpos.xyz - c_position;//vec3(u_view * vec4(0.0,0.0,0.0,1.0));
    vg_rawNormal = in_normal;
    vg_LightSpaceFragPos = u_lightspace * vec4(fpos, 1.0);
}