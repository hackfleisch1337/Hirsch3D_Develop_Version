#version 330 core

in vec2 texCoord;

uniform sampler2D frame;

void main() {

    gl_FragColor = texture(frame, texCoord.xy);

}