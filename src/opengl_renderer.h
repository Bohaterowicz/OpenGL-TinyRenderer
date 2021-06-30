#pragma once
#include <string>
#include <unordered_map>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader_program.h"
#include "render_object.h"

struct uniform_buffer
{
	uint32 GLID;
	size_t Size;
};

class opengl_renderer
{
private:
	std::unordered_map<std::string, uniform_buffer> UniformBuffers;

public:
	opengl_renderer();
	static void SetViewportDimensions(uint32 X, uint32 Y, uint32 Width, uint32 Height);
	static void EnableAlphaBlending();
	void Draw(std::vector<std::unique_ptr<render_object>> const *RenderObjects, const shader_program *Shader);
	void CreateUniformBuffer(std::string Name, size_t Size, void *Data = nullptr);
	void SetUniformBufferData(std::string Name, void *Data, size_t Size, size_t BufferOffset);
	void BindUniformBuffer(std::string Name, uint32 BindIndex, size_t Size = 0, size_t Offset = 0);
	void Clear() const;
};
