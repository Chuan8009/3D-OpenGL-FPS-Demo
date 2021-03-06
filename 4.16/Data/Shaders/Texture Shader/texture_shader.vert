#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec3 normal;

out vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	uv = v_uv;
}