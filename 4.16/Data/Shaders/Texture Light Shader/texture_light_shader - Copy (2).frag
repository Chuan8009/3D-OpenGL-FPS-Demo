#version 450 core

const int MAX_LIGHTS = 20;

in vec2 uv;
in vec3 world_position;
in vec3 camera_normal;
in vec3 camera_eye_direction;
in vec3 camera_light_direction[MAX_LIGHTS];

layout (location = 0) out vec4 f_color;

uniform sampler2D texture_sampler;

uniform vec3 light_color[MAX_LIGHTS];
uniform vec3 light_position[MAX_LIGHTS];
uniform float light_intensity[MAX_LIGHTS];

uniform int num_lights;

vec3 calc_point_light(vec3 position, vec3 color, float intensity, int light_num) {
	vec3 material_diffuse_color = texture(texture_sampler, uv).rgb;
	vec3 material_ambient_color = vec3(0.1, 0.1, 0.1) * material_diffuse_color;
	vec3 material_specular_color = vec3(0.05, 0.05, 0.05);

	float distance = length(position - world_position);

	vec3 n = normalize(camera_normal);
	vec3 l = normalize(camera_light_direction[light_num]);

	float cos_theta = clamp(dot(n, l), 0, 1);

	vec3 e = normalize(camera_eye_direction);
	vec3 r = reflect(-l, n);

	float cos_alpha = clamp(dot(e, r), 0, 1);

	return material_ambient_color +
	       material_diffuse_color * color * intensity * cos_theta / (distance*distance) +
 	       material_specular_color * color * intensity * pow(cos_alpha, 5) / (distance*distance);
}

void main() {
	vec3 color = texture(texture_sampler, uv).rgb * vec3(0.1, 0.1, 0.1);

	for(int i = 0; i < num_lights; ++i) {
		color += calc_point_light(light_position[i], light_color[i], light_intensity[i], i);
	}

	f_color = vec4(color, 1.0);
}