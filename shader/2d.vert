/**
 * Hirsch3D Renderengine v.0.9.7
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/2d.vert
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

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 u_model;
out vec2 v_uv;

void main() {
    gl_Position = vec4(u_model * vec4(position, 1.0f));
    v_uv = uv;
}