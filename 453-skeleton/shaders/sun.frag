#version 330 core

in vec2 text_uv;

out vec4 color;


uniform sampler2D texture0;


void main() {
	color = texture(texture0, text_uv);
} 
