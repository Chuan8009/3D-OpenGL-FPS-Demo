#version 450 core

in vec4 v_color;

layout (location = 0) out vec3 f_color;

uniform vec3 light_color;

void main() {
	f_color = vec3(1.0, 1.0, 0.0);
}