#pragma once
#include "tiny_renderer_c.h"
#include "opengl_util.h"

class sampler2d
{
private:
    uint32 GLID;

public:
    sampler2d();
    void Bind(uint32 Slot) const;
    static void UnBind(uint32 Slot);
    void SetFilteringMode(GLenum MinFilter, GLenum MagFilter) const;
    void SetWrapMode(GLenum WrapS, GLenum WrapT) const;
    void SetAnisotropy(real32 AnisotropyLevel) const;
    static real32 GetMaxAnisotropy();
    ~sampler2d();
};