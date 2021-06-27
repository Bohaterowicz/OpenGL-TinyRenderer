#include "tiny_renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl_util.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "shader_program.h"
#include "texture2d.h"
#include "texture1d.h"
#include "mesh_data.h"
#include "render_object.h"
#include "util.h"

/////////CAMERA DEFINES////////

#define FRUSTUM_Z_NEAR 0.01f
#define FRUSTUM_Z_FAR 100000.f
#define CAMERA_FOV 45.0f

///////////////////////////////

struct un_proj
{
	glm::mat4 ClipToCamera;
	glm::ivec2 WindowSize;
};

static bool32 invers;
static glm::vec2 CameraRotChange;

static un_proj UnProj;

glm::mat4 CalcLookAtMatrix(const glm::vec3 &cameraPt, const glm::vec3 &lookPt, const glm::vec3 &upPt)
{
	glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
	glm::vec3 upDir = glm::normalize(upPt);

	glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
	glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

	glm::mat4 rotMat(1.0f);
	rotMat[0] = glm::vec4(rightDir, 0.0f);
	rotMat[1] = glm::vec4(perpUpDir, 0.0f);
	rotMat[2] = glm::vec4(-lookDir, 0.0f);

	rotMat = glm::transpose(rotMat);

	glm::mat4 transMat(1.0f);
	transMat[3] = glm::vec4(-cameraPt, 1.0f);

	return rotMat * transMat;
}

void BuildGaussianData(std::vector<GLubyte> &TextureData, int CosAngleResolution, int ShineResolution)
{
	TextureData.resize(size_t(ShineResolution) * size_t(CosAngleResolution));

	auto currIt = TextureData.begin();
	for (int iShin = 1; iShin <= ShineResolution; iShin++)
	{
		float shininess = iShin / (float)(ShineResolution);
		for (int iCosAng = 0; iCosAng < CosAngleResolution; iCosAng++)
		{
			float cosAng = iCosAng / (float)(CosAngleResolution - 1);
			float angle = acosf(cosAng);
			float exponent = angle / shininess;
			exponent = -(exponent * exponent);
			float gaussianTerm = glm::exp(exponent);

			*currIt = (unsigned char)(gaussianTerm * 255.0f);
			++currIt;
		}
	}
}

