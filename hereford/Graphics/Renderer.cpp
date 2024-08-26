#include "Renderer.h"
#include "RenderComponent.h"
#include "LightComponent.h"
#include "Core/GameContext.h"
#include "Asset/Texture.h"
#include "Asset/Model.h"
#include "ShaderOp.h"
#include "Asset/AssetManager.h"
#include "Animation/Animator.h"
#include "Animation/Animation.h"

#include <stb_image.h>

#include <glad/glad.h>

#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

struct Character
{
	Uint32 mTextureID;
	Vec2 mSize;
	Vec2 mBearing;
	Uint32 mAdvance;
};

std::map<char, Character> Characters;
std::shared_ptr<Shader> textShader;
Uint32 textVAO, textVBO;


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

	std::vector<Animation> animClips;

	//TODO: having two copies of the data right now
	if (0)
	{
		testModel = am->LoadAsset<Model>(std::string("LocalResources/SillyDancing/Silly Dancing.dae"));
		animClips = Animation::LoadAnimations("LocalResources/SillyDancing/Silly Dancing.dae", testModel.get());
	}
	else
	{
		testModel = am->LoadAsset<Model>(std::string("LocalResources/mark23/source/Mark23v1.fbx"));
		animClips = Animation::LoadAnimations("LocalResources/mark23/source/Mark23v1.fbx", testModel.get());
	}
	testAnimator = new Animator(animClips);

	testShader = am->LoadAsset<Shader>(std::string("Shaders/model_tex_vert.glsl*Shaders/model_tex_frag.glsl"));
	skyboxShader = am->LoadAsset<Shader>(std::string("Shaders/skybox_vert.glsl*Shaders/skybox_frag.glsl"));
	textShader = am->LoadAsset<Shader>(std::string("Shaders/ui_text_vert.glsl*Shaders/ui_text_frag.glsl"));

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
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
	}

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
	SDL_GL_DeleteContext(m_pGLContext);
}

void Renderer::Render(float deltaTime)
{
	srand(time(NULL));
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Uint32 cumTexChannel = 0;

	Mat4 projection = m_pMainCamera->GetPerspMatrix((float)m_ScreenWidth/m_ScreenHeight);
	Mat4 view = m_pMainCamera->GetViewMatrix();

	Uint32 lastShaderID = 0;
	Uint32 lastVAOID = 0;

	if(testAnimator)
		testAnimator->UpdateAnimation(deltaTime);
	testShader->Use();
	Uint32 shaderID = testShader->GetID();

	ShaderOp::SetMat4(shaderID, "projection", projection);

	ShaderOp::SetMat4(shaderID, "view", view);

	Mat4 model = Mat4::Identity;
	model.Scale(0.03f);
	ShaderOp::SetMat4(shaderID, "model", model);

	ShaderOp::SetVec3(shaderID, "pointLight.position", Vec3(0.0f, 5.0f, 0.0f));
	ShaderOp::SetVec3(shaderID, "pointLight.color", Vec3(150.0f, 150.0f, 150.0f));
	ShaderOp::SetVec3(shaderID, "eyePos", m_pMainCamera->GetCameraPosition());

	if (testAnimator)
	{
		auto transforms = testAnimator->GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); i++)
		{
			ShaderOp::SetMat4(shaderID, "finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		}
	}

	for (unsigned int i = 0; i < testModel->mMeshes.size(); i++)
	{
		Mesh* mesh = &testModel->mMeshes[i];
		
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < mesh->mTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			cumTexChannel = i;

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
			ShaderOp::SetInt(shaderID, texStr.c_str(), i);
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
				ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].position", mLightComponents[i]->GetPosition());
				ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].ambient", mLightComponents[i]->GetPointLight().ambient);
				ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].diffuse", mLightComponents[i]->GetPointLight().diffuse);
				ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].specular", mLightComponents[i]->GetPointLight().specular);

				ShaderOp::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].constant", mLightComponents[i]->GetPointLight().constant);
				ShaderOp::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].linear", mLightComponents[i]->GetPointLight().linear);
				ShaderOp::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].quadratic", mLightComponents[i]->GetPointLight().quadratic);
			}


			ShaderOp::SetVec3(shaderID, "viewPos", m_pMainCamera->GetCameraPosition());

			ShaderOp::SetMat4(shaderID, "projection", projection);

			ShaderOp::SetMat4(shaderID, "view", view);
		}

		lastShaderID = shaderID;

		Mat4 model = renderComponent->GetModelMatrix();
		ShaderOp::SetMat4(shaderID, "model", model);
		ShaderOp::SetVec3(shaderID, "inColor", renderComponent->color);

		glDrawArrays(GL_TRIANGLES, 0, 36);

	}*/

	// Draw crosshair
	/*{
		glBindVertexArray(crosshairVAOID);
		glUseProgram(debugShaderID);

		glLineWidth(1.5f);
		glDrawArrays(GL_LINES, 0, 4);
	}*/

	glDepthFunc(GL_LEQUAL);
	skyboxShader->Use();
	ShaderOp::SetInt(skyboxShader->GetID(), "skybox", cumTexChannel + 2);
	ShaderOp::SetMat4(skyboxShader->GetID(), "projection", projection);
	Mat4 view2 = m_pMainCamera->GetViewMatrix();
	view2.m[0][3] = view2.m[1][3] = view2.m[2][3] = view2.m[3][0] = view2.m[3][1] = view2.m[3][2] = 0;

	ShaderOp::SetMat4(skyboxShader->GetID(), "view", view2);
	glBindVertexArray(skyboxVAOID);
	glActiveTexture(GL_TEXTURE0 + cumTexChannel + 2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textShader->Use();
	// TODO: use cast
	Mat4 uiProj = m_pMainCamera->GetOrthoMatrix(0.0f, (float)m_ScreenWidth, 0.0f, (float)m_ScreenHeight);

	ShaderOp::SetVec3(textShader->GetID(), "textColor", Vec3(0.7f, 0.7f, 0.0f));
	ShaderOp::SetMat4(textShader->GetID(), "projection", uiProj);
	ShaderOp::SetInt(textShader->GetID(), "text", cumTexChannel + 1);
	glActiveTexture(GL_TEXTURE0 + cumTexChannel + 1);

	glBindVertexArray(textVAO);
	std::string::const_iterator c;
	std::string text = "Hereford";
	float x = 300.0f;
	float y = 200.0f;
	float scale = 1.0f;
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
	glBindTexture(GL_TEXTURE_2D, 0);


	

	SDL_GL_SwapWindow(m_pSDLWindowContext);

	//TODO: OpenGL release assets: DeleteVertexArray, DeleteBuffer
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