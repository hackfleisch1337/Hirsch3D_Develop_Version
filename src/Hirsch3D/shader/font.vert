#version 120

attribute vec2 a_position;
attribute vec2 a_tex_coord;

varying v_tex_coord;

uniform mat4 u_model;

void main() {
    gl_Position = u_model * vec4(a_position, 0.5f, 1.0f);
    v_tex_coord = a_tex_coord;
}