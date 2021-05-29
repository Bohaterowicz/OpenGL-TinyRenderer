#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "opengl_util.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "shader_program.h"
#include "opengl_renderer.h"
#include "texture2d.h"
#include "texture1d.h"
#include "mesh_data.h"
#include "render_object.h"
#include "base_camera.h"
#include "util.h"

struct un_proj {
	glm::mat4 ClipToCamera;
	glm::ivec2 WindowSize;
};

static real32 AspectRatio;
static bool32 UpdateAspectRatio;
static bool32 invers;
static glm::vec3 CameraPolarPos;
static glm::vec3 CameraPosChange;
static glm::vec2 CameraRotChange;

static un_proj UnProj;

void WindowSizeChangeCallback(GLFWwindow* window, int Width, int Height)
{
	std::cout << "Window size changed!" << std::endl;
	AspectRatio = (real32)Width / (real32)Height;
	UpdateAspectRatio = true;
	UnProj.WindowSize = glm::ivec2(Width, Height);
	GLCall(glViewport(0, 0, Width, Height));
}

void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		CameraPosChange.x = 1.0f;
	}
	if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
		CameraPosChange.x = -1.0f;
	}
	if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
		CameraPosChange.y = -1.0f;
	}
	if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
		CameraPosChange.y = 1.0f;
	}
	if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
		CameraPosChange.z = -1.0f;
	}
	if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
		CameraPosChange.z = 1.0f;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		std::cout << "ESCAPE..." << std::endl;
		glfwSetWindowShouldClose(window, 1);
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		std::cout << "I pressed" << std::endl;
		if (invers) {
			invers = false;
		}
		else {
			invers = true;
		}
	}
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	int W, H;
	glfwGetWindowSize(window, &W, &H);

	real32 CenterX = ((real32)W) / 2.0f;
	real32 CenterY = ((real32)H) / 2.0f;

	real32 dX = xpos - CenterX;
	real32 dY = ypos - CenterY;

	CameraRotChange = glm::vec2(dX, dY);

	glfwSetCursorPos(window, CenterX, CenterY);
}

glm::mat4 TranslationMtx(real32 Offset) {
	glm::mat4 Translation(1.0f);
	const real32 fLoopDuration = 5.0f;
	const real32 fScale = 3.14159f * 2.0f / fLoopDuration;

	real32 fElapsedTime = glfwGetTime() / 1.0f;

	real32 fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	Translation[3].x = cosf(fCurrTimeThroughLoop * fScale) * Offset;
	Translation[3].y = sinf(fCurrTimeThroughLoop * fScale) * Offset;
	Translation[3].z = -50.0f;
	Translation[0].x = fCurrTimeThroughLoop / 4.0f;
	Translation[1].y =  fCurrTimeThroughLoop / 4.0f;
	return Translation;
}

