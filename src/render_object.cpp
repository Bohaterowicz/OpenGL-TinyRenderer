#include "render_object.h"

render_object::render_object(shared_vertex_array_ptr VertexArray, index_buffer IndexBuffer) : MeshVertexArray(VertexArray), MeshIndexBuffer(IndexBuffer), WorldPosition(glm::vec3(0.0f))
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

