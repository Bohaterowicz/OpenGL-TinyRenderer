#pragma once
#include "tiny_renderer_c.h"
#include "../glimg/glimage.h"
#include <string>

class texture2d
{
private:
	uint32 GLID;
	uint8 *PixelBuffer;
	int32 Width;
	int32 Height;
	int32 BytesPerPixel;

public:
	texture2d(uint8 *Data, int32 Width, int32 Height, int32 BPP);
	texture2d(std::string Path);
	texture2d(std::string Path, bool32 Mipmaps, uint32 MipmapCount, uint32 ChannelCount);
	~texture2d();

	void Bind(uint32 BindSlot = 0) const;
	void Unbind() const;

	inline uint32 GetWidth() const { return Width; }
	inline uint32 GetHeight() const { return Height; }
	inline uint32 GetBytesPerPixel() const { return BytesPerPixel; }
};
