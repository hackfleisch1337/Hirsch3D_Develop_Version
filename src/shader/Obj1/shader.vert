#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 u_model; // = proj * view * model
uniform vec3 u_position;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_position;

void main() {

    /*
    vec3 absolutePosition = position + u_position;

    vec4 rotated = u_model * vec4(position, 1.0f);

    vec4 result = rotated + vec4(u_position, 1.0f);
    */


    gl_Position = u_model * vec4(position, 1.0f);

    /*
    vec4 rotation = vec4( u_model * vec4(position + u_position, 1.0f) );
    gl_Position = rotation;*/
    v_uv = uv;
    v_normal = in_normal;
    v_position = position;
}