#pragma once
#include<memory>

#include "tiny_renderer_c.h"
#include "mesh_data.h"

class vertex_buffer
{
private:
	uint32 GLID;
public:
	vertex_buffer(const void* Data, size_t size);
	vertex_buffer(const mesh_data& Data);
	~vertex_buffer();
	inline uint32 GetId() const { return GLID; }
	void Bind() const;
	void Unbind() const;
};

typedef std::shared_ptr<vertex_buffer> shared_vertex_buffer_ptr;

