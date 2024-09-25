#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

const float PI = 3.14159265359;

void main()
{
	vec3 N = normalize(TexCoords);
	vec3 irradiance = vec3(0.0f);

	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = normalize(cross(up, N));
	up = normalize(cross(N, right));

	float sampleDelta = 0.025f;
	float sampleNum = 0.0f;

	for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta)
	{
		for(float theta = 0.0f; theta < 0.5f *PI; theta += sampleDelta)
		{
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

			irradiance += texture(skybox, sampleVec).rgb * cos(theta) * sin(theta);
			sampleNum++;
		}
	}
	irradiance = PI * irradiance * (1.0f/float(sampleNum));

	FragColor = vec4(irradiance, 1.0f);
}