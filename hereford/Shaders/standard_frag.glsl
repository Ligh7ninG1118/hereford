#version 460 core


struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

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


vec3 CalcDirLightSimple(DirLight light, vec3 normal, vec3 viewDir, vec3 objColor, float shininess);
vec3 CalcPointLightSimple(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 objColor, float shininess);
vec3 CalcSpotLightSimple(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 objColor, float shininess);

vec3 CalcDirLightTex(DirLight light, vec3 normal, vec3 viewDir, Material material, vec2 texCoords);
vec3 CalcPointLightTex(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, Material material, vec2 texCoords);
vec3 CalcSpotLightTex(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, Material material, vec2 texCoords);

vec3 CalcDirLightSimple(DirLight light, vec3 normal, vec3 viewDir, vec3 objColor, float shininess)
{
	vec3 lightDir = normalize(-light.direction);
	
	vec3 ambient = light.ambient * objColor;

	float diffDot = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * objColor * diffDot;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = light.specular * objColor* specDot;

	return ambient + diffuse + specular;
}

vec3 CalcPointLightSimple(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 objColor, float shininess)
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 lightDir = normalize(light.position - FragPos);

	vec3 ambient = light.ambient * objColor;
	ambient *= attenuation;

	float diffDot = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * objColor * diffDot;
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = light.specular * objColor * specDot;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLightSimple(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, vec3 objColor, float shininess)
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * objColor;
	ambient *= attenuation;

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 norm = normalize(normal);
	float diffDot = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * objColor * diffDot;
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir, norm);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = light.specular * objColor * specDot;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	vec3 result;
	float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}

vec3 CalcDirLightTex(DirLight light, vec3 normal, vec3 viewDir, Material material, vec2 texCoords)
{
	vec3 lightDir = normalize(-light.direction);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	float diffDot = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, texCoords))) * diffDot;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (vec3(texture(material.specular, texCoords)) )* specDot;

	return ambient + diffuse + specular;
}

vec3 CalcPointLightTex(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir, Material material, vec2 texCoords)
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 lightDir = normalize(light.position - FragPos);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	ambient *= attenuation;

	float diffDot = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, texCoords))) * diffDot;
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (vec3(texture(material.specular, texCoords)) )* specDot;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLightTex(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir, Material material, vec2 texCoords)
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	ambient *= attenuation;

	vec3 lightDir = normalize(light.position - FragPos);
	vec3 norm = normalize(normal);
	float diffDot = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, texCoords))) * diffDot;
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir, norm);
	float specDot = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (vec3(texture(material.specular, texCoords)) )* specDot;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	vec3 result;
	float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	return ambient + diffuse + specular;
}

uniform int lightNum;

uniform PointLight pointLight[16];
uniform vec3 viewPos;

in vec3 outColor;
in vec3 FragPos;
in vec3 Norm;
in vec2 TexCoords;

uniform vec3 inColor;


out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Norm);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0f);


	for(int i=0;i<lightNum;i++)
	{
		result += CalcPointLightSimple(pointLight[i], norm, FragPos, viewDir, inColor, 64.0f);
	}

    //vec3 result = CalcDirLightSimple(ddLight, norm, viewDir, outColor, 64.0f);
    FragColor = vec4(result, 1.0f);
} 