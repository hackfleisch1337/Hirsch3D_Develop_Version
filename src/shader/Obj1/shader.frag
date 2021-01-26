#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_position;
uniform sampler2D u_texture;
uniform int isSamplerSet;
void main() {
    //if(u_texture != -1)
    
    if(isSamplerSet != 1) {

        vec3 view = normalize(-v_position);
        vec3 light = normalize(vec3(1,1,1));
        vec3 normal = normalize(v_normal);
        
        vec3 reflection = reflect(-light, normal);

        vec3 ambient = vec3(u_color) * 0.1;
        vec3 deffuse = max(dot(normal, light), 0.0) * vec3(u_color);
        vec3 specular = pow(max(dot(reflection, view), 0.0), 4.0) * vec3(u_color);

        

        color = vec4(ambient + deffuse + specular, 1.0);



    } else {
        vec4 tColor = texture(u_texture, v_uv);
        
        
        color = tColor;
    }
    
    //else color = u_color;
}