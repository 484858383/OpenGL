#version 460

out vec4 outColor;

in vec2 v_texCoords;

uniform sampler2D u_texture;

void main()
{
	vec4 frag = (texture(u_texture, v_texCoords));
	if(frag.a == 0)
		discard;
	outColor = vec4(frag.xyz, frag.a);
}