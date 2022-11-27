#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;


out vec2 text_uv;


uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;


void main() {

	text_uv = vec2(uv.x, uv.y * -1.0f);

	gl_Position = project * view * transform * vec4(pos, 1.0);

}
