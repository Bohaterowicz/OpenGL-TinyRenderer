#pragma once
#include "tiny_renderer_c.h"
#include "mesh_data.h"

class index_buffer
{
private:
	uint32 GLID;
	uint32 Count;

public:
	index_buffer(uint32 *Indices, uint32 Count);
	index_buffer(const mesh_data &Mesh);
	~index_buffer();
	void Bind() const;
	void Unbind() const;

	inline uint32 GetID() const { return GLID; }
	inline uint32 GetCount() const { return Count; }
};
