#include "opengl_renderer.h"
#include "opengl_util.h"
#include <GL/glew.h>

opengl_renderer::opengl_renderer()
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

	GLCall(glClearColor(1.0F, 1.0F, 1.0F, 1.0F));
	GLCall(glClearDepth(1.0F));
	GLDEBUG("Specified clear colors");
}

void opengl_renderer::Draw(const vertex_array &VertexArray, const index_buffer &IndexBuffer, const shader_program &Shader) const
{
	VertexArray.Bind();
	IndexBuffer.Bind();
	Shader.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void opengl_renderer::CreateUniformBuffer(std::string Name, size_t Size, void *Data)
{
	uint32 BufferGLID;
	GLCall(glGenBuffers(1, &BufferGLID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, BufferGLID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, Size, Data, GL_STREAM_DRAW));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	uniform_buffer Buffer = {BufferGLID, Size};
	UniformBuffers[Name] = Buffer;
}

void opengl_renderer::SetUniformBufferData(std::string Name, void *Data, size_t Size, size_t BufferOffset)
{
	//TODO: Check if buffer exists in hash!!!!!!!!!!!
	uint32 GLID = UniformBuffers[Name].GLID;
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, GLID));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, BufferOffset, Size, Data));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
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

void opengl_renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
