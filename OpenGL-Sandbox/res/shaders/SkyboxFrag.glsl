#version 460

out vec4 outColor;

in vec3 v_texCoords;

uniform samplerCube u_skybox;

void main()
{
	outColor = texture(u_skybox, v_texCoords);
}