void UpdateAndRender(tiny_renderer_window_info &WindowInfo, tiny_renderer_state &TinyRendererState, tiny_renderer_input &Input)
{

	if (TinyRendererState.IsInitialized() == FALSE)
	{
		//Create our OpenGL Renderer object, this object perform all the draw calls, and other associated functionality
		TinyRendererState.SetRenderer(std::make_unique<opengl_renderer>());
		real32 AspectRatio = (real32)WindowInfo.ClientWidth / (real32)WindowInfo.ClientHeight;
		TinyRendererState.SetCamera(std::make_unique<base_camera>(base_camera::FrustumScaleFromFOV(CAMERA_FOV), AspectRatio, FRUSTUM_Z_NEAR, FRUSTUM_Z_FAR));
		mesh_data Mesh = ReadObjFile("../../../resources/meshes/plane_big.obj");
		TinyRendererState.CreateRenderObject(Mesh);

		//We read source code from our shader files
		std::string VertexShaderSource = ReadFile("../../../resources/shaders/test_texture_vert.glsl");
		std::string FragmentShaderSource = ReadFile("../../../resources/shaders/checker_texture_frag.glsl");
		//We create (compile) shader during construction
		auto Shader = std::make_unique<shader_program>(VertexShaderSource, FragmentShaderSource);
		TinyRendererState.AddShader(std::move(Shader));

		TinyRendererState.GetRenderer()->CreateUniformBuffer("CameraProjectionMtxStack", 2 * sizeof(glm::mat4));
		TinyRendererState.GetRenderer()->CreateUniformBuffer("UnProjection", sizeof(un_proj));

		auto CheckerTexture = std::make_unique<texture2d>("../../../resources/textures/checker.png", false, 8, 1);
		CheckerTexture->Bind(0);
		TinyRendererState.AddTexture(std::move(CheckerTexture));

		TinyRendererState.SetInitialized(TRUE);
	}

	auto *const Renderer = TinyRendererState.GetRenderer();
	auto *const Camera = TinyRendererState.GetCamera();

	opengl_renderer::SetViewportDimensions(0, 0, WindowInfo.ClientWidth, WindowInfo.ClientHeight);

	//Create an vertex buffer layout object, that specifues the layout of data in the vertex buffer

	//Enable Blending, and specify blending mode (function)
	opengl_renderer::EnableAlphaBlending();

	auto *const Shader = TinyRendererState.GetShader(0);

	Shader->Bind();

	Renderer->SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(Camera->GetPerspectiveTransform()), sizeof(glm::mat4), sizeof(glm::mat4));
	Renderer->BindUniformBuffer("CameraProjectionMtxStack", 1);
	Shader->BindUniformBlock("ub_GlobalMatrices", 1);

	UnProj.ClipToCamera = glm::inverse(Camera->GetPerspectiveTransform());
	UnProj.WindowSize = glm::ivec2(WindowInfo.ClientWidth, WindowInfo.ClientHeight);

	Renderer->SetUniformBufferData("UnProjection", &UnProj, sizeof(un_proj), 0);
	Renderer->BindUniformBuffer("UnProjection", 2);
	Shader->BindUniformBlock("ub_UnProjection", 2);
	GLfloat maxAniso = 0.0F;
	GLCall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso));
	GLuint SamplerID;
	GLCall(glGenSamplers(1, &SamplerID));
	GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glSamplerParameterf(SamplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso));
	GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT));

	//std::vector<uint8> GaussianTexture;
	//BuildGaussianData(GaussianTexture, 256, 256);
	//texture2d GaussAngleTexture(&GaussianTexture[0], 256, 256, 1);

	//int32 Width, Height, BytesPerPixel;
	//uint8 *PixelBuffer = nullptr;
	//LoadPNG("../../../resources/textures/checker.png", &PixelBuffer, Width, Height, BytesPerPixel, 1);

	//texture2d ShineTexture(PixelBuffer, Width, Height, BytesPerPixel);

	//GaussAngleTexture.Bind(0);
	//GLCall(glBindSampler(0, SamplerID));
	//Shader.SetUniform1i("gaussianTexture", 0);
	GLCall(glBindSampler(0, SamplerID));
	Shader->SetUniform1i("checkerTexture", 0);

	Shader->Unbind();

	//Main loop
	float OffsetX = 0.0f;
	float OffsetY = 0.0f;

	glm::mat4 BaseTranslation(1.0f);
	BaseTranslation[3].z = -50.0f;
	glm::mat4 CamTransMtx;
	glm::mat4 ModelTrans(1.0f);
	glm::mat3 NormalTrans;
	glm::vec3 CamLightPos;
	glm::vec4 LightPos(55.0f, 40.0f, 0.0f, 1.0f);
	Renderer->Clear();

	//Perform a draw call via our renderer
	Shader->Bind();
	Camera->ProcessInput(Input);
	//CameraRotChange = {};
	CamTransMtx = Camera->GetCmameraTransformationMatrix(); //CalcLookAtMatrix(CameraOrbitPosition(), CameraTargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(CamTransMtx), sizeof(glm::mat4), 0);
	Shader->SetUniformMtx4("u_ModelTransformation", ModelTrans);
	NormalTrans = glm::mat3(CamTransMtx);
	//Used to fix normals when using non uniform scaling....
	if (invers)
	{
		NormalTrans = glm::transpose(glm::inverse(NormalTrans));
	}
	//CamLightPos = (CamTransMtx * (RotationMtx() * LightPos));
	//Shader->SetUniform3f("u_CameraSpaceLight", CamLightPos);
	Shader->SetUniform1i("u_UseSquaredAttenuationDistance", 1);
	Shader->SetUniform1f("u_LightAttenuation", 0.0001f);
	Renderer->Draw(TinyRendererState.GetRenderObject(0), Shader);
	Shader->Unbind();
}