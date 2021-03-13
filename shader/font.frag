/**
 * Hirsch3D Renderengine v.0.9.7
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/font.frag
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
layout(location = 0) out vec4 color;
in vec2 v_uv;

uniform vec4 u_color;
uniform sampler2D u_texture;


void main() {
    if(texture2D(u_texture, v_uv).a < 0.1) discard;
    color = vec4(texture2D(u_texture, v_uv).a) * u_color;
}