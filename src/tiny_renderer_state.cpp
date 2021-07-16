#include "tiny_renderer_state.h"
#include "opengl_util.h"

tiny_renderer_state::tiny_renderer_state()
    : Initialized(FALSE), Renderer(nullptr), Camera(nullptr)
{
}

void tiny_renderer_state::SetRenderer(std::unique_ptr<opengl_renderer> NewRenderer)
{
    Renderer = std::move(NewRenderer);
}

void tiny_renderer_state::SetCamera(std::unique_ptr<base_camera> NewCamera)
{
    Camera = std::move(NewCamera);
}

bool32 tiny_renderer_state::CreateRenderObject(mesh_data &Mesh)
{
    auto OBJVertexBuffer = std::make_unique<vertex_buffer>(Mesh);
    //Create an vertex buffer layout object, that specifues the layout of data in the vertex buffer
    vertex_buffer_layout OBJVertexBufferLayout;
    OBJVertexBufferLayout.AddLayoutElementF(3, FALSE); //Vertex positions
    OBJVertexBufferLayout.AddLayoutElementF(3, FALSE); //Normals
    OBJVertexBufferLayout.AddLayoutElementF(2, FALSE); //Texture coords
    auto OBJVertexArray = std::make_unique<vertex_array>();
    OBJVertexArray->SetVertexBufferLayout(&OBJVertexBufferLayout);
    auto OBJIndexBuffer = std::make_unique<index_buffer>(Mesh);
    RenderObjects.push_back(std::make_unique<render_object>(std::move(OBJVertexBuffer), std::move(OBJVertexArray), std::move(OBJIndexBuffer)));
    GLDEBUG("CREATED OBJECT");
    return TRUE;
}

bool32 tiny_renderer_state::AddShader(std::unique_ptr<shader_program> Shader)
{
    Shaders.push_back(std::move(Shader));
    return TRUE;
}

bool32 tiny_renderer_state::AddTexture(std::unique_ptr<texture2d> Texture)
{
    Textures.push_back(std::move(Texture));
    return TRUE;
}

bool32 tiny_renderer_state::AddMaterial(std::unique_ptr<render_material> Material)
{
    Materials.push_back(std::move(Material));
    return TRUE;
}