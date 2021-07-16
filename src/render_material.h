#pragma once

#include "tiny_renderer_c.h"
#include "shader_program.h"
#include "sampler2d.h"
#include "glm/glm.hpp"
#include "texture2d.h"

#include <memory>

class render_material
{
private:
    std::shared_ptr<shader_program> Shader;
    glm::vec3 Color;
    std::shared_ptr<texture2d> ColorTexture;
    std::shared_ptr<sampler2d> ColorSampler;

    bool32 UseColorTexture;

public:
    render_material(std::shared_ptr<shader_program> _Shader, glm::vec3 _Color = glm::vec3(0.0F)) : Shader(_Shader), Color(_Color), ColorTexture(nullptr), ColorSampler(nullptr), UseColorTexture(FALSE) {}

    inline glm::vec3 GetColor() const { return Color; }
    inline shader_program *GetShader() const { return Shader.get(); }
    inline texture2d *GetTexture() const { return ColorTexture.get(); }

    inline bool32 IsUsingColorTexture() const { return UseColorTexture; }

    inline void SetColorTexture(std::shared_ptr<texture2d> CT, bool32 Use = TRUE)
    {
        ColorTexture = CT;
        UseColorTexture = Use;
    }
    inline void SetColorSampler(std::shared_ptr<sampler2d> Sampler) { ColorSampler = Sampler; }
    inline void SetColor(glm::vec3 _Color) { Color = _Color; }

    bool32 SetUseColorTexture(bool32 Use);
    bool32 ActivateTextureWithSampler(uint32 Slot);
};