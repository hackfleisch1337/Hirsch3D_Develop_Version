/**
 * Hirsch3D Renderengine v.0.9.7
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/cubemap/cube.vert
 * @ref Hirsch3d/shader/cubemap
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

out vec3 v_texCoord;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    v_texCoord = position;
    gl_Position = u_proj * vec4(mat3(u_view) * position, 1.0);
}