#include "opengl_renderer.h"
#include <glm/gtc/type_ptr.hpp>

opengl_renderer::opengl_renderer() : WireframeActive(FALSE)
{
	GLDEBUG("Initialize OpenGL Renderer object");

	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	GLCall(glFrontFace(GL_CCW));
	GLDEBUG("Backface cull enabled");

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glDepthRange(0.0F, 1.0F));
	GLDEBUG("Enabled depth testing");

	GLCall(glClearColor(0.7F, 0.7F, 0.7F, 1.0F));
	GLCall(glClearDepth(1.0F));
	GLDEBUG("Specified clear colors");
}

void opengl_renderer::SetViewportDimensions(uint32 X, uint32 Y, uint32 Width, uint32 Height)
{
	GLCall(glViewport(X, Y, Width, Height));
}

void opengl_renderer::EnableAlphaBlending()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void opengl_renderer::Draw(const std::vector<std::unique_ptr<render_object>> *RenderObjects, const base_camera *Camera)
{
	auto CamTransMtx = Camera->GetCmameraTransformationMatrix();
	SetUniformBufferData("MVPMtxStack", glm::value_ptr(CamTransMtx), sizeof(glm::mat4), sizeof(glm::mat4));
	shader_program *CurrentShader = WireframeShader.get();
	CurrentShader->Bind();

	render_material *CurrentMaterial = nullptr;

	for (const auto &Object : *RenderObjects)
	{
		if (WireframeActive == FALSE)
		{
			CurrentMaterial = Object->GetMaterial();
			auto *MaterialShader = CurrentMaterial->GetShader();
			if (MaterialShader != CurrentShader)
			{
				CurrentShader = MaterialShader;
				CurrentShader->Bind();
			}
			auto MatColor = CurrentMaterial->GetColor();
			auto SpecularStrength = CurrentMaterial->GetSpecularStrength();
			auto UseColorTexture = CurrentMaterial->IsUsingColorTexture();
			SetUniformBufferData("BaseMaterial", glm::value_ptr(MatColor), sizeof(glm::vec3), 0);
			SetUniformBufferData("BaseMaterial", &SpecularStrength, sizeof(real32), sizeof(glm::vec3));
			SetUniformBufferData("BaseMaterial", &UseColorTexture, sizeof(bool32), sizeof(glm::vec3) + 4);
			if (UseColorTexture == TRUE)
			{
				CurrentMaterial->ActivateTextureWithSampler(0);
				CurrentShader->SetUniform1i("ColorTextureSampler", 0);
			}
			CurrentShader->SetUniform3f("LightPos", glm::vec3(10.0F, 1000.0F, 10.0F));
			auto ViewPosVec = Camera->GetPosition();
			CurrentShader->SetUniform3f("ViewPos", ViewPosVec);
		}

		Object->ComputeModelTransform();
		glm::mat4 ModelTransform = Object->GetModelTransform();
		SetUniformBufferData("MVPMtxStack", glm::value_ptr(ModelTransform), sizeof(glm::mat4), 0);
		BindUniformBuffer("MVPMtxStack", 0);
		BindUniformBuffer("BaseMaterial", 1);
		Object->GetVertexBuffer()->Bind();
		Object->GetVertexArray()->Bind();
		Object->GetIndexBuffer()->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, Object->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	CurrentShader->Unbind();
}

void opengl_renderer::CreateUniformBuffer(std::string Name, size_t Size, void *Data)
{
	uint32 BufferGLID;
	GLCall(glCreateBuffers(1, &BufferGLID));
	GLCall(glNamedBufferStorage(BufferGLID, Size, Data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT));
	uniform_buffer Buffer = {BufferGLID, Size};
	UniformBuffers[Name] = Buffer;
}

void opengl_renderer::SetUniformBufferData(std::string Name, void *Data, size_t Size, size_t BufferOffset)
{
	//TODO: Check if buffer exists in hash!!!!!!!!!!!
	uint32 GLID = UniformBuffers[Name].GLID;
	GLCall(glNamedBufferSubData(GLID, BufferOffset, Size, Data));
}

void opengl_renderer::BindUniformBuffer(std::string Name, uint32 BindIndex, size_t Size, size_t Offset)
{
	//TODO: Check if buffer exists in hash!!!!!!!!!!!
	uniform_buffer Buffer = UniformBuffers[Name];

	//If size was not specified by the caller, then we use the total stored size of the buffer
	//Also if user did not specify his own size, and thus we use total buffer size, we cant use any offsets as we would overflow...
	if (Size == 0)
	{
		Size = Buffer.Size;
		Assert(Offset == 0); // This could become simply Offset = 0, but maybe that would not be so smart? (better to assert for now I guess)
	}

	GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, BindIndex, Buffer.GLID, Offset, Size));
}

void opengl_renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
