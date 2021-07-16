#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "shader_program.h"
#include "vertex_buffer_layout.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "mesh_data.h"
#include "render_material.h"

class render_object
{
private:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::mat4 ModelTransform;
	std::unique_ptr<vertex_array> MeshVertexArray;
	std::unique_ptr<index_buffer> MeshIndexBuffer;
	std::unique_ptr<vertex_buffer> MeshVertexBuffer;

	std::shared_ptr<render_material> Material;

public:
	render_object(std::unique_ptr<vertex_buffer> VertexBuffer, std::unique_ptr<vertex_array> VertexArray, std::unique_ptr<index_buffer> IndexBuffer);
	void ComputeModelTransform();
	inline void CenterOnWorldOrigin() { Position = glm::vec3(0.0F, 0.0F, 0.0F); };
	inline void SetPosition(glm::vec3 Position_) { Position = Position_; }
	inline void SetRotation(glm::vec3 Rotation_) { Rotation = Rotation_; }
	inline void SetScale(glm::vec3 Scale_) { Scale = Scale_; }
	inline void SetMaterial(std::shared_ptr<render_material> _Material) { Material = _Material; }
	inline void OffsetPosition(glm::vec3 Offset) { Position + Offset; }
	inline glm::vec3 GetWPosition() const { return Position; }
	inline glm::mat4 GetModelTransform() const { return ModelTransform; }
	inline vertex_array *GetVertexArray() const { return MeshVertexArray.get(); }
	inline index_buffer *GetIndexBuffer() const { return MeshIndexBuffer.get(); }
	inline vertex_buffer *GetVertexBuffer() const { return MeshVertexBuffer.get(); }
	inline render_material *GetMaterial() const { return Material.get(); }
};
