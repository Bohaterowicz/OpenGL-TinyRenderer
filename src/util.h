#pragma once
#include<string>
#include<vector>

#include "mesh_data.h"

#define Assert(x) if(!(x)) __debugbreak()


std::string ReadFile(const std::string ShaderPath);
void LoadPNG(std::string Path, uint8** PixelBuffer, int32& Width, int32& Height, int32& BytesPerPixel, int32 ChannelsCount);
mesh_data ReadObjFile(std::string FilePath);