#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 in_normal;

out vec3 v_texCoord;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    v_texCoord = position;
    gl_Position = u_proj * vec4(mat3(u_view) * position, 1.0);
}