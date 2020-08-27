#version 460

out vec4 outColor;

in vec2 v_texCoords;
in vec3 v_normals;
in vec3 v_fragPos;

uniform sampler2D u_texture;
uniform vec3 u_lightPos;

const float ambientStrength = 0.1f;
const vec3 lightColor = vec3(1, 1, 1);

const float k_constant = 1.0f;
const float k_linear = 0.01f;
const float k_quadratic = 0.002f;

void main()
{
	vec4 frag = (texture(u_texture, v_texCoords));
	if(frag.a == 0)
		discard;

	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(u_lightPos - v_fragPos);
	float diffuseStrength = max(dot(v_normals, lightDir), 0.0);

	vec3 diffuse = diffuseStrength * lightColor;

	float d = length(u_lightPos - v_fragPos);
	float attenuation = 1.0f / (k_constant + k_linear * d + k_quadratic * d * d);
	vec3 result = attenuation * (ambient + diffuse) * frag.xyz;

	outColor = vec4(result, frag.a);
}