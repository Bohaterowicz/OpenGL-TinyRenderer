#pragma once
#include "tiny_renderer_c.h"

class texture1d
{
private:
	uint32 GLID;
	uint8* PixelBuffer;
	int32 Width;
	int32 Height;
	int32 BytesPerPixel;
public:
	texture1d(uint8* Data, int32 Resolution);
	~texture1d();

	void Bind(uint32 BindSlot = 0) const;
	void Unbind() const;

	inline uint32 GetWidth() const { return Width; }
	inline uint32 GetHeight() const { return Height; }
	inline uint32 GetBytesPerPixel() const { return BytesPerPixel; }
};

