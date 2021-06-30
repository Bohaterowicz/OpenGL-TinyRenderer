#pragma once
#include "tiny_renderer_c.h"
#include "opengl_renderer.h"
#include "base_camera.h"
#include "mesh_data.h"
#include "render_object.h"
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
    std::vector<std::unique_ptr<shader_program>> Shaders;
    std::vector<std::unique_ptr<texture2d>> Textures;

public:
    tiny_renderer_state();
    void SetRenderer(std::unique_ptr<opengl_renderer> NewRenderer);
    void SetCamera(std::unique_ptr<base_camera> NewCamera);
    bool32 CreateRenderObject(mesh_data &Mesh);
    bool32 AddShader(std::unique_ptr<shader_program> Shader);
    bool32 AddTexture(std::unique_ptr<texture2d> Texture);

    inline bool32 IsInitialized() const { return Initialized; }
    inline void SetInitialized(bool32 State) { Initialized = State; }
    inline opengl_renderer *GetRenderer() const { return Renderer.get(); }
    inline base_camera *GetCamera() const { return Camera.get(); }
    inline render_object *GetRenderObject(uint32 Idx) const { return RenderObjects[Idx].get(); }
    inline std::vector<std::unique_ptr<render_object>> const *GetRenderObjects() const { return &RenderObjects; }
    inline shader_program *GetShader(uint32 Idx) const { return Shaders[Idx].get(); }
};