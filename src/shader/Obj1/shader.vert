#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 u_model;

void main() {
    gl_Position = vec4(position, 1.0f) * u_model;
}