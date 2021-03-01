#include "vertex_array.h"
#include<GL/glew.h>
#include "opengl_util.h"

vertex_array::vertex_array()
{
	GLCall(glGenVertexArrays(1, &GLID));
}

vertex_array::~vertex_array()
{
	GLCall(glDeleteVertexArrays(1, &GLID));
}

void vertex_array::Bind() const
{
	GLCall(glBindVertexArray(GLID));
}

void vertex_array::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void vertex_array::BindBufferLayout(vertex_buffer& Buffer, vertex_buffer_layout& BufferLayout)
{
	Bind();
	Buffer.Bind();
	const auto& Elements = BufferLayout.GetElements();
	size_t ElementOffset = 0;
	for (uint32 ElementIndex = 0; ElementIndex < Elements.size(); ++ElementIndex) {
		const auto& Element = Elements[ElementIndex];
		GLCall(glEnableVertexAttribArray(ElementIndex));
		GLCall(glVertexAttribPointer(ElementIndex, Element.Count, Element.Type, Element.Normalize, BufferLayout.GetStride(), (const void*)ElementOffset));
		ElementOffset += Element.TypeSize * Element.Count;
	}
}
