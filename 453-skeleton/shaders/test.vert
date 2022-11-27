#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;


out vec2 text_uv;
out vec3 text_normal;
out vec3 text_pose;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;


void main() {

	text_uv = vec2(uv.x, uv.y * -1.0f);
	text_pose = vec4(transform * vec4(pos,1.0f)).xyz;

	gl_Position = project * view * transform * vec4(pos, 1.0);
	//gl_Position = vec4(pos, 1.0);

	//text_normal = normal;
	 text_normal = mat3(transform) * normal;
}
