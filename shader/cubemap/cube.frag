#version 330 core
layout(location = 0) out vec4 color;

uniform samplerCube u_cubemap;
in vec3 v_texCoord;
void main() {
    color = texture(u_cubemap, v_texCoord);
}