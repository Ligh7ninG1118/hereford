#include "Renderer.h"
#include "RenderComponent.h"
#include "AnimatedRenderComponent.h"
#include "LightComponent.h"
#include "Core/GameContext.h"
#include "Asset/Texture.h"
#include "Asset/Model.h"
#include "Asset/AssetManager.h"
#include "Animation/Animator.h"
#include "Animation/Animation.h"
#include "Animation/AnimationStateMachine.h"
#include "UI/UIElement.h"
#include "UI/UIAmmoIndicator.h"
#include "Gameplay/WeaponComponent.h"

#include <stb_image.h>

#include <glad/glad.h>

#include <iostream>
#include <format>
#include <algorithm>

#include "ft2build.h"
#include FT_FREETYPE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"



Renderer::Renderer(SDL_Window* sdlWindow, class GameContext* gameContext, int width, int height)
	:
	mPtrSDLWindowContext(sdlWindow),
	mPtrMainCamera(nullptr),
	mPtrGameContext(gameContext),
	mScreenWidth(width),
	mScreenHeight(height)
{
	/*for (int i = 0; i < 8; i++)
	{
		mRenderComponents.push_back

	}*/
}

Renderer::~Renderer()
{
	//Handled in Shutdown()
}

bool Renderer::Initialize()
{
	mGLContext = SDL_GL_CreateContext(mPtrSDLWindowContext);
	if (mGLContext == nullptr)
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

	glViewport(0, 0, mScreenWidth, mScreenHeight);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

	debugLineShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/debug_line_vert.glsl*Shaders/debug_line_frag.glsl"));
	debugLineShader->Use();
	debugLineShader->SetVec4("startColor", 0.0f, 1.0f, 0.0f, 1.0f);
	debugLineShader->SetVec4("endColor", 1.0f, 0.0f, 0.0f, 1.0f);

	skyboxShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/skybox_vert.glsl*Shaders/skybox_frag.glsl"));
	textShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_text_vert.glsl*Shaders/ui_text_frag.glsl"));

	glGenTextures(1, &skyboxTexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);

	std::vector<std::string> textures_faces{ "LocalResources/skybox/AboveDay_Right.png", "LocalResources/skybox/AboveDay_Left.png", 
	"LocalResources/skybox/AboveDay_Up.png", "LocalResources/skybox/AboveDay_Down.png", "LocalResources/skybox/AboveDay_Back.png", 
	"LocalResources/skybox/AboveDay_Front.png"};

	int width, height, channelNum;
	unsigned char* data;
	for (Uint32 i = 0; i < textures_faces.size(); i++)
	{
		data = stbi_load(textures_faces[i].c_str(), &width, &height, &channelNum, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}

	}
	//delete data;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	skyboxVAOID = skyboxVAO;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		printf("ERROR::FREETYPE: Could not init Freetype Libaray\n");
	}

	FT_Face face;
	if (FT_New_Face(ft, "LocalResources/Fonts/ChakraPetch-Regular.ttf", 0, &face))
	{
		printf("ERROR::FREETYPE: Failed to load font\n");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("ERROR::FREETYPE: Failed to load Glyph\n");
			continue;
		}

		Uint32 texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			Vec2((int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows),
			Vec2((int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void Renderer::Shutdown()
{
	glDeleteVertexArrays(1, &skyboxVAOID);
	glDeleteTextures(1, &skyboxTexID);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	SDL_GL_DeleteContext(mGLContext);
}

void Renderer::Render(float deltaTime)
{
	srand(time(NULL));
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Mat4 projection = mPtrMainCamera->GetPerspMatrix((float)mScreenWidth/mScreenHeight);
	Mat4 view = mPtrMainCamera->GetViewMatrix();

	Uint32 lastShaderID = 0;
	Uint32 lastVAOID = 0;

	glDepthFunc(GL_LEQUAL);
	skyboxShader->Use();
	skyboxShader->SetMat4("projection", projection);
	Mat4 view2 = mPtrMainCamera->GetViewMatrix();
	view2.m[0][3] = view2.m[1][3] = view2.m[2][3] = view2.m[3][0] = view2.m[3][1] = view2.m[3][2] = 0;

	skyboxShader->SetMat4("view", view2);
	skyboxShader->SetInt("skybox", 0);
	glBindVertexArray(skyboxVAOID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	for (const auto& renderComponents : mRenderComponentMap)
	{
		// Clear depth buffer across different layers
		glClear(GL_DEPTH_BUFFER_BIT);

		for (const auto& renderComponent : renderComponents.second)
		{
			unsigned int VAO, VBO;
			VAO = renderComponent->GetVAOID();
			glBindVertexArray(VAO);

			auto shader = renderComponent->GetShader().get();
			shader->Use();

			shader->SetInt("lightNum", 1);

			shader->SetVec3("pointLights[0].position", Vec3(0.0f, 5.0f, 0.0f));

			shader->SetVec3("pointLights[0].ambient", Vec3(0.1f, 0.1f, 0.1f));
			shader->SetVec3("pointLights[0].diffuse", Vec3(0.6f, 0.6f, 0.6f));
			shader->SetVec3("pointLights[0].specular", Vec3(1.0f, 1.0f, 1.0f));
			shader->SetVec3("pointLights[0].color", Vec3(10.0f, 10.0f, 10.0f));

			shader->SetFloat("pointLights[0].constant", 1.0f);
			shader->SetFloat("pointLights[0].linear", 0.007f);
			shader->SetFloat("pointLights[0].quadratic", 0.0002f);


			shader->SetVec3("eyePos", mPtrMainCamera->GetCameraPosition());

			Mat4 model = renderComponent->GetModelMatrix();
			shader->SetMat4("model", model);
			shader->SetMat4("projection", projection);
			shader->SetMat4("view", view);

			if (auto animRenderComp = dynamic_cast<AnimatedRenderComponent*>(renderComponent); animRenderComp != nullptr)
			{
				Mat4 model = animRenderComp->GetModelMatrix();

				shader->SetMat4("model", model);

				auto transforms = animRenderComp->GetAnimator()->GetFinalBoneMatrices();
				for (int i = 0; i < transforms.size(); i++)
				{
					shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
				}

				auto meshes = animRenderComp->GetMeshes();
				for (unsigned int i = 0; i < meshes.size(); i++)
				{
					Mesh* mesh = &meshes[i];

					unsigned int diffuseNr = 1;
					unsigned int specularNr = 1;
					unsigned int normalNr = 1;
					unsigned int heightNr = 1;
					for (unsigned int j = 0; j < mesh->mTextures.size(); j++)
					{
						glActiveTexture(GL_TEXTURE0 + j);

						std::string texStr;
						switch (mesh->mTextures[j].GetType())
						{
						case ETextureType::DIFFUSE:
							texStr = "tex_diffuse_1";
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
						shader->SetInt(texStr.c_str(), j);
						glBindTexture(GL_TEXTURE_2D, mesh->mTextures[j].GetID());
					}

					glBindVertexArray(mesh->mVAOID);
					glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->mIndices.size()), GL_UNSIGNED_INT, 0);

					glBindVertexArray(0);
					glActiveTexture(GL_TEXTURE0);
				}
			}
			else
			{
				if (renderComponent->mTextures.size() > 0)
				{
					for (unsigned int j = 0; j < renderComponent->mTextures.size(); j++)
					{
						glActiveTexture(GL_TEXTURE0 + j);

						std::string texStr;
						switch (renderComponent->mTextures[j]->GetType())
						{
						case ETextureType::DIFFUSE:
							texStr = "tex_diffuse_1";
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
						shader->SetInt(texStr.c_str(), j);
						glBindTexture(GL_TEXTURE_2D, renderComponent->mTextures[j]->GetID());
					}
				}
				else
				{
					shader->SetVec3("inColor", renderComponent->GetColor());
				}

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textShader->Use();
	Mat4 uiProj = mPtrMainCamera->GetOrthoMatrix(0.0f, static_cast<float>(mScreenWidth), 0.0f, static_cast<float>(mScreenHeight));

	textShader->SetVec3("textColor", Vec3(0.1f, 0.1f, 0.1f));
	textShader->SetMat4("projection", uiProj);
	textShader->SetInt("text", 0);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(textVAO);
	std::string::const_iterator c;
	// TODO: toString for my math classes?
	const Vec3 pos = mPtrMainCamera->GetOwner()->GetPosition();
	const Vec3 rot = mPtrMainCamera->GetRotation();
	const uint32 cpuTime = mPtrGameContext->cpuTime;
	const uint32 gpuTime = mPtrGameContext->gpuTime;

	/*std::string text = std::format("Pos: ({:.2f}, {:.2f}, {:.2f}) Rot:  ({:.2f}, {:.2f}, {:.2f}) CPU Frame Time: {:d} GPU Frame Time: {:d}"
		, pos.mX, pos.mY, pos.mZ, rot.mX, rot.mY, rot.mZ, cpuTime, gpuTime);
	float x = 100.0f;
	float y = mScreenHeight - 100.0f;
	float scale = 0.5f;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		float xpos = x + ch.mBearing.mX * scale;
		float ypos = y - (ch.mSize.mY - ch.mBearing.mY) * scale;

		float w = ch.mSize.mX * scale;
		float h = ch.mSize.mY * scale;

		float vertices[6][4] =
		{
			{ xpos,		ypos + h,	0.0f, 0.0f},
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.mTextureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.mAdvance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0); */
	

	for (auto uiElement : mUIElements)
	{
		//TODO: Costly! Update UI content using event
		uiElement->UpdateContent();
		glBindVertexArray(uiElement->GetVAO());
		uiElement->GetShader()->Use();
		if(auto uiImage = dynamic_cast<UIImage*>(uiElement);uiImage != nullptr)
			glBindTexture(GL_TEXTURE_2D, uiImage->GetTexture()->GetID());
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(mPtrSDLWindowContext);
}

void Renderer::SetMainCamera(CameraComponent* pMainCam)
{
	mPtrMainCamera = pMainCam;
}

void Renderer::AddDebugLines(Vec3 startPos, Vec3 endPos)
{
	float vertices[] =
	{
		startPos.mX, startPos.mY, startPos.mZ, 0.0f,
		endPos.mX, endPos.mY, endPos.mZ, 1.0f
	};
	uint32 vaoID, vboID;
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	mDebugLines.push_back(vaoID);
}

void Renderer::AddRenderComponent(RenderComponent* c)
{
	ERenderLayer renderLayer = c->GetRenderLayer();
	mRenderComponentMap[renderLayer].push_back(c);
}

void Renderer::RemoveRenderComponent(RenderComponent* c)
{
	ERenderLayer renderLayer = c->GetRenderLayer();
	mRenderComponentMap[renderLayer].erase(std::find(mRenderComponentMap[renderLayer].begin(), mRenderComponentMap[renderLayer].end(), c));
}

void Renderer::AddLightComponent(LightComponent* c)
{
	mLightComponents.push_back(c);
}

void Renderer::RemoveLightComponent(LightComponent* c)
{
	mLightComponents.erase(std::find(mLightComponents.begin(), mLightComponents.end(), c));
}

void Renderer::AddUIElement(UIElement* ui)
{
	mUIElements.push_back(ui);
}

void Renderer::RemoveUIElement(UIElement* ui)
{
	mUIElements.erase(std::find(mUIElements.begin(), mUIElements.end(), ui));
}
