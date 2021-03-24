/**
 * Hirsch3D Renderengine v.0.9.7
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

in vec2 texCoord;
uniform sampler2D u_texture;

uniform sampler2D u_brightTexture;

void main() {
    
    const float gamma = 2.2;
    
    vec4 tColor = texture2D(u_texture, texCoord);
    vec4 bColor = texture2D(u_brightTexture, texCoord);

    tColor += bColor*3;

    if(tColor.a < 0.1) discard;

    
    gl_FragColor = tColor;
}