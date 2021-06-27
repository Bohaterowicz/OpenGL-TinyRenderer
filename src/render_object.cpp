#include "render_object.h"

render_object::render_object(std::unique_ptr<vertex_buffer> VertexBuffer, std::unique_ptr<vertex_array> VertexArray, std::unique_ptr<index_buffer> IndexBuffer)
	: WorldPosition(glm::vec3(0.0F)), MeshVertexArray(std::move(VertexArray)), MeshIndexBuffer(std::move(IndexBuffer)), MeshVertexBuffer(std::move(VertexBuffer))
{
	//TODO: COnsider (in the future) crating opengl vertex buffer in constructor, instead having a seperate call??
}

void render_object::CenterOnWorldOrigin()
{
}

void render_object::CrateOpenGLVertexBuffer()
{
}

void render_object::FreeOpenGLVertexBuffer()
{
}
