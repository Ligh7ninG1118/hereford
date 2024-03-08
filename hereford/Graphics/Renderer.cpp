#include "Renderer.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <string>

Renderer::Renderer(SDL_Window* sdlWindow, int width, int height)
	:
	m_pSDLWindowContext(sdlWindow),
	m_ScreenWidth(width),
	m_ScreenHeight(height)
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
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

	stdShader = Shader("Graphics/Shaders/standard_vert.glsl", "Graphics/Shaders/standard_frag.glsl");

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	return true;
}

void Renderer::Shutdown()
{
}

void Renderer::Render(float deltaTime)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	

	stdShader.Use();
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 3);
	

	glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(m_ScreenWidth) / static_cast<float>(m_ScreenHeight), 0.1f, 100.0f);
	stdShader.SetMat4("projection", projection);

	// camera/view transformation
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	view = camera.GetViewMatrix();
	stdShader.SetMat4("view", view);

	// render boxes
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		stdShader.SetMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	SDL_GL_SwapWindow(m_pSDLWindowContext);
}