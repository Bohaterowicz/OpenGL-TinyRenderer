#pragma once
#include<string>
#include<unordered_map>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader_program.h"

struct uniform_buffer {
	uint32 GLID;
	size_t Size;
};

class opengl_renderer
{
private:
	std::unordered_map<std::string, uniform_buffer> UniformBuffers;
public:
	opengl_renderer();
	void Draw(const vertex_array& VertexArray, const index_buffer& IndexBuffer, const shader_program& Shader) const;
	void CreateUniformBuffer(std::string Name, size_t Size, void* Data = nullptr);
	void SetUniformBufferData(std::string Name, void* Data, size_t Size, size_t BufferOffset);
	void BindUniformBuffer(std::string Name, uint32 BindIndex, size_t Size = 0, size_t Offset = 0);
	void Clear() const;
};

