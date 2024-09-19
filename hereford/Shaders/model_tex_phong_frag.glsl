#version 460 core

const float PI = 3.14159265359;

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

vec3 CalcDirLightTex(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex, float shininess);
vec3 CalcPointLightTex(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex, float shininess);
vec3 CalcSpotLightTex(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex, float shininess);

vec3 CalcDirLightTex(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex, float shininess)
{
	vec3 lightDir = normalize(-light.direction);
	
	vec3 ambient = light.ambient * diffuseTex;

	float diffDot = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diffuseTex * diffDot;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = light.specular * specularTex * specDot;

	return ambient + diffuse + specular;
}

vec3 CalcPointLightTex(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex, float shininess)
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 lightDir = normalize(light.position - FragPos);

	vec3 ambient = light.ambient * diffuseTex;
	ambient *= attenuation;

	float diffDot = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diffuseTex * diffDot;
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = light.specular * specularTex * specDot;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLightTex(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex, float shininess)
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * diffuseTex;
	ambient *= attenuation;

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 norm = normalize(normal);
	float diffDot = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diffuseTex * diffDot;
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir, norm);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = light.specular * specularTex * specDot;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	vec3 result;
	float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}

uniform sampler2D tex_diffuse_1;
uniform sampler2D tex_specular_1;

uniform int lightNum;
uniform PointLight pointLights[16];
uniform float shininess;
uniform vec3 eyePos;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(eyePos - WorldPos);
	vec3 result = vec3(0.0f);

	vec3 diffTex = vec3(texture(tex_diffuse_1, TexCoords));
	vec3 specTex = vec3(texture(tex_specular_1, TexCoords));

	for(int i=0;i<lightNum;i++)
	{
		result += CalcPointLightTex(pointLights[i], norm, WorldPos, viewDir, diffTex, specTex, shininess);
	}

    //vec3 result = CalcDirLightSimple(ddLight, norm, viewDir, outColor, 64.0f);
    FragColor = vec4(result, 1.0f);
}

