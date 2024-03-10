#include "RenderComponent.h"
#include "../Core/Actor.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

RenderComponent::RenderComponent(Actor* owner, Renderer* renderer)
	: 
	Component(owner),
	pRenderer(renderer)
{
	pRenderer->AddRenderComponent(this);
	Initialize();
}

RenderComponent::~RenderComponent()
{
}

bool RenderComponent::Initialize()
{
	Shader stdShader = Shader("Graphics/Shaders/standard_vert.glsl", "Graphics/Shaders/standard_frag.glsl");
	SetShaderID(stdShader.ID);

	float vertices[] = {
		// positions
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	glGenVertexArrays(1, &m_VertexAttributeObjectID);
	glGenBuffers(1, &m_VertexBufferObjectID);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_VertexAttributeObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return true;

}



unsigned int RenderComponent::GetShaderID()
{
	return m_ShaderID;
}

void RenderComponent::SetShaderID(unsigned int id)
{
	m_ShaderID = id;
}

unsigned int RenderComponent::GetVAOID()
{
	return m_VertexAttributeObjectID;
}

void RenderComponent::SetVAOID(unsigned int id)
{
	m_VertexAttributeObjectID = id;
}

unsigned int RenderComponent::GetVBOID()
{
	return m_VertexBufferObjectID;
}

void RenderComponent::SetVBOID(unsigned int id)
{
	m_VertexBufferObjectID = id;
}

glm::mat4 RenderComponent::GetModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	Vector3 rawPos = mOwner->GetPosition();

	glm::vec3 pos(rawPos.mX, rawPos.mY, rawPos.mZ);

	model = glm::translate(model, pos);
	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	return model;
}
