#include "vertex_buffer.h"
#include "opengl_util.h"
#include<GL/glew.h>

#include<glm.hpp>
#include "glm/gtc/type_ptr.hpp"

vertex_buffer::vertex_buffer(const void* Data, size_t Size) : GLID(0)
{
	GLCall(glGenBuffers(1, &GLID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, GLID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, Size, Data, GL_STATIC_DRAW));
}

vertex_buffer::vertex_buffer(const mesh_data& Data) : GLID(0)
{
	//Loop over all faces, as we need to specify normals for each vertex for each face that vertex is associated with
	//TODO: Right now I hardcode the buffer size to be 3 position floats, and 3 normals floats (6), this could be comptued from data stored in mesh_data in the future
	size_t Count = 6 * Data.Indicies.size() * 3;
	auto DataBuffer = std::make_unique<real32[]>(Count); //new float[Size];
	uint32 DataIndex = 0;
	for (auto& Face : Data.Indicies) {
		for (uint32 TriIndex = 0; TriIndex < 3; ++TriIndex) {
			//TODO: SERIOUSLY I should just pack the data into a continous buffer on load from obj, so we dont have to create this temporary buffer here maybe? Or maybe not, we will se...
			std::memcpy(DataBuffer.get() + DataIndex, glm::value_ptr(Data.Position[Face.vert_idx[TriIndex]]), sizeof(glm::vec3));
			DataIndex += 3;
			std::memcpy(DataBuffer.get() + DataIndex, glm::value_ptr(Data.Normals[Face.normal_idx[TriIndex]]), sizeof(glm::vec3));
			DataIndex += 3;
		}
	}
	GLCall(glGenBuffers(1, &GLID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, GLID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, Count * sizeof(real32), DataBuffer.get(), GL_STATIC_DRAW));
}


vertex_buffer::~vertex_buffer()
{
	GLCall(glDeleteBuffers(1, &GLID));
	GLID = 0;
}

void vertex_buffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, GLID));
}

void vertex_buffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
