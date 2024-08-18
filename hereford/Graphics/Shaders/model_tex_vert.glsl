#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;
layout (location = 5) in ivec4 inBoneIDs;
layout (location = 6) in vec4 inWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat4 sampleMat;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];



void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i=0;i<MAX_BONE_INFLUENCE;i++)
    {
        if(inBoneIDs[i] == -1)
            continue;
        if(inBoneIDs[i] >= MAX_BONES)
        {
            totalPosition = vec4(inPos, 1.0f);
            break;
        }
        vec4 localPos = finalBonesMatrices[inBoneIDs[i]] * vec4(inPos, 1.0f);
        totalPosition += localPos * inWeights[i];
        vec3 localNormal = mat3(finalBonesMatrices[inBoneIDs[i]]) * inNormal;
     }
    sampleMat = finalBonesMatrices[inBoneIDs[0]];
    TexCoords = inTexCoords;    
    WorldPos = vec3(model * totalPosition);
    Normal = inNormal;
    gl_Position = projection * view * model * totalPosition;

}