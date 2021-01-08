#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;
in vec2 v_uv;
uniform sampler2D u_texture;
uniform int isSamplerSet;
void main() {
    //if(u_texture != -1)
    
    if(isSamplerSet != 1) {
        color = u_color;        
    } else {
        vec4 tColor = texture(u_texture, v_uv);
        
        
        color = tColor;
    }
    
    //else color = u_color;
}