#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_position;
uniform sampler2D u_texture;
uniform int isSamplerSet;

uniform float u_ambient;

void main() {
    
    vec4 f_color = vec4(0.0f);

    if(isSamplerSet != 1) {
        f_color = u_color;
    } else {
        vec4 tColor = texture(u_texture, v_uv);
        f_color = tColor;
    }

    // Light Constants
    vec3 light = normalize(vec3(0,-0.5,1));
    vec3 lightColor = vec3(1,1,1);

    // Vectors
    vec3 view = normalize(-v_position);
    vec3 normal = normalize(v_normal);
    vec3 reflection = reflect(-light, normal);

    
    // Material Constants
    float shininess = 100.0;
    vec3 specColor = lightColor;
    float specIntensity = 80;
    float Kd = 1.0;

    // Light 
    vec3 ambient = vec3(f_color) * u_ambient;
    vec3 deffuse = max(dot(normal, light), 0.0) * vec3(f_color);
    vec3 specular = pow(max(dot(reflection, view), 0.0), shininess) * specColor * abs(deffuse);

    // Lightcolor
    deffuse.x *= lightColor.x;
    deffuse.y *= lightColor.y;
    deffuse.z *= lightColor.z;

    color = vec4(ambient + (deffuse * Kd) + (specular * specIntensity), 1.0);
    

    
    //else color = u_color;
}