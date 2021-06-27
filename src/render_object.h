#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "vertex_buffer_layout.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "mesh_data.h"

class render_object
{
private:
	glm::vec3 WorldPosition;
	std::unique_ptr<vertex_array> MeshVertexArray;
	std::unique_ptr<index_buffer> MeshIndexBuffer;
	std::unique_ptr<vertex_buffer> MeshVertexBuffer;

public:
	render_object(std::unique_ptr<vertex_buffer> VertexBuffer, std::unique_ptr<vertex_array> VertexArray, std::unique_ptr<index_buffer> IndexBuffer);
	void CenterOnWorldOrigin();
	void CrateOpenGLVertexBuffer();
	void FreeOpenGLVertexBuffer();
	inline void SetWorldPosition(glm::vec3 Position) { WorldPosition = Position; }
	inline void OffsetWorldPosition(glm::vec3 Offset) { WorldPosition + Offset; }
	inline glm::vec3 GetWorldPosition() const { return WorldPosition; }
	inline vertex_array *GetVertexArray() const { return MeshVertexArray.get(); }
	inline index_buffer *GetIndexBuffer() const { return MeshIndexBuffer.get(); }
	inline vertex_buffer *GetVertexBuffer() const { return MeshVertexBuffer.get(); }
};
