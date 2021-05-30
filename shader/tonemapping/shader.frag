#version 330 core

layout(location = 0) out vec4 outcolor;

in vec2 texCoord;
uniform sampler2D u_texture;

uniform sampler2D u_brightTexture;
uniform bool u_blur;
uniform float u_blurBrightness;
uniform float u_gamma;
uniform float u_exposure;


void main() {

    float gamma = u_gamma;
    
    vec4 tColor = texture2D(u_texture, texCoord);
    
    if(u_blur) {
        vec4 bColor = texture2D(u_brightTexture, texCoord);

        tColor += bColor*u_blurBrightness;
    }
    
    float exposure = u_exposure;
    vec3 hdrColor = tColor.rgb;
  
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    outcolor = vec4(mapped, 1.0);
}