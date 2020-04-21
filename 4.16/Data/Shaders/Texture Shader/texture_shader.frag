#version 450 core

in vec2 uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D texture_sampler;

void main() {
	vec4 tex_color = texture(texture_sampler, uv);
	if (tex_color.a < 0.1) {	
		discard;
	}
	f_color = tex_color;
}