#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoords;

out vec2 v_texCoords;
out float v_faceBrightness;

uniform mat4 u_projView;
uniform float u_time;

void main()
{
	v_texCoords = a_texCoords;
	v_faceBrightness = 1.0;

	float y = a_position.y - 0.1;
	y += 0.05 * cos(a_position.x + 3 * u_time);
	y -= 0.05 * sin(a_position.z + u_time);

	gl_Position = u_projView * vec4(a_position.x, y, a_position.z, 1.0);
}