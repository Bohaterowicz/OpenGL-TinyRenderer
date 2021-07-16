#pragma once
#include <string>
#include <unordered_map>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader_program.h"
#include "render_object.h"
#include "opengl_util.h"
#include <GL/glew.h>

struct uniform_buffer
{
	uint32 GLID;
	size_t Size;
};

class opengl_renderer
{
private:
	std::unordered_map<std::string, uniform_buffer> UniformBuffers;
	bool32 WireframeActive;
	std::shared_ptr<shader_program> WireframeShader;

public:
	opengl_renderer();
	static void SetViewportDimensions(uint32 X, uint32 Y, uint32 Width, uint32 Height);
	static void EnableAlphaBlending();
	static void Clear();
	void Draw(std::vector<std::unique_ptr<render_object>> const *RenderObjects);
	void CreateUniformBuffer(std::string Name, size_t Size, void *Data = NULL);
	void SetUniformBufferData(std::string Name, void *Data, size_t Size, size_t BufferOffset);
	void BindUniformBuffer(std::string Name, uint32 BindIndex, size_t Size = 0, size_t Offset = 0);

	inline void SetWireframeShader(std::shared_ptr<shader_program> Shader) { WireframeShader = Shader; }

	inline void SwitchWireframeMode()
	{
		if (WireframeActive == TRUE)
		{
			WireframeActive = FALSE;
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}
		else
		{
			WireframeActive = TRUE;
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}
	}
};
