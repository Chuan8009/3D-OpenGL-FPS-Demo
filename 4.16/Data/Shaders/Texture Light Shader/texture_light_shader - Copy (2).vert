#version 450 core

const int MAX_LIGHTS = 20;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec3 normal;

out vec2 uv;
out vec3 world_position;
out vec3 camera_normal;
out vec3 camera_eye_direction;
out vec3 camera_light_direction[MAX_LIGHTS];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 light_position[MAX_LIGHTS];

uniform int num_lights;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	uv = v_uv;

	world_position = (model * vec4(position, 1.0)).xyz;
	vec3 camera_vertex_position = (view * model * vec4(position, 1.0)).xyz;

	camera_eye_direction = vec3(0, 0, 0) - camera_vertex_position;
	
	for(int i = 0; i < num_lights; ++i) {
	vec3 camera_light_position = (view * vec4(light_position[i], 1.0)).xyz;
	camera_light_direction[i] = camera_light_position + camera_eye_direction;
	}

	camera_normal = vec3(view * model * vec4(normal, 0.0)).xyz;
}