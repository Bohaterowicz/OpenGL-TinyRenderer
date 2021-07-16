#pragma once
#include "tiny_renderer_c.h"
#include "opengl_renderer.h"
#include "base_camera.h"
#include "mesh_data.h"
#include "render_object.h"
#include "render_material.h"
#include "shader_program.h"
#include "texture2d.h"

#include "memory"

class tiny_renderer_state
{
private:
    bool32 Initialized;
    std::unique_ptr<opengl_renderer> Renderer;
    std::unique_ptr<base_camera> Camera;
    std::vector<std::unique_ptr<render_object>> RenderObjects;
    std::vector<std::shared_ptr<shader_program>> Shaders;
    std::vector<std::shared_ptr<texture2d>> Textures;
    std::vector<std::shared_ptr<render_material>> Materials;

public:
    tiny_renderer_state();
    void SetRenderer(std::unique_ptr<opengl_renderer> NewRenderer);
    void SetCamera(std::unique_ptr<base_camera> NewCamera);
    bool32 CreateRenderObject(mesh_data &Mesh);
    bool32 AddShader(std::unique_ptr<shader_program> Shader);
    bool32 AddTexture(std::unique_ptr<texture2d> Texture);
    bool32 AddMaterial(std::unique_ptr<render_material> Material);

    inline bool32 IsInitialized() const { return Initialized; }
    inline void SetInitialized(bool32 State) { Initialized = State; }
    inline opengl_renderer *GetRenderer() const { return Renderer.get(); }
    inline base_camera *GetCamera() const { return Camera.get(); }
    inline render_object *GetRenderObject(uint32 Idx) const { return RenderObjects[Idx].get(); }
    inline std::vector<std::unique_ptr<render_object>> const *GetRenderObjects() const { return &RenderObjects; }
    inline std::shared_ptr<shader_program> GetShader(uint32 Idx) const { return Shaders[Idx]; }
    inline std::shared_ptr<render_material> GetMaterial(uint32 Idx) const { return Materials[Idx]; }
    inline std::shared_ptr<texture2d> GetTexture(uint32 Idx) const { return Textures[Idx]; }
};