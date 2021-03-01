#include "index_buffer.h"
#include "opengl_util.h"
#include "util.h"

index_buffer::index_buffer(uint32* Indices, uint32 IdxCount) : GLID(0), Count(IdxCount)
{
	Assert(sizeof(uint32) == sizeof(GLuint));
	GLCall(glGenBuffers(1, &GLID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(uint32), Indices, GL_STATIC_DRAW));
}

index_buffer::index_buffer(const mesh_data& Mesh) : GLID(0)
{
	Assert(sizeof(uint32) == sizeof(GLuint));
	Count = Mesh.Indicies.size() * 3;

	auto DataBuffer = std::make_unique<uint32[]>(Count); //new float[Size];
	for (uint32 Index = 0; Index < Mesh.Indicies.size() * 3; ++Index) {
		DataBuffer.get()[Index] = Index;
	}
	GLCall(glGenBuffers(1, &GLID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(uint32), DataBuffer.get(), GL_STATIC_DRAW));
}

index_buffer::~index_buffer()
{
	GLCall(glDeleteBuffers(1, &GLID));
}

void index_buffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLID));
}

void index_buffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
