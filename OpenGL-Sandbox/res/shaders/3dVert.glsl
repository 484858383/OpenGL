#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoords;
layout (location = 2) in float a_faceBrightness;

out vec2 v_texCoords;
out float v_faceBrightness;

uniform mat4 u_projView;

void main()
{
	v_texCoords = a_texCoords;
	v_faceBrightness = a_faceBrightness;
	gl_Position = u_projView * vec4(a_position, 1.0);
}