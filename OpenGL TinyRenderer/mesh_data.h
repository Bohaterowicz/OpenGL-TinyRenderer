#pragma once
#include "tiny_renderer_c.h"
#include<vector>
#include<glm.hpp> //TODO: Do I want to use glm here??? Maybe write my own simple math library in the future?

#include "mesh_data.h"


/*
* TODO: This is not very optimal, maybe it would be better to store all vertex data directly into an continous buffer of floats
* and then specify the layout of the buffer in the struct (sort of the way opengl does it?)
* 
*/

struct mesh_indicies {
	uint32 vert_idx[3];
	uint32 normal_idx[3];
};

struct mesh_data {
	std::vector<glm::vec3> Position;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> UVCoords;
	std::vector<mesh_indicies> Indicies;
};
