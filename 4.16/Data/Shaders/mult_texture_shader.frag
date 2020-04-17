#version 450 core

in vec2 uv;

layout (location = 0) out vec3 f_color;

uniform sampler2D texture_sampler;

void main() {
	f_color = texture(texture_sampler, uv).rgb;
}