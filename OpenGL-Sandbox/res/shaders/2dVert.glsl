#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoords;

out vec2 v_texCoords;

uniform mat4 u_transform;

void main()
{
	v_texCoords = a_texCoords;
	gl_Position = vec4(a_position, 1.0) * u_transform;
}