/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


#include "Graphics/Shader.h"

//Screen dimension constants
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

bool init();
SDL_Surface* loadSurface(std::string path);
bool loadMedia();
void close();

static SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gCurrentSurface = nullptr;

static SDL_GLContext mainCtx;

SDL_Texture* loadTexture(std::string path);
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;


bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_LoadLibrary(nullptr);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (gWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	mainCtx = SDL_GL_CreateContext(gWindow);
	if (mainCtx == nullptr)
	{
		printf("Main Context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}


	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		
		return -1;
	}

	printf("OpenGL loaded\n");
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	SDL_GL_SetSwapInterval(1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int w, h;
	SDL_GetWindowSize(gWindow, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);



	return true;
}

//bool initGL()
//{
//	GLenum error = GL_NO_ERROR;
//}


void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[])
{

	if (!init())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	Shader stdShader("Graphics/Shaders/standard_vert.glsl", "Graphics/Shaders/standard_frag.glsl");

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	SDL_Event e; 
	bool quit = false; 

	while (!quit) 
	{ 
		while (SDL_PollEvent(&e)) 
		{ 
			if (e.type == SDL_QUIT) 
				quit = true; 
		} 

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// draw our first triangle
		stdShader.Use();

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(gWindow);
	}

	close();

	return 0;
}
