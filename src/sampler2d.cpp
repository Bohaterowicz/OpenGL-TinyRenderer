#include "sampler2d.h"

sampler2d::sampler2d()
{
    GLCall(glGenSamplers(1, &GLID));
}

void sampler2d::Bind(uint32 Slot) const
{
    GLCall(glBindSampler(Slot, GLID));
}

void sampler2d::UnBind(uint32 Slot)
{
    GLCall(glBindSampler(Slot, 0));
}

void sampler2d::SetFilteringMode(GLenum MinFilter, GLenum MagFilter) const
{
    GLCall(glSamplerParameteri(GLID, GL_TEXTURE_MIN_FILTER, MinFilter));
    GLCall(glSamplerParameteri(GLID, GL_TEXTURE_MAG_FILTER, MagFilter));
}

void sampler2d::SetWrapMode(GLenum WrapS, GLenum WrapT) const
{
    GLCall(glSamplerParameteri(GLID, GL_TEXTURE_WRAP_S, WrapS));
    GLCall(glSamplerParameteri(GLID, GL_TEXTURE_WRAP_T, WrapT));
}

void sampler2d::SetAnisotropy(real32 AnisotropyLevel) const
{
    GLCall(glSamplerParameterf(GLID, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<GLfloat>(AnisotropyLevel)));
}

real32 sampler2d::GetMaxAnisotropy()
{
    GLfloat MaxAniso = 0.0F;
    GLCall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAniso));
    return static_cast<real32>(MaxAniso);
}

sampler2d::~sampler2d()
{
    GLCall(glDeleteSamplers(1, &GLID));
}