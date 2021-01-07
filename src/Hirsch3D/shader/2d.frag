#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;
in vec2 v_uv;

void main() {
    color = u_color;
}