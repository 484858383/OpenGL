#version 460

layout (location = 0) in vec3 a_position;

out vec3 v_texCoords;

uniform mat4 u_projView;

void main()
{
	v_texCoords = a_position;
	vec4 position = u_projView * vec4(a_position, 1.0);
	gl_Position = position.xyww;
}