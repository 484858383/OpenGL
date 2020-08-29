#version 460

out vec4 outColor;

in vec2 v_texCoords;
in vec3 v_normals;
in vec3 v_fragPos;

const int max_lights = 32;

uniform sampler2D u_texture;

uniform vec3 u_lightPos;
uniform vec3 u_lightPositions[max_lights];
uniform int u_numberUsedLights;

uniform float u_time;

const float ambientStrength = 0.1f;
const vec3 lightColor = vec3(1, 1, 1);

const float k_constant = 1.0f;
const float k_linear = 0.045f;
const float k_quadratic = 0.0075f;

vec3 calculatePointLight(vec3 lightPos, vec3 normal, vec3 fragPos);
vec3 calculateDirectionalLight(vec3 direction, vec3 normal);

float calculateAttenuation(vec3 lightPos, vec3 fragPos);

void main()
{
	vec4 frag = (texture(u_texture, v_texCoords));
	if(frag.a == 0)
		discard;

	vec3 result = vec3(0, 0, 0);
	vec3 light;
	float att;

	result += frag.xyz * u_time;

	for(int i = 0; i < u_numberUsedLights; i++)
	{
		vec3 light = calculatePointLight(u_lightPositions[i], v_normals, v_fragPos);
		float att = calculateAttenuation(u_lightPositions[i], v_fragPos);
		result = min(result + (att * light * frag.xyz), frag.xyz);
	}

	outColor = vec4(result, frag.a);
}

vec3 calculatePointLight(vec3 lightPos, vec3 normal, vec3 fragPos)
{
	//vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuseStrength = max(dot(normal, lightDir), 0.0);

	//vec3 diffuse = diffuseStrength * lightColor;

	return (ambientStrength + diffuseStrength) * lightColor;
}

vec3 calculateDirectionalLight(vec3 direction, vec3 normal)
{
	vec3 lightDir = -direction;
	float diffuseStrength = max(dot(normal, lightDir), 0.0);
	return (ambientStrength + diffuseStrength) * lightColor;
}

float calculateAttenuation(vec3 lightPos, vec3 fragPos)
{
	float d = length(lightPos - fragPos);
	return 1.0f / (k_constant + k_linear * d + k_quadratic * d * d);
}