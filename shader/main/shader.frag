/**
 * Hirsch3D Renderengine v.0.10.1
 * For more informations see README.md
 * or read the documentation in the doc folder
 * GitHub Repository: https://github.com/hackfleisch1337/Hirsch3D_Release_v1.0
 * Licence: MIT Licence
 * Last changes: 02.03.2021 (DD.MM.YYYY)
 * 
 * @file Hirsch3D/shader/main/shader.frag
 * @ref Hirsch3d/shader/main
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
layout(location = 1) out vec4 bright_color;


in vec2 v_uv;
in vec3 v_normal;
in vec3 v_position;
in mat4 v_model;
in vec3 v_vertex_pos;
in vec3 v_camera_pos;
in vec3 v_rawNormal;

in vec4 v_lightSpaceFragPos0;
in vec4 v_lightSpaceFragPos1;
in vec4 v_lightSpaceFragPos2;
in vec4 v_lightSpaceFragPos3;

uniform vec4 u_color;

uniform sampler2D u_texture;        // Texture 0
uniform int isSamplerSet;

uniform sampler2D u_normalmap;      // Texture 1
uniform int isNormalSet;

uniform sampler2D u_roughnessmap;   // Texture 2
uniform int isRoughnessSet;

uniform float u_ambient;

uniform float u_shininess;
uniform float u_specIntensity;
uniform float u_kD;
uniform vec3 u_specColor;
uniform vec3 u_emmisive;

uniform int u_transparency;

uniform samplerCube u_cubemap;      // Texture 3
uniform int u_isCubeMapSet;

uniform float u_reflection;
uniform float u_refractionIndex;

uniform float u_solidColor;

// TBN Matrix

#define MAX_LIGHTS 10

struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float brightness;
    int shadowMap;
};

struct PointLight {
    vec3 color;
    vec3 position;
    float brightness;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float brightness;
    float outerCone;
    float innerCone;
};

uniform int amountOfDlights;
uniform int amountOfPlights;
uniform int amountOfSlights;

uniform DirectionalLight dlights[MAX_LIGHTS];
uniform PointLight plights[MAX_LIGHTS];
uniform SpotLight slights[MAX_LIGHTS];


// Directional light shadowmaps
uniform sampler2D u_shadowMap0;
uniform sampler2D u_shadowMap1;
uniform sampler2D u_shadowMap2;
uniform sampler2D u_shadowMap3;


uniform int catchesShadow;

in vec3 gf_T;
in vec3 gf_B;


float CalculateShadowDirectionalLight(vec4 fragPosLightSpace, sampler2D shadow_map, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture2D(shadow_map, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(v_normal, lightDir)), 0.005);

    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main() {
    
    vec4 f_color = vec4(0.0);

    mat3 tbn = mat3(gf_T, gf_B, v_normal);

    if(isSamplerSet != 1) {
        f_color = u_color;
    } else {
        vec4 tColor = texture(u_texture, v_uv);
        f_color = tColor;
    }
    if(f_color.a < 0.1) {
        discard;
    }
    // Scene ambient constant lightning
    vec3 ambient = vec3(f_color);

    

    // Vectors

    vec3 view = normalize(-v_position);
    vec3 normal = normalize(v_normal);

    if(isNormalSet == 1) {
        vec3 uv_normal = texture(u_normalmap, v_uv).rgb;
        uv_normal = uv_normal * 2.0 - 1.0;
        normal =  normalize(tbn*uv_normal);
    }


    // Light Constants
    
    
    // Material Constants
    float shininess = u_shininess;
    vec3 specColor;

    float specIntensity = u_specIntensity;
    float Kd = u_kD;

    if(isRoughnessSet == 1) {
        shininess = 1 + shininess * max(log2(texture(u_roughnessmap, v_uv).x) + 1,0.0);
        specIntensity = max(specIntensity * max(log2(texture(u_roughnessmap, v_uv).x + 1),0.0), 0.1);
        specColor *= max(log2(texture(u_roughnessmap, v_uv).x) + 1, 0.0);
    }

    vec3 deffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);


    // Directional Light
    for(int i = 0; i < amountOfDlights; i++) {
        float brightness = dlights[i].brightness;
        vec3 light = normalize(dlights[i].direction);
        vec3 lightColor = dlights[i].color * brightness;

        
        
        if(u_specColor == vec3(-1,-1,-1)) {
            specColor = lightColor * vec3(f_color);
        } else {
            specColor = lightColor * u_specColor;
        }   

        float shadow = 0.0;
        if(catchesShadow == 1 && dlights[i].shadowMap != -1) {
            
            switch(dlights[i].shadowMap) {
                case 0: shadow = CalculateShadowDirectionalLight(v_lightSpaceFragPos0, u_shadowMap0, dlights[i].direction); break;
                case 1: shadow = CalculateShadowDirectionalLight(v_lightSpaceFragPos1, u_shadowMap1, dlights[i].direction); break;
                case 2: shadow = CalculateShadowDirectionalLight(v_lightSpaceFragPos2, u_shadowMap2, dlights[i].direction); break;
                case 3: shadow = CalculateShadowDirectionalLight(v_lightSpaceFragPos3, u_shadowMap3, dlights[i].direction); break;
            }
        }
            
            

        vec3 reflection = reflect(-light, normal);
        vec3 c_deffuse = max(dot(normal, light), 0.0) * vec3(f_color) * (1-shadow);
        vec3 c_specular = pow(max(dot(reflection, view), 0.0), shininess) * specColor * abs(c_deffuse);

        c_deffuse.x *= lightColor.x;
        c_deffuse.y *= lightColor.y;
        c_deffuse.z *= lightColor.z;

        ambient *= lightColor;
        deffuse += c_deffuse;
        specular += c_specular;
    }

    // Point Light
    for(int i = 0; i < amountOfPlights; i++) {
        float brightness = plights[i].brightness;
        vec3 lightColor = plights[i].color * brightness;
        if(u_specColor == vec3(-1,-1,-1)) {
            specColor = lightColor * vec3(f_color);
        } else {
            specColor = lightColor * u_specColor;
        }  
        float quadratic = plights[i].quadratic;
        float linear = plights[i].linear;

        vec3 light = normalize(plights[i].position - v_vertex_pos);
        vec3 reflection = reflect(-light, normal);

        float dist = length(plights[i].position - v_vertex_pos);
        float attentuation = 1.0 / ((1.0) + plights[i].linear*dist + plights[i].quadratic*dist*dist);
        
        vec3 c_deffuse = attentuation * plights[i].color * max(dot(normal, light), 0.0) * vec3(f_color);
        vec3 c_specular = attentuation * pow(max(dot(reflection, view), 0.0), shininess) * specColor * abs(c_deffuse);

        c_deffuse.x *= lightColor.x;
        c_deffuse.y *= lightColor.y;
        c_deffuse.z *= lightColor.z;

        ambient *= lightColor;
        deffuse += c_deffuse;
        specular += c_specular;
    }

    // Spotlight
    for(int i = 0; i < amountOfSlights; i++) {
        vec3 light = normalize(slights[i].position - v_vertex_pos);
        vec3 lightColor = slights[i].color * slights[i].brightness;
        if(u_specColor == vec3(-1,-1,-1)) {
            specColor = lightColor * vec3(f_color);
        } else {
            specColor = lightColor * u_specColor;
        }  
        vec3 reflection = reflect(-light, normal);
        float theta = dot(light, slights[i].direction);
        float epsilon = slights[i].innerCone - slights[i].outerCone;
        float intensity = clamp((theta - slights[i].outerCone) / epsilon, 0.0f, 1.0f);
        vec3 c_deffuse = vec3(0);
        vec3 c_specular = vec3(0);
        if(theta > slights[i].outerCone) {
            c_deffuse += intensity * max(dot(normal, light), 0.0) * vec3(f_color);
            c_specular += intensity * pow(max(dot(reflection, view), 0.0), shininess) * specColor * abs(deffuse);
        } else {
           ambient += u_ambient * slights[i].color * specColor;
        }
        c_deffuse.x *= slights[i].color.x;
        c_deffuse.y *= slights[i].color.y;
        c_deffuse.z *= slights[i].color.z;

        ambient *= lightColor;
        deffuse += c_deffuse;
        specular += c_specular;
    }

    // Lightcolor

    float transparentcy = 1.0;
    if(u_transparency == 1) {
        transparentcy = f_color.a;
    }

    

    vec4 out_color = vec4(u_ambient * ambient + (deffuse * Kd) + (specular * specIntensity), transparentcy);
    
    out_color = vec4(out_color.rgb + u_emmisive, out_color.a);

    if(u_isCubeMapSet == 1 && u_solidColor != 1) {

        vec4 reflectedColor = texture(u_cubemap, reflect(v_camera_pos, normalize(normal)));
        vec4 refractedColor = texture(u_cubemap, refract(v_camera_pos, normalize(normal), 1.0/u_refractionIndex));
        vec4 envColor = mix(reflectedColor, refractedColor, u_reflection);

        color = mix(envColor,out_color, u_solidColor);
    } else {
        color = out_color;
    }

    

    //else color = u_color;
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        bright_color = color;
    else
        bright_color = vec4(0.0, 0.0, 0.0, 1.0);
        
}