#include "Renderer.h"
#include "RenderComponent.h"
#include "LightComponent.h"
#include "Core/GameContext.h"
#include "Asset/Texture.h"
#include "Asset/Model.h"

#include "Asset/AssetManager.h"

#include <glad/glad.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

Renderer::Renderer(SDL_Window* sdlWindow, class GameContext* gameContext, int width, int height)
	:
	m_pSDLWindowContext(sdlWindow),
	m_pMainCamera(nullptr),
	m_pGameContext(gameContext),
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

	debugShaderID = m_pGameContext->GetShader("Graphics/Shaders/debug_vert.glsl", "Graphics/Shaders/debug_frag.glsl");
	backpackShaderID = m_pGameContext->GetShader("Graphics/Shaders/model_tex_vert.glsl", "Graphics/Shaders/model_tex_frag.glsl");


	float cameraCenter[] = { 0.0f, 0.0355f, 0.0f, 1.0f, 1.0f, 1.0f,
							0.0f, -0.0355f, 0.0f, 1.0f, 1.0f, 1.0f,
							0.02f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
							-0.02f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	Uint32 vboID;
	glGenVertexArrays(1, &crosshairVAOID);
	glGenBuffers(1, &vboID);
	glBindVertexArray(crosshairVAOID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cameraCenter), cameraCenter, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	AssetManager* am = new AssetManager();
	testBackpack = am->LoadAsset<Model>(std::string("damagedhelmet/DamagedHelmet.gltf"));

	return true;
}

void Renderer::Shutdown()
{
	SDL_GL_DeleteContext(m_pGLContext);
}

void Renderer::Render(float deltaTime)
{
	srand(time(NULL));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(backpackShaderID);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Mat4 projection = m_pMainCamera->GetPerspMatrix((float)m_ScreenWidth/m_ScreenHeight);
	Mat4 view = m_pMainCamera->GetViewMatrix();

	Uint32 lastShaderID = 0;
	Uint32 lastVAOID = 0;

	for (unsigned int i = 0; i < testBackpack->mMeshes.size(); i++)
	{
		Mesh* mesh = &testBackpack->mMeshes[i];


		Shader::SetMat4(backpackShaderID, "projection", projection);

		Shader::SetMat4(backpackShaderID, "view", view);

		Mat4 model = Mat4::Identity;
		model.Translate(Vec3(5.0f, 1.5f, 0.0f));
		model.Rotate(-1.57f, Vec3::Up);
		model.Rotate(1.57f, Vec3::Right);

		Shader::SetMat4(backpackShaderID, "model", model);

		Shader::SetVec3(backpackShaderID, "pointLight.position", Vec3(0.0f, 5.0f, 0.0f));
		Shader::SetVec3(backpackShaderID, "pointLight.color", Vec3(150.0f, 150.0f, 150.0f));
		Shader::SetVec3(backpackShaderID, "eyePos", m_pMainCamera->GetCameraPosition());


		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < mesh->mTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string texStr;
			switch (mesh->mTextures[i].GetType())
			{
			case ETextureType::DIFFUSE:
				texStr = "tex_diffuse_1" + std::to_string(diffuseNr++);
				break;
			case ETextureType::SPECULAR:
				texStr = "tex_specular_1";
				break;
			case ETextureType::NORMALS:
				texStr = "tex_normals_1";
				break;
			case ETextureType::HEIGHT:
				texStr = "tex_height_1";
				break;
			case ETextureType::EMISSIVE:
				texStr = "tex_emissive_1";
				break;
			case ETextureType::METALNESS:
				texStr = "tex_metalrough_1";
				break;
			case ETextureType::AMBIENT:
				texStr = "tex_ao_1";
				break;
			default:
				break;
			}

			glUniform1i(glGetUniformLocation(backpackShaderID, texStr.c_str()), i);
			glBindTexture(GL_TEXTURE_2D, mesh->mTextures[i].GetID());
		}

		glBindVertexArray(mesh->mVAOID);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->mIndices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}


	/*for (auto renderComponent : mRenderComponents)
	{
		unsigned int VAO, VBO, shaderID;
		VAO = renderComponent->GetVAOID();
		shaderID = renderComponent->GetShaderID();


		glBindVertexArray(VAO);
		glUseProgram(shaderID);

		if (shaderID != lastShaderID)
		{

			int lightSize = mLightComponents.size();
			Shader::SetInt(shaderID, "lightNum", lightSize);

			for (int i = 0; i < lightSize; i++)
			{
				Shader::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].position", mLightComponents[i]->GetPosition());
				Shader::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].ambient", mLightComponents[i]->GetPointLight().ambient);
				Shader::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].diffuse", mLightComponents[i]->GetPointLight().diffuse);
				Shader::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].specular", mLightComponents[i]->GetPointLight().specular);

				Shader::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].constant", mLightComponents[i]->GetPointLight().constant);
				Shader::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].linear", mLightComponents[i]->GetPointLight().linear);
				Shader::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].quadratic", mLightComponents[i]->GetPointLight().quadratic);
			}


			Shader::SetVec3(shaderID, "viewPos", m_pMainCamera->GetCameraPosition());

			Shader::SetMat4(shaderID, "projection", projection);

			Shader::SetMat4(shaderID, "view", view);
		}

		lastShaderID = shaderID;

		Mat4 model = renderComponent->GetModelMatrix();
		Shader::SetMat4(shaderID, "model", model);
		Shader::SetVec3(shaderID, "inColor", renderComponent->color);

		glDrawArrays(GL_TRIANGLES, 0, 36);

	}*/

	// Draw crosshair
	/*{
		glBindVertexArray(crosshairVAOID);
		glUseProgram(debugShaderID);

		glLineWidth(1.5f);
		glDrawArrays(GL_LINES, 0, 4);
	}*/


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

void Renderer::AddLightComponent(LightComponent* c)
{
	mLightComponents.push_back(c);
}

void Renderer::RemoveLightComponent(LightComponent* c)
{
	mLightComponents.erase(std::find(mLightComponents.begin(), mLightComponents.end(), c));
}