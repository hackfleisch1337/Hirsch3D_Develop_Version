/**
 * Hirsch3D Renderengine v.0.10.1
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/2d.frag
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

uniform vec4 u_color;
in vec2 v_uv;
uniform sampler2D u_texture;
uniform int isSamplerSet;

void main() {
    vec4 f_color = vec4(1);
    if(isSamplerSet != 1) {
        f_color = u_color;        
    } else {
        vec4 tColor = texture(u_texture, v_uv);
        
        
        f_color = tColor;
    }
    if(f_color.a < 0.1) {
        discard;
    } 
    color = f_color;
}