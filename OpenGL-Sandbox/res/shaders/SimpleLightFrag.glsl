#version 460

out vec4 outColor;

in vec2 v_texCoords;
in vec3 v_fragPos;

uniform sampler2D u_texture;
uniform vec3 u_cameraPos;

const float k_constant = 1.0f;
const float k_linear = 0.01f;
const float k_quadratic = 0.002f;


void main()
{
	vec4 frag = (texture(u_texture, v_texCoords));
	if(frag.a == 0)
		discard;

	float d = length(u_cameraPos - v_fragPos);
	float attenuation = 1.0f / (k_constant + k_linear * d + k_quadratic * d * d);

	outColor = vec4(frag.xyz * attenuation, frag.a);
}