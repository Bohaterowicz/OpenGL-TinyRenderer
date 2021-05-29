#pragma once
#include<GL/glew.h>
#include<iostream>
#include "util.h"
#include "tiny_renderer_c.h"


#define GLCall(x) GLClearError(); x; Assert(GLLogCall(#x, __FILE__, __LINE__));


inline void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* File, int line) {
	bool32 err = true;
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error (" << error << "): " << File << " -> " << line << " -> " << function << std::endl;
		err = false;
	}
	return err;
}