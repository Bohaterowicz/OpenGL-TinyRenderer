#pragma once
#include<string>
#include<vector>

#include "mesh_data.h"

#define Assert(x) if(!(x)) __debugbreak()


std::string ReadFile(const std::string ShaderPath);

mesh_data ReadObjFile(std::string FilePath);