#pragma once
#include <GL/glew.h>
#include <iostream>
#include "util.h"
#include "tiny_renderer_c.h"

#ifdef DEGUB
#define GLCall(x)   \
	GLClearError(); \
	x;              \
	Assert(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) \
	x;
#endif

#ifdef DEBUG
#define GLDEBUG(msg) \
	std::cout << "GL_DEBUG:	" << (msg) << std::endl
#else
#define GLDEBUG(msg)
#endif

inline void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

inline bool32 GLLogCall(const char *Function, const char *File, int Line)
{
	bool32 Err = TRUE;
	while (GLenum Error = glGetError())
	{
		std::cout << "OpenGL Error (" << Error << "): " << File << " -> " << Line << " -> " << Function << std::endl;
		Err = FALSE;
	}
	return Err;
}