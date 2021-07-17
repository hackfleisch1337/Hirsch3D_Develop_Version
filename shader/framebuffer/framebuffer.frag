/**
 * Hirsch3D Renderengine v.0.10.1
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

layout(location = 0) out vec4 out_color;

in vec2 texCoord;
uniform sampler2D u_texture;        // Last frame

// Screen Space Reflection
uniform int ssrenabled;
uniform sampler2D u_metallic;   // metallic
uniform sampler2D u_depth;      // viewspace position (not depth)
uniform sampler2D u_normal;     // worldspace normals
uniform sampler2D u_roughness;  // roughness

uniform vec3 camera_pos;

uniform mat4 invView;
uniform mat4 projection;
uniform mat4 invprojection;
uniform mat4 view;

vec4 getSSR();
vec3 PositionFromDepth(float depth);


vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
vec4 RayCast(vec3 dir, inout vec3 hitcoord, out float dDepth);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 hash(vec3 a);

const float rayStep = 0.1;
const float minRayStep = 0.1;
const float maxSteps = 30;
const float searchDist = 10;
const int numBinarySearchSteps = 5;
const float reflectionSpecularFalloffExponent = 3.0;


void main() {


    vec3 framecolor = texture(u_texture, texCoord).rgb;

    if(ssrenabled == 1) {
        vec4 ssrcolor = getSSR();
        framecolor = mix(vec3(framecolor), vec3(ssrcolor), ssrcolor.a);
    }
    
    
    out_color = vec4(framecolor, 1.0);
}

vec4 getSSR() {

    float Metallic = texture2D(u_metallic, texCoord).r;

    if(Metallic < 0.01) {
        return vec4(0,0,0,0.0);
    }

    vec3 viewNormal = vec3(texture2D(u_normal, texCoord) * invView);
    vec3 viewPos = texture2D(u_depth, texCoord).xyz;
    vec3 albedo = texture2D(u_texture, texCoord).rgb;
    vec3 worldpos = vec3(vec4(viewPos, 1.0) * invView);
    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, Metallic);
    vec3 Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);


    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));

    vec3 hit = viewPos;
    float dDepth;

    

    float roughness = texture2D(u_roughness, texCoord).r;
    roughness /= 200.0;

    vec3 jitt = mix(vec3(hash(worldpos)), vec3(0.0), roughness);
    //vec4 uv  =  RayCast(jitt + reflected* max(minRayStep, -viewPos.z), hit, dDepth) ;

    vec4 uv = RayCast((vec3(jitt) + reflected * max(minRayStep, -viewPos.z)), hit, dDepth);

    /*if (uv.x > 1.0 || uv.x < 0.0) {
        return vec4(0,0,0,0);
    }

    if (uv.y > 1.0 || uv.y < 0.0) {
        return vec4(0,0,0,0);
    }*/

    vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - uv.xy));
 
 
    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

    float ReflectionMultiplier = pow(Metallic, reflectionSpecularFalloffExponent) * 
                screenEdgefactor * 
                -reflected.z;

    vec4 res = texture(u_texture, uv.xy) * clamp(ReflectionMultiplier, 0.0, 0.9) * vec4(Fresnel,1.0);
    return res;
}



vec3 PositionFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(texCoord * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = invprojection * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
}

vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
{
    float depth;

    vec4 projectedCoord;
 
    for(int i = 0; i < numBinarySearchSteps; i++)
    {

        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
        depth = textureLod(u_depth, projectedCoord.xy, 2).z;

 
        dDepth = hitCoord.z - depth;

        dir *= 0.5;
        if(dDepth > 0.0)
            hitCoord += dir;
        else
            hitCoord -= dir;    
    }

        projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
 
    return vec3(projectedCoord.xy, depth);
}

vec4 RayCast(vec3 dir, inout vec3 hitcoord, out float dDepth) {
    dir *= rayStep;
    float depth = 0.0;
    int steps = 0;
    vec4 projectedCoord = vec4(0.0);

    for(int i = 0; i < maxSteps; ++i) {
        hitcoord += dir;
        projectedCoord = projection * vec4(hitcoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
        depth = texture(u_depth, projectedCoord.xy).z;
        if(depth > 1000.0) {
            continue;
        }

        dDepth = hitcoord.z - depth;
        if((dir.z - dDepth) < 1.2) {
            if(dDepth <= 0.0) {
                //vec4 Result = vec4(BinarySearch(dir, hitcoord, dDepth), 1.0);
                vec4 Result = vec4(BinarySearch(dir, hitcoord, dDepth), 1.0);
                return Result;
            }
        }
        steps++;
        
    }

    return vec4(projectedCoord.xy, depth, 0.0);
    //return vec4(0,0,0,0);

}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

#define Scale vec3(.8, .8, .8)
#define K 19.19
vec3 hash(vec3 a)
{
    a = fract(a * Scale);
    a += dot(a, a.yxz + K);
    return fract((a.xxy + a.yxx)*a.zyx);
}