#version 460

out vec4 outColor;

in vec2 v_texCoords;

uniform sampler2D u_texture;

void main()
{
	outColor = vec4(texture(u_texture, v_texCoords));
}