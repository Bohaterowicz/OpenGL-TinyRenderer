#pragma once
#include<vector>
#include "tiny_renderer_c.h"

struct layout_element {
	uint32 Type;
	size_t TypeSize;
	uint32 Count;
	bool32 Normalize;
};

class vertex_buffer_layout
{
private:
	std::vector<layout_element> LayoutElements;
	size_t Stride;
public:
	vertex_buffer_layout();
	~vertex_buffer_layout();
	void AddLayoutElementF(uint32 Count, bool32 Normalize);
	void AddLayoutElementUI(uint32 Count, bool32 Normalize);
	void AddLayoutElementUB(uint32 Count, bool32 Normalize);

	inline size_t GetStride() const { return Stride; }
	inline const std::vector<layout_element>& GetElements() const { return LayoutElements; }
};

