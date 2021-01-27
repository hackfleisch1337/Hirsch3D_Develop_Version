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
    
    vec4 f_color = vec4(0.0f);

    if(isSamplerSet != 1) {
        f_color = u_color;
    } else {
        vec4 tColor = texture(u_texture, v_uv);
        f_color = tColor;
    }

    vec3 view = normalize(-v_position);
    vec3 light = normalize(vec3(1,1,1));
    vec3 normal = normalize(v_normal);
    
    vec3 reflection = reflect(-light, normal);

    vec3 ambient = vec3(f_color) * 0.1;
    vec3 deffuse = max(dot(normal, light), 0.0) * vec3(f_color);
    vec3 specular = pow(max(dot(reflection, view), 0.0), 4.0) * vec3(f_color);

        

    color = vec4(ambient + deffuse + specular, 1.0);


    
    //else color = u_color;
}