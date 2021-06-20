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

glm::mat4 TranslationMtx(real32 Offset)
{
	glm::mat4 Translation(1.0f);
	const real32 fLoopDuration = 5.0f;
	const real32 fScale = 3.14159f * 2.0f / fLoopDuration;

	real32 fElapsedTime = 1.0; //glfwGetTime() / 1.0f;

	real32 fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	Translation[3].x = cosf(fCurrTimeThroughLoop * fScale) * Offset;
	Translation[3].y = sinf(fCurrTimeThroughLoop * fScale) * Offset;
	Translation[3].z = -50.0f;
	Translation[0].x = fCurrTimeThroughLoop / 4.0f;
	Translation[1].y = fCurrTimeThroughLoop / 4.0f;
	return Translation;
}

glm::mat4 RotationMtx()
{
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	const real32 fLoopDuration = 360.0f;
	real32 fElapsedTime = 1.0; //glfwGetTime() / 1.0f;
	real32 fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);
	float fAngRad = fCurrTimeThroughLoop * degToRad * 40.0f;
	float fCos = cosf(fAngRad);
	float fInvCos = 1.0f - fCos;
	float fSin = sinf(fAngRad);
	float fInvSin = 1.0f - fSin;

	glm::vec3 axis(0.0f, 1.0f, 0.0f);
	axis = glm::normalize(axis);

	glm::mat3 theMat(1.0f);
	theMat[0].x = (axis.x * axis.x) + ((1 - axis.x * axis.x) * fCos);
	theMat[1].x = axis.x * axis.y * (fInvCos) - (axis.z * fSin);
	theMat[2].x = axis.x * axis.z * (fInvCos) + (axis.y * fSin);

	theMat[0].y = axis.x * axis.y * (fInvCos) + (axis.z * fSin);
	theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
	theMat[2].y = axis.y * axis.z * (fInvCos) - (axis.x * fSin);

	theMat[0].z = axis.x * axis.z * (fInvCos) - (axis.y * fSin);
	theMat[1].z = axis.y * axis.z * (fInvCos) + (axis.x * fSin);
	theMat[2].z = (axis.z * axis.z) + ((1 - axis.z * axis.z) * fCos);
	glm::mat4 res(theMat);
	return res;
}

