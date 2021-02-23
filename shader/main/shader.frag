#version 330 core

layout(location = 0) out vec4 color;


in vec2 v_uv;
in vec3 v_normal;
in vec3 v_position;
in mat4 v_model;

uniform vec4 u_color;

uniform sampler2D u_texture;
uniform int isSamplerSet;

uniform sampler2D u_normalmap;
uniform int isNormalSet;

uniform sampler2D u_roughnessmap;
uniform int isRoughnessSet;

uniform float u_ambient;

uniform float u_shininess;
uniform float u_specIntensity;
uniform float u_kD;
uniform vec3 u_specColor;

// Light

void main() {
    
    vec4 f_color = vec4(0.0);

    if(isSamplerSet != 1) {
        f_color = u_color;
    } else {
        vec4 tColor = texture(u_texture, v_uv);
        f_color = tColor;
    }
    
    // Scene ambient constant lightning
    vec3 ambient = vec3(f_color) * u_ambient;


    // Light Constants
    float brightness = 1.0;
    vec3 light = normalize(vec3(0,0,1));
    vec3 lightColor = vec3(1.0, 1.0, 1.0) * brightness;

    // Vectors

    vec3 view = normalize(-v_position);
    vec3 normal = normalize(v_normal);
    if(isNormalSet == 1) {
        vec3 uv_normal = vec3(texture(u_normalmap, v_uv));
        uv_normal.x = uv_normal.x * 2 - 1;
        uv_normal.y = uv_normal.y * 2 - 1;
        uv_normal.z = uv_normal.z * 2 - 1;
        normal =  normalize(mat3(v_model) * uv_normal);
    }
    vec3 reflection = reflect(-light, normal);

    
    // Material Constants
    float shininess = u_shininess;
    vec3 specColor;
    if(u_specColor == vec3(-1,-1,-1)) {
        specColor = lightColor * vec3(f_color);
    } else {
        specColor = lightColor * u_specColor;
    }
    
    float specIntensity = u_specIntensity;
    float Kd = u_kD;

    if(isRoughnessSet == 1) {
        shininess = 1 + shininess * max(log2(texture(u_roughnessmap, v_uv).x) + 1,0.0);
        specIntensity = max(specIntensity * max(log2(texture(u_roughnessmap, v_uv).x + 1),0.0), 0.1);
        specColor *= max(log2(texture(u_roughnessmap, v_uv).x) + 1, 0.0);
    }

    // Light 
    
    vec3 deffuse = max(dot(normal, light), 0.0) * vec3(f_color);
    vec3 specular = pow(max(dot(reflection, view), 0.0), shininess) * specColor * abs(deffuse);

    // Lightcolor
    deffuse.x *= lightColor.x;
    deffuse.y *= lightColor.y;
    deffuse.z *= lightColor.z;

    color = vec4(ambient + (deffuse * Kd) + (specular * specIntensity), 1.0);
    

    
    //else color = u_color;
}