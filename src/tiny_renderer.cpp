#include "tiny_renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../glimg/glimage.h"

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
#include "render_material.h"
#include "sampler2d.h"
#include "util.h"

/////////CAMERA DEFINES////////

#define FRUSTUM_Z_NEAR 0.01f
#define FRUSTUM_Z_FAR 100000.f
#define CAMERA_FOV 45.0f

///////////////////////////////

void UpdateAndRender(tiny_renderer_window_info &WindowInfo, tiny_renderer_state &TinyRendererState, tiny_renderer_input &Input)
{

	if (TinyRendererState.IsInitialized() == FALSE)
	{
		//Create our OpenGL Renderer object, this object perform all the draw calls, and other associated functionality
		TinyRendererState.SetRenderer(std::make_unique<opengl_renderer>());
		real32 AspectRatio = (real32)WindowInfo.ClientWidth / (real32)WindowInfo.ClientHeight;
		TinyRendererState.SetCamera(std::make_unique<base_camera>(base_camera::FrustumScaleFromFOV(CAMERA_FOV), AspectRatio, FRUSTUM_Z_NEAR, FRUSTUM_Z_FAR));
		mesh_data Mesh1 = ReadObjFile("../../../resources/meshes/plane_big.obj");
		mesh_data Mesh2 = ReadObjFile("../../../resources/meshes/knot.obj");
		TinyRendererState.CreateRenderObject(Mesh1);
		TinyRendererState.CreateRenderObject(Mesh2);

		//Enable Blending, and specify blending mode (function)
		opengl_renderer::EnableAlphaBlending();

		glm::vec3 Pos(-600.0F, 300.0F, 0.0F);
		glm::vec3 Scale(3.0F);
		glm::vec3 Rot(glm::radians(90.0F), 0.0F, 0.0F);
		auto *Obj = TinyRendererState.GetRenderObject(1);
		Obj->SetPosition(Pos);
		Obj->SetScale(Scale);
		Obj->SetRotation(Rot);

		Input = {};

		//We read source code from our shader files
		std::string VertexShaderSource = ReadFile("../../../resources/shaders/base_mat_vert.glsl");
		std::string FragmentShaderSource = ReadFile("../../../resources/shaders/base_mat_frag.glsl");
		//We create (compile) shader during construction
		auto Shader = std::make_unique<shader_program>(VertexShaderSource, FragmentShaderSource);
		Shader->Bind();
		Shader->BindUniformBlock("ub_MVP", 0);
		Shader->BindUniformBlock("ub_BaseMaterial", 1);
		Shader->Unbind();
		TinyRendererState.AddShader(std::move(Shader));

		FragmentShaderSource = ReadFile("../../../resources/shaders/wireframe_fragment.glsl");
		auto Shader2 = std::make_unique<shader_program>(VertexShaderSource, FragmentShaderSource);
		Shader2->Bind();
		Shader2->BindUniformBlock("ub_MVP", 0);
		Shader2->Unbind();
		TinyRendererState.AddShader(std::move(Shader2));

		TinyRendererState.GetRenderer()->SetWireframeShader(TinyRendererState.GetShader(1));

		TinyRendererState.GetRenderer()->CreateUniformBuffer("MVPMtxStack", 3 * sizeof(glm::mat4));
		TinyRendererState.GetRenderer()->CreateUniformBuffer("BaseMaterial", sizeof(glm::vec3) + 4 + 4);

		auto CheckerTexture = std::make_unique<texture2d>("../../../resources/textures/coronodds.dds");

		TinyRendererState.AddTexture(std::move(CheckerTexture));

		auto Material_1 = std::make_unique<render_material>(TinyRendererState.GetShader(0), glm::vec3(0.3, 0.9, 0.1));
		auto Material_2 = std::make_unique<render_material>(TinyRendererState.GetShader(0), glm::vec3(0.9, 0.3, 0.6));
		Material_1->SetColorTexture(TinyRendererState.GetTexture(0));
		Material_1->SetUseColorTexture(TRUE);
		Material_1->SetSpecularStrength(0.2F);

		auto Sampler_1 = std::make_unique<sampler2d>();
		Sampler_1->SetAnisotropy(sampler2d::GetMaxAnisotropy());
		Sampler_1->SetFilteringMode(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		Sampler_1->SetWrapMode(GL_REPEAT, GL_REPEAT);

		Material_1->SetColorSampler(std::move(Sampler_1));

		Material_2->SetSpecularStrength(0.9F);

		TinyRendererState.AddMaterial(std::move(Material_1));
		TinyRendererState.AddMaterial(std::move(Material_2));

		TinyRendererState.GetRenderObject(0)->SetMaterial(TinyRendererState.GetMaterial(0));
		TinyRendererState.GetRenderObject(1)->SetMaterial(TinyRendererState.GetMaterial(1));

		/*
		for (const auto &Obj : *(TinyRendererState.GetRenderObjects()))
		{
			Obj->SetMaterial(TinyRendererState.GetMaterial(0));
		}
		*/

		TinyRendererState.GetRenderer()->SetUniformBufferData("MVPMtxStack", glm::value_ptr(TinyRendererState.GetCamera()->GetPerspectiveTransform()), sizeof(glm::mat4), 2 * sizeof(glm::mat4));
		TinyRendererState.SetInitialized(TRUE);
	}

	auto *const Renderer = TinyRendererState.GetRenderer();
	auto *const Camera = TinyRendererState.GetCamera();

	opengl_renderer::SetViewportDimensions(0, 0, WindowInfo.ClientWidth, WindowInfo.ClientHeight);

	if (Input.Controller[0].ActionWireframe.HalfTransisionCount > 0 && Input.Controller[0].ActionWireframe.EndedDown)
	{
		Renderer->SwitchWireframeMode();
		GLDEBUG("Switched wireframe mode");
	}

	//Main loop
	glm::mat4 BaseTranslation(1.0f);
	BaseTranslation[3].z = -50.0f;
	glm::mat4 CamTransMtx;
	glm::mat3 NormalTrans;
	glm::vec3 CamLightPos;
	glm::vec4 LightPos(55.0f, 40.0f, 0.0f, 1.0f);
	opengl_renderer::Clear();

	Camera->ProcessInput(Input);

	Renderer->Draw(TinyRendererState.GetRenderObjects(), Camera);
}