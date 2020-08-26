#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoords;
layout (location = 2) in float a_faceBrightness;
layout (location = 3) in vec3 a_normals;

out vec2 v_texCoords;
out float v_faceBrightness;
out vec3 v_normals;
out vec3 v_fragPos;

uniform mat4 u_projView;

void main()
{
	v_texCoords = a_texCoords;
	v_faceBrightness = a_faceBrightness;
	v_normals = a_normals;
	v_fragPos = a_position;

	gl_Position = u_projView * vec4(a_position, 1.0);
}