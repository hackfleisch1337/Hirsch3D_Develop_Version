/**
 * Hirsch3D Renderengine v.0.10.1
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/shader.frag
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
uniform sampler2D u_texture;

uniform vec4 u_color;

void main() {

    vec4 texColor = texture(u_texture, v_uv);
    vec4 uni_color = u_color;
    if(texColor == vec4(0.0f)) {
       texColor = vec4(1.0,1.0,1.0,1.0); 
    }
    if(u_color == vec4(0.0f)) {
        uni_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    color = texColor * uni_color;
}