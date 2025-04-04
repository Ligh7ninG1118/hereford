#version 460 core

const float PI = 3.14159265359;

struct PointLight
{
	vec3 position;

	vec3 color;
};


uniform sampler2D tex_diffuse_1;
uniform sampler2D tex_normals_1;
uniform sampler2D tex_emissive_1;
uniform sampler2D tex_metallic_1;
uniform sampler2D tex_roughness_1;
uniform sampler2D tex_height_1;

uniform sampler2D tex_ao_1;
uniform samplerCube skybox;
uniform samplerCube prefilter;
uniform sampler2D brdfLUT;

uniform int lightNum;
uniform PointLight pointLights[16];

uniform bool combinedMR;

uniform vec3 eyePos;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in mat3 TBN;
out vec4 FragColor;

float DistributionComponent(vec3 N, vec3 H, float roughness);
float GeometryComponent(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelComponent(float cosTheta, vec3 f0, float roughness);
vec3 CalculateNormalFromMap(vec2 texCoords)
{
    vec3 tangentNormal = texture(tex_normals_1, texCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 matTBN = mat3(T, B, N);

    return normalize(matTBN * tangentNormal);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float height = texture(tex_height_1, texCoords).r;

    float heightScale = 0.05;
    height = height * heightScale - heightScale / 2.0;

    return texCoords - (viewDir.xy * height);
}


void main()
{    
    //sRGB
    vec3 albedo = pow(texture(tex_diffuse_1, TexCoords).rgb, vec3(2.2f));
    float roughness = texture(tex_roughness_1, TexCoords).g;
    float metallic = texture(tex_metallic_1, TexCoords).b;
    float ao = texture(tex_ao_1, TexCoords).r;
    vec3 emissive = texture(tex_emissive_1, TexCoords).rgb;

    vec3 N = CalculateNormalFromMap(TexCoords);
    vec3 V = normalize(eyePos - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0f);
    for(int i=0;i<lightNum;i++)
    {
        vec3 L = normalize(pointLights[i].position - WorldPos);
        vec3 H = normalize(V+L);
        float distance = length(pointLights[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;

        float NDF = DistributionComponent(N, H, roughness);
        float G = GeometryComponent(N, V, L, roughness);
        
        vec3 F = FresnelComponent(max(dot(H, V), 0.0f), F0, roughness);

        vec3 num = NDF * G * F;
        //adding small number to prevent divide by zero
        float denom = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L),0.0f) + 0.0001f;
        vec3 specular = num / denom;

        vec3 kS = F;
        vec3 kD = vec3(1.0f) - kS;
        kD *= 1.0f - metallic;

        float NdotL = max(dot(N, L), 0.0f);
        Lo += (kD * albedo/PI + specular) * radiance * NdotL;
    }

    vec3 F = FresnelComponent(max(dot(N,V),0.0f), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;

    //TODO: reflection upside down?
    vec3 irradiance = texture(skybox, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilter, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N,V),0.0f), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0f));
    //color = pow(color, vec3(1.0f/2.2f));
    //color += emissive;

    FragColor = vec4(color, 1.0f) ;
}

float DistributionComponent(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometryComponent(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);

    float r = roughness + 1.0f;
    float k = (r*r)/8.0f;

    float num1 = NdotV;
    float denom1 = NdotV * (1.0f - k) + k;
    float ggx1 = num1 / denom1;

    float num2 = NdotL;
    float denom2 = NdotL * (1.0f - k) + k;
    float ggx2 = num2 / denom2;

    return ggx1 * ggx2;
}

vec3 FresnelComponent(float cosTheta, vec3 f0, float roughness)
{
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow( clamp(1 - cosTheta, 0.0, 1.0), 5.0f);
}