/*
glm::vec3 CameraOrbitPosition() {
	const real32 degToRad = 3.14159f * 2.0f / 360.0f;
	real32 phi = degToRad * CameraPolarPos.x;
	real32 theta = degToRad * (CameraPolarPos.y + 90.0f);

	real32 SinTheta = sinf(theta);
	real32 CosTheta = cosf(theta);
	real32 CosPhi = cosf(phi);
	real32 SinPhi = sinf(phi);

	glm::vec3 CameraDir(SinTheta * CosPhi, CosTheta, SinTheta * SinPhi);
	return (CameraDir * CameraPolarPos.z) + CameraTargetPos;
}
*/

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

	if (TinyRendererState.IsInitialized == FALSE)
	{
		//Create our OpenGL Renderer object, this object perform all the draw calls, and other associated functionality
		TinyRendererState.OpenGlRenderer = std::make_unique<opengl_renderer>();
		real32 AspectRatio = (real32)WindowInfo.ClientWidth / (real32)WindowInfo.ClientHeight;
		TinyRendererState.Camera = std::make_unique<base_camera>(base_camera::FrustumScaleFromFOV(CAMERA_FOV), AspectRatio, FRUSTUM_Z_NEAR, FRUSTUM_Z_FAR);
		TinyRendererState.IsInitialized = TRUE;
	}

	auto &Renderer = *TinyRendererState.OpenGlRenderer;

	{
		int x = 1;
		GLCall(glViewport(0, 0, WindowInfo.ClientWidth, WindowInfo.ClientHeight));
		auto &Camera = *TinyRendererState.Camera;

		mesh_data Mesh = ReadObjFile("../../../resources/meshes/plane_big.obj");
		vertex_buffer OBJVertexBuffer(Mesh);
		//Create an vertex buffer layout object, that specifues the layout of data in the vertex buffer
		vertex_buffer_layout OBJVertexBufferLayout;
		OBJVertexBufferLayout.AddLayoutElementF(3, FALSE); //Vertex positions
		OBJVertexBufferLayout.AddLayoutElementF(3, FALSE); //Normals
		OBJVertexBufferLayout.AddLayoutElementF(2, FALSE); //Texture coords
		vertex_array OBJVertexArray;
		OBJVertexArray.BindBufferLayout(OBJVertexBuffer, OBJVertexBufferLayout); //TODO: This maybe should happen in vertex array constructor
		index_buffer OBJIndexBuffer(Mesh);
		//render_object CubeObj(OBJVertexArray, OBJIndexBuffer);

		//Enable Blending, and specify blending mode (function)
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//We read source code from our shader files
		//std::string VertexShaderSource = ReadFile("../../../resources/shaders/test_texture_vert.glsl");
		//std::string FragmentShaderSource = ReadFile("../../../resources/shaders/checker_texture_frag.glsl");
		//We create (compile) shader during construction
		//shader_program Shader(VertexShaderSource, FragmentShaderSource);
		//Shader.Bind();

		//Renderer.CreateUniformBuffer("CameraProjectionMtxStack", 2 * sizeof(glm::mat4));
		//Renderer.SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(Camera.GetPerspectiveTransform()), sizeof(glm::mat4), sizeof(glm::mat4));
		//Renderer.BindUniformBuffer("CameraProjectionMtxStack", 1);
		//Shader.BindUniformBlock("ub_GlobalMatrices", 1);

		UnProj.ClipToCamera = glm::inverse(Camera.GetPerspectiveTransform());
		UnProj.WindowSize = glm::ivec2(WindowInfo.ClientWidth, WindowInfo.ClientHeight);

		//Renderer.CreateUniformBuffer("UnProjection", sizeof(un_proj));
		//Renderer.SetUniformBufferData("UnProjection", &UnProj, sizeof(un_proj), 0);
		//Renderer.BindUniformBuffer("UnProjection", 2);
		//Shader.BindUniformBlock("ub_UnProjection", 2);
		GLfloat maxAniso = 0.0F;
		GLCall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso));
		GLuint SamplerID;
		//GLCall(glGenSamplers(1, &SamplerID));
		//GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		//GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		//GLCall(glSamplerParameterf(SamplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso));
		//GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		//GLCall(glSamplerParameteri(SamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT));

		//std::vector<uint8> GaussianTexture;
		//BuildGaussianData(GaussianTexture, 256, 256);
		//texture2d GaussAngleTexture(&GaussianTexture[0], 256, 256, 1);

		//int32 Width, Height, BytesPerPixel;
		//uint8 *PixelBuffer = nullptr;
		//LoadPNG("../../../resources/textures/checker.png", &PixelBuffer, Width, Height, BytesPerPixel, 1);
		//texture2d CheckerTexture("../../../resources/textures/checker.png", false, 8, 1);
		//texture2d ShineTexture(PixelBuffer, Width, Height, BytesPerPixel);

		//GaussAngleTexture.Bind(0);
		//GLCall(glBindSampler(0, SamplerID));
		//Shader.SetUniform1i("gaussianTexture", 0);
		//CheckerTexture.Bind(0);
		//GLCall(glBindSampler(0, SamplerID));
		//Shader.SetUniform1i("checkerTexture", 0);

		//Shader.Unbind();

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
		Renderer.Clear();

		//Perform a draw call via our renderer
		//Shader.Bind();
		CameraRotChange = glm::vec2(Input.Mouse.dX, Input.Mouse.dY);
		Camera.ProcessInput(Input);
		Camera.UpdateForwardVector(CameraRotChange);
		//CameraRotChange = {};
		CamTransMtx = Camera.GetCmameraTransformationMatrix(); //CalcLookAtMatrix(CameraOrbitPosition(), CameraTargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
		//Renderer.SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(CamTransMtx), sizeof(glm::mat4), 0);
		//Shader.SetUniformMtx4("u_ModelTransformation", ModelTrans);
		NormalTrans = glm::mat3(CamTransMtx);
		//Used to fix normals when using non uniform scaling....
		if (invers)
		{
			NormalTrans = glm::transpose(glm::inverse(NormalTrans));
		}
		CamLightPos = (CamTransMtx * (RotationMtx() * LightPos));
		//Shader.SetUniform3f("u_CameraSpaceLight", CamLightPos);
		//Shader.SetUniform1i("u_UseSquaredAttenuationDistance", 1);
		//Shader.SetUniform1f("u_LightAttenuation", 0.0001f);
		//Renderer.Draw(OBJVertexArray, OBJIndexBuffer, Shader);
		//Shader.Unbind();

		/*
		while (!glfwWindowShouldClose(Window))
		{
			

			if (UpdateAspectRatio)
			{
				Camera.UpdateAspectRatio(AspectRatio);
				UnProj.ClipToCamera = glm::inverse(Camera.GetPerspectiveTransform());
				Renderer.SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(Camera.GetPerspectiveTransform()), sizeof(glm::mat4), sizeof(glm::mat4));
				//Renderer.SetUniformBufferData("UnProjection", &UnProj, sizeof(un_proj), 0);
				Shader.Bind();
				Shader.Unbind();
				UpdateAspectRatio = false;
			}
		}
		*/
	}
}