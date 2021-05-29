#include "vertex_buffer_layout.h"
#include<GL/glew.h>

vertex_buffer_layout::vertex_buffer_layout() : Stride(0)
{
}

vertex_buffer_layout::~vertex_buffer_layout()
{
}

void vertex_buffer_layout::AddLayoutElementF(uint32 Count, bool32 Normalize)
{
	LayoutElements.push_back({ GL_FLOAT, sizeof(GLfloat), Count, Normalize });
	Stride += sizeof(GLfloat) * Count;
}

void vertex_buffer_layout::AddLayoutElementUI(uint32 Count, bool32 Normalize)
{
	LayoutElements.push_back({ GL_UNSIGNED_INT, sizeof(GLuint), Count, Normalize });
	Stride += sizeof(GLuint) * Count;
}

void vertex_buffer_layout::AddLayoutElementUB(uint32 Count, bool32 Normalize)
{
	LayoutElements.push_back({ GL_UNSIGNED_BYTE, sizeof(GLubyte), Count, Normalize });
	Stride += sizeof(GLubyte) * Count;
}
