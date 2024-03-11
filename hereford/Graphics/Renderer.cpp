#include "Renderer.h"
#include "RenderComponent.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

Renderer::Renderer(SDL_Window* sdlWindow, int width, int height)
	:
	m_pSDLWindowContext(sdlWindow),
	m_pMainCamera(nullptr),
	m_ScreenWidth(width),
	m_ScreenHeight(height)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	m_pGLContext = SDL_GL_CreateContext(m_pSDLWindowContext);
	if (m_pGLContext == nullptr)
	{
		printf("Renderer::Initialize(): Main Context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}


	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Renderer::Initialize(): Failed to initialize GLAD\n");
		return false;
	}

	printf("Renderer::Initialize(): OpenGL loaded\n");
	printf("Renderer::Initialize(): Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer::Initialize(): Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Renderer::Initialize(): Version:  %s\n", glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);


	return true;
}

void Renderer::Shutdown()
{
}

void Renderer::Render(float deltaTime)
{
	srand(time(NULL));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto renderComponent : mRenderComponents)
	{
		unsigned int VAO, VBO, shaderID;
		VAO = renderComponent->GetVAOID();
		VBO = renderComponent->GetVBOID();
		shaderID = renderComponent->GetShaderID();

		glBindVertexArray(VAO);
		glUseProgram(shaderID);

		glm::mat4 projection = m_pMainCamera->GetPerspMatrix();
		Shader::SetMat4(shaderID, "projection", projection);

		// camera/view transformation
		glm::mat4 view = m_pMainCamera->GetViewMatrix();
		Shader::SetMat4(shaderID, "view", view);

		glm::mat4 model = renderComponent->GetModelMatrix();
		Shader::SetMat4(shaderID, "model", model);

		Vector3 color((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);

		Shader::SetVec3(shaderID, "inColor", color);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	SDL_GL_SwapWindow(m_pSDLWindowContext);
}

void Renderer::SetMainCamera(CameraComponent* pMainCam)
{
	m_pMainCamera = pMainCam;
}

void Renderer::AddRenderComponent(RenderComponent* c)
{
	mRenderComponents.push_back(c);
}

void Renderer::RemoveRenderComponent(RenderComponent* c)
{
	mRenderComponents.erase(std::find(mRenderComponents.begin(), mRenderComponents.end(), c));

}