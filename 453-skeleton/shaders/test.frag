#version 330 core

in vec2 text_uv;
in vec3 text_pose;
in vec3 text_normal;
 
out vec4 color;


uniform sampler2D texture0;
uniform vec3 light;

void main() {

		// Ambient
		vec3 ambientLight = vec3(0.1f, 0.1f,0.1f);

		// Diffuse
		vec3 VertexToLight = normalize(light - text_pose);
		vec3 diffuseColor = vec3(1.0f,1.0f,1.0f);
		float dotProduct = clamp(dot(VertexToLight, text_normal), 0.0f, 1.0f);
		vec3 diffuse = diffuseColor * dotProduct;

		color = texture(texture0, text_uv) * (vec4(ambientLight, 1.0f) + vec4(diffuse, 1.0f));

} 
