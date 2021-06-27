#pragma once
#include <memory>

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class vertex_array
{
private:
	uint32 GLID;

public:
	vertex_array();
	~vertex_array();
	void Bind() const;
	void Unbind() const;

	void SetVertexBufferLayout(vertex_buffer_layout *BufferLayout);
};

typedef std::shared_ptr<vertex_array> shared_vertex_array_ptr;
