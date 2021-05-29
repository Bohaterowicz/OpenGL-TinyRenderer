#pragma once
#include<vector>
#include<glm/glm.hpp>

#include "vertex_buffer_layout.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "mesh_data.h"




class render_object
{
private:
	glm::vec3 WorldPosition;
	shared_vertex_array_ptr MeshVertexArray;
	index_buffer MeshIndexBuffer;

public:
	render_object(shared_vertex_array_ptr VertexArray, index_buffer IndexBuffer);
	void CenterOnWorldOrigin();
	void CrateOpenGLVertexBuffer();
	void FreeOpenGLVertexBuffer();
	inline void SetWorldPosition(glm::vec3 Position) { WorldPosition = Position; }
	inline void OffsetWorldPosition(glm::vec3 Offset) { WorldPosition + Offset; }
	inline glm::vec3 GetWorldPosition() const { return WorldPosition; }
};

