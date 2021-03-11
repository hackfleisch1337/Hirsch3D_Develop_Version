/**
 * Hirsch3D Renderengine v.0.9.7
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/font.vert
 * @ref Hirsch3d/shader
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

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

out vec2 v_uv;

uniform mat4 u_modelView;

void main() {
    gl_Position = u_modelView * vec4(vec2(a_position), 0.0f, 1.0f);
    v_uv = a_uv;
}