#include "texture1d.h"
#include "opengl_util.h"

texture1d::texture1d(uint8* Data, int32 Resolution) : GLID(0), PixelBuffer(nullptr), Width(0), Height(0), BytesPerPixel(0)
{
	PixelBuffer = Data;
	GLCall(glGenTextures(1, &GLID));
	GLCall(glBindTexture(GL_TEXTURE_1D, GLID));
    GLCall(glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, Resolution, 0, GL_RED, GL_UNSIGNED_BYTE, PixelBuffer));
    GLCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0));
    GLCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0));
    GLCall(glBindTexture(GL_TEXTURE_1D, 0));
}

texture1d::~texture1d()
{
    GLCall(glDeleteTextures(1, &GLID));
}

void texture1d::Bind(uint32 BindSlot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + BindSlot));
    GLCall(glBindTexture(GL_TEXTURE_1D, GLID));
}

void texture1d::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_1D, 0));
}
