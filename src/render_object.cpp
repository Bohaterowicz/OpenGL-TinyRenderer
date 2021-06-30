#include "render_object.h"
#include "quaternion.h"

render_object::render_object(std::unique_ptr<vertex_buffer> VertexBuffer, std::unique_ptr<vertex_array> VertexArray, std::unique_ptr<index_buffer> IndexBuffer)
	: Position(glm::vec3(0.0F)), Rotation(glm::vec3(0.0F)), Scale(glm::vec3(1.0F)),
	  MeshVertexArray(std::move(VertexArray)), MeshIndexBuffer(std::move(IndexBuffer)), MeshVertexBuffer(std::move(VertexBuffer))
{
	//TODO: COnsider (in the future) crating opengl vertex buffer in constructor, instead having a seperate call??
	ComputeModelTransform();
}

void render_object::ComputeModelTransform()
{
	auto RotationQuat = quaternion(Rotation);
	auto RotationTransform = RotationQuat.ToRotationMatrix();

	auto PositionTransform = glm::mat4(1.0F);
	PositionTransform[3][0] = Position.x;
	PositionTransform[3][1] = Position.y;
	PositionTransform[3][2] = Position.z;

	auto ScaleTransform = glm::mat4(1.0F);
	ScaleTransform[0][0] *= Scale.x;
	ScaleTransform[1][1] *= Scale.y;
	ScaleTransform[2][2] *= Scale.z;

	ModelTransform = PositionTransform * RotationTransform * ScaleTransform;
}