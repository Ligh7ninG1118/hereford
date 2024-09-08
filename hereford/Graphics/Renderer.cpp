#include "Renderer.h"
#include "RenderComponent.h"
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
	mPtrSDLWindowContext(sdlWindow),
	mPtrMainCamera(nullptr),
	mPtrGameContext(gameContext),
	mScreenWidth(width),
	mScreenHeight(height)
{
	
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	mPtrGLContext = SDL_GL_CreateContext(mPtrSDLWindowContext);
	if (mPtrGLContext == nullptr)
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

	std::vector<Animation> dancingAnimClips;

	//TODO: having two copies of the data right now
	
	dancingModel = AssetManager::LoadAsset<Model>(std::string("LocalResources/SillyDancing/Silly Dancing.dae"));
	dancingAnimClips = Animation::LoadAnimations("LocalResources/SillyDancing/Silly Dancing.dae", dancingModel.get());

	dancingAnimator = new Animator(dancingAnimClips);
	dancingAnimator->SetShouldLoop(true);

	testShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/model_tex_vert.glsl*Shaders/model_tex_frag.glsl"));
	skyboxShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/skybox_vert.glsl*Shaders/skybox_frag.glsl"));
	textShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_text_vert.glsl*Shaders/ui_text_frag.glsl"));


	std::shared_ptr<Texture> testUI = AssetManager::LoadAsset<Texture>(std::string("LocalResources/rifle-round-silhouette.png"));
	std::shared_ptr<Shader> uiShader = AssetManager::LoadAsset<Shader>(std::string("Shaders/ui_image_ammo_count_vert.glsl*Shaders/ui_image_ammo_count_frag.glsl"));

	//TODO: Unchanged shader values dont need to update every frame
	Mat4 uiProj = mPtrMainCamera->GetOrthoMatrix(0.0f, static_cast<float>(mScreenWidth), 0.0f, static_cast<float>(mScreenHeight));

	uiAmmo = new UIAmmoIndicator(this, uiShader.get(), testUI, weapon);
	uiAmmo->Initialize();
	uiAmmo->SetUIProjection(uiProj);


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
	SDL_GL_DeleteContext(mPtrGLContext);
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

	if (dancingAnimator)
		dancingAnimator->UpdateAnimation(deltaTime);

	testShader->Use();
	testShader->SetMat4("projection", projection);

	testShader->SetMat4("view", view);

	Mat4 model = Mat4::Identity;
	model.Scale(0.03f);
	model.Rotate(DEG2RAD * 90.0f, Vec3::Up);
	//TODO: I messed up axis again? big time
	Vec3 playerRot = mPtrMainCamera->GetRotation();
	//playerRot *= DEG2RAD;
	//Vec3 actualRot = Vec3(-playerRot.mY, playerRot.mX, 0.0f);
	//TODO: Prone to gimbal lock, cant switch order
	//		Use quaternion!
	model.Rotate(DEG2RAD * playerRot.mY, Vector3::Down);
	model.Rotate(DEG2RAD * playerRot.mX, Vector3::Left);
	//model.Rotate(Quaternion::EulerToQuat(actualRot));
	//model.Translate(Vec3(-0.25f, 1.32f, -0.05f));
	model.Translate(Vec3(0.0f, 1.32f, 0.0f));

	model.Translate(mPtrMainCamera->GetOwner()->GetPosition());

	
	//model.Rotate(DEG2RAD * playerRot.mZ, Vector3::Up);

	testShader->SetMat4("model", model);
	testShader->SetVec3("pointLight.position", Vec3(0.0f, 5.0f, 0.0f));
	testShader->SetVec3("pointLight.color", Vec3(10.0f, 10.0f, 10.0f));
	testShader->SetVec3("eyePos", mPtrMainCamera->GetCameraPosition());

	if (gunAnimator)
	{
		auto transforms = gunAnimator->GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); i++)
		{
			testShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		}
	}

	int cum = 0;

	for (unsigned int i = 0; i < gunModel->mMeshes.size(); i++)
	{
		Mesh* mesh = &gunModel->mMeshes[i];
		
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
				texStr = "tex_diffuse_1";// +std::to_string(diffuseNr++);
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
			testShader->SetInt(texStr.c_str(), j);
			glBindTexture(GL_TEXTURE_2D, mesh->mTextures[j].GetID());
			cum = j;
		}

		glBindVertexArray(mesh->mVAOID);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->mIndices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	Mat4 dModel = Mat4::Identity;
	dModel.Translate(Vec3(0.0f, 0.4f, -2.0f));

	testShader->SetMat4("model", dModel);


	if (dancingAnimator)
	{
		auto transforms = dancingAnimator->GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); i++)
		{
			testShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		}
	}

	for (unsigned int i = 0; i < dancingModel->mMeshes.size(); i++)
	{
		Mesh* mesh = &dancingModel->mMeshes[i];

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int j = 0; j < mesh->mTextures.size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + j + cum);

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
			testShader->SetInt(texStr.c_str(), j + cum);
			glBindTexture(GL_TEXTURE_2D, mesh->mTextures[j].GetID());
		}

		glBindVertexArray(mesh->mVAOID);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->mIndices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}


	for (auto renderComponent : mRenderComponents)
	{
		unsigned int VAO, VBO;
		Shader* shader;
		VAO = renderComponent->GetVAOID();
		shader = renderComponent->GetShader().get();

		glBindVertexArray(VAO);
		glUseProgram(shader->GetID());

		shader->SetInt("lightNum", 1);

		/*for (int i = 0; i < lightSize; i++)
		{
			ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].position", mLightComponents[i]->GetPosition());
			ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].ambient", mLightComponents[i]->GetPointLight().ambient);
			ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].diffuse", mLightComponents[i]->GetPointLight().diffuse);
			ShaderOp::SetVec3(shaderID, "pointLight[" + std::to_string(i) + "].specular", mLightComponents[i]->GetPointLight().specular);

			ShaderOp::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].constant", mLightComponents[i]->GetPointLight().constant);
			ShaderOp::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].linear", mLightComponents[i]->GetPointLight().linear);
			ShaderOp::SetFloat(shaderID, "pointLight[" + std::to_string(i) + "].quadratic", mLightComponents[i]->GetPointLight().quadratic);
		}*/

		shader->SetVec3("pointLight[0].position", Vec3(0.0f, 5.0f, 0.0f));

		shader->SetVec3("pointLight[0].ambient", Vec3(0.1f, 0.1f, 0.1f));
		shader->SetVec3("pointLight[0].diffuse", Vec3(0.6f, 0.6f, 0.6f));
		shader->SetVec3("pointLight[0].specular", Vec3(1.0f, 1.0f, 1.0f));

		shader->SetFloat("pointLight[0].constant", 1.0f);
		shader->SetFloat("pointLight[0].linear", 0.007f);
		shader->SetFloat("pointLight[0].quadratic", 0.0002f);

		shader->SetVec3("viewPos", mPtrMainCamera->GetCameraPosition());

		Mat4 model = renderComponent->GetModelMatrix();
		shader->SetMat4("model", model);
		shader->SetMat4("projection", projection);
		shader->SetMat4("view", view);

		shader->SetVec3("inColor", renderComponent->GetColor());

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Draw crosshair
	{
		glBindVertexArray(crosshairVAOID);
		glUseProgram(debugShaderID);

		glLineWidth(1.5f);
		glDrawArrays(GL_LINES, 0, 4);
	}

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


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textShader->Use();
	// TODO: use cast
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

	std::string text = std::format("Pos: ({:.2f}, {:.2f}, {:.2f}) Rot:  ({:.2f}, {:.2f}, {:.2f}) CPU Frame Time: {:d} GPU Frame Time: {:d}"
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
	glBindTexture(GL_TEXTURE_2D, 0); 
	
	uiAmmo->UpdateContent();
	glBindVertexArray(uiAmmo->GetVAO());
	glBindTexture(GL_TEXTURE_2D, uiAmmo->GetTexture()->GetID());
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_GL_SwapWindow(mPtrSDLWindowContext);

	//TODO: OpenGL release assets: DeleteVertexArray, DeleteBuffer
}

void Renderer::SetMainCamera(CameraComponent* pMainCam)
{
	mPtrMainCamera = pMainCam;
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

void Renderer::AddUIElement(UIElement* ui)
{
	mUIElements.push_back(ui);
}

void Renderer::RemoveUIElement(UIElement* ui)
{
	mUIElements.erase(std::find(mUIElements.begin(), mUIElements.end(), ui));
}
