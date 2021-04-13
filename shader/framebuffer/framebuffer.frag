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
uniform bool u_blur;
uniform float u_blurBrightness;

void main() {
    
    const float gamma = 1.0;
    
    vec4 tColor = texture2D(u_texture, texCoord);
    
    if(u_blur) {
        vec4 bColor = texture2D(u_brightTexture, texCoord);

        tColor += bColor*u_blurBrightness;
    }
    
    float exposure = 1.0;
    vec3 hdrColor = tColor.rgb;
  
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    
    gl_FragColor = vec4(mapped, 1.0);
}