glm::mat4 RotationMtx() {
	const float degToRad = 3.14159f * 2.0f / 360.0f;
	const real32 fLoopDuration = 360.0f;
	real32 fElapsedTime = glfwGetTime() / 1.0f;
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
	theMat[1].x = axis.x * axis.y * (fInvCos)-(axis.z * fSin);
	theMat[2].x = axis.x * axis.z * (fInvCos)+(axis.y * fSin);

	theMat[0].y = axis.x * axis.y * (fInvCos)+(axis.z * fSin);
	theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
	theMat[2].y = axis.y * axis.z * (fInvCos)-(axis.x * fSin);

	theMat[0].z = axis.x * axis.z * (fInvCos)-(axis.y * fSin);
	theMat[1].z = axis.y * axis.z * (fInvCos)+(axis.x * fSin);
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

glm::mat4 CalcLookAtMatrix(const glm::vec3& cameraPt, const glm::vec3& lookPt, const glm::vec3& upPt)
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

void BuildGaussianData(std::vector<GLubyte>& TextureData, int CosAngleResolution, int ShineResolution) {
	TextureData.resize(ShineResolution * CosAngleResolution);

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

int main() {

	CameraPolarPos.x = 0.0f;
	CameraPolarPos.y = -45.0f;
	CameraPolarPos.z = 150.0f;

	GLFWwindow* Window;
	//Initialize GLFW so that we can use OpenGL functionality on our plaform
	if (!glfwInit()) {
		return -1;
	}

	//Specify "hints" as to what version of OpenGL we want to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//Specify what profile of OpenGl we want to use, we want to use 'core'
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//We use GLFW library to create a windows window with attached OpenGL context
	Window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!Window) {
		glfwTerminate();
		return -1;
	}

	//Make our windows context the current OpenGL context (in case we have multiple windows)
	glfwMakeContextCurrent(Window);
	//We set swap buffers interval to be 1, meaning swap buffer once every monitor refresh rate
	glfwSwapInterval(1);

	//Initi GL Extension Wrangler (GLEW), in order to get access to all available OpenGL functions on our machine (platform)
	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
		return -1;
	}
	else {
		std::cout << glGetString(GL_VERSION) << std::endl;
		//Set window specific callbacks
		glfwSetWindowSizeCallback(Window, WindowSizeChangeCallback);
		glfwSetKeyCallback(Window, KeyboardInputCallback);
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(Window, CursorPosCallback);
	}

	//Create our OpenGL Renderer object, this object perform all the draw calls, and other associated functionality
	opengl_renderer Renderer;

	{

		mesh_data Mesh = ReadObjFile("../../../resources/meshes/plane_big.obj");
		vertex_buffer OBJVertexBuffer(Mesh);
		//Create an vertex buffer layout object, that specifues the layout of data in the vertex buffer
		vertex_buffer_layout OBJVertexBufferLayout;
		OBJVertexBufferLayout.AddLayoutElementF(3, false); //Vertex positions
		OBJVertexBufferLayout.AddLayoutElementF(3, false); //Normals
		OBJVertexBufferLayout.AddLayoutElementF(2, false); //Texture coords
		vertex_array OBJVertexArray;
		OBJVertexArray.BindBufferLayout(OBJVertexBuffer, OBJVertexBufferLayout); //TODO: This maybe should happen in vertex array constructor
		index_buffer OBJIndexBuffer(Mesh);
		//render_object CubeObj(OBJVertexArray, OBJIndexBuffer);


		//Enable Blending, and specify blending mode (function)
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	

		//We read source code from our shader files
		std::string VertexShaderSource = ReadFile("../../../resources/shaders/test_texture_vert.glsl");
		std::string FragmentShaderSource = ReadFile("../../../resources/shaders/checker_texture_frag.glsl");
		//We create (compile) shader during construction
		shader_program Shader(VertexShaderSource, FragmentShaderSource);
		Shader.Bind();
		
		real32 zNear = 0.1f;
		real32 zFar = 100000.0f;
		int32 W, H;
		glfwGetWindowSize(Window, &W, &H);
		AspectRatio = (real32)W / (real32)H;
		base_camera Camera(base_camera::FrustumScaleFromFOV(45.0f), AspectRatio, zNear, zFar);
		real32 CenterX = ((real32)W) / 2.0f;
		real32 CenterY = ((real32)H) / 2.0f;
		glfwSetCursorPos(Window, CenterX, CenterY);

		Renderer.CreateUniformBuffer("CameraProjectionMtxStack", 2*sizeof(glm::mat4));
		Renderer.SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(Camera.GetPerspectiveTransform()), sizeof(glm::mat4), sizeof(glm::mat4));
		Renderer.BindUniformBuffer("CameraProjectionMtxStack", 1);
		Shader.BindUniformBlock("ub_GlobalMatrices", 1);

		UnProj.ClipToCamera = glm::inverse(Camera.GetPerspectiveTransform());
		UnProj.WindowSize = glm::ivec2(640, 480);

		//Renderer.CreateUniformBuffer("UnProjection", sizeof(un_proj));
		//Renderer.SetUniformBufferData("UnProjection", &UnProj, sizeof(un_proj), 0);
		//Renderer.BindUniformBuffer("UnProjection", 2);
		//Shader.BindUniformBlock("ub_UnProjection", 2);
		GLfloat maxAniso = 0.0f;
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

		int32 Width, Height, BytesPerPixel;
		uint8* PixelBuffer = nullptr;
		//LoadPNG("../../../resources/textures/checker.png", &PixelBuffer, Width, Height, BytesPerPixel, 1);
		texture2d CheckerTexture("../../../resources/textures/checker.png", false, 8, 1);
		//texture2d ShineTexture(PixelBuffer, Width, Height, BytesPerPixel);

		//GaussAngleTexture.Bind(0);
		//GLCall(glBindSampler(0, SamplerID));
		//Shader.SetUniform1i("gaussianTexture", 0);
		CheckerTexture.Bind(0);
		GLCall(glBindSampler(0, SamplerID));
		Shader.SetUniform1i("checkerTexture", 0);

		

		Shader.Unbind();

		//Main loop
		float OffsetX = 0.0f;
		float OffsetY = 0.0f;

		CameraPosChange = {};

		glm::mat4 BaseTranslation(1.0f);
		BaseTranslation[3].z = -50.0f;
		glm::mat4 CamTransMtx;
		glm::mat4 ModelTrans(1.0f);
		glm::mat3 NormalTrans;
		glm::vec3 CamLightPos;
		glm::vec4 LightPos(55.0f, 40.0f, 0.0f, 1.0f);
		while (!glfwWindowShouldClose(Window)) {
			Renderer.Clear();
			

			if (UpdateAspectRatio) {
				Camera.UpdateAspectRatio(AspectRatio);
				UnProj.ClipToCamera = glm::inverse(Camera.GetPerspectiveTransform());
				Renderer.SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(Camera.GetPerspectiveTransform()), sizeof(glm::mat4), sizeof(glm::mat4));
				//Renderer.SetUniformBufferData("UnProjection", &UnProj, sizeof(un_proj), 0);
				Shader.Bind();
				Shader.Unbind();
				UpdateAspectRatio = false;
			}
			
			//Perform a draw call via our renderer
			Shader.Bind();
			Camera.UpdatePosition(CameraPosChange);
			Camera.UpdateForwardVector(CameraRotChange);
			CameraRotChange = {};
			CameraPosChange = {};
			CamTransMtx = Camera.GetCmameraTransformationMatrix(); //CalcLookAtMatrix(CameraOrbitPosition(), CameraTargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
			Renderer.SetUniformBufferData("CameraProjectionMtxStack", glm::value_ptr(CamTransMtx), sizeof(glm::mat4), 0);
			//Shader.SetUniformMtx4("u_ModelTransformation", ModelTrans);
			NormalTrans = glm::mat3(CamTransMtx);
			//Used to fix normals when using non uniform scaling....
			if (invers) {
				NormalTrans = glm::transpose(glm::inverse(NormalTrans));
			}
			CamLightPos = (CamTransMtx * (RotationMtx() * LightPos));
			Shader.SetUniform3f("u_CameraSpaceLight", CamLightPos);
			Shader.SetUniform1i("u_UseSquaredAttenuationDistance", 1);
			Shader.SetUniform1f("u_LightAttenuation", 0.0001f);
			Renderer.Draw(OBJVertexArray, OBJIndexBuffer, Shader);
			Shader.Unbind();
			//Swap buffers will display current framebuffer to the screen (window)
			glfwSwapBuffers(Window);
			glfwPollEvents();
		}
	}
	
	glfwTerminate();
	return 0;
}