#include "util.h"
#include<fstream>
#include<sstream>
#include<glm.hpp>

std::string ReadFile(const std::string ShaderPath) {
	std::ifstream stream(ShaderPath);
	std::stringstream buffer;
	buffer << stream.rdbuf();
	return buffer.str();
}


/*
* This is very PRIMITIVE loader, only loads a single object...........................
* TODO: write 'proper' obj loader for MY needs (no point in doing anything crazy)
*/
mesh_data ReadObjFile(std::string FilePath)
{
	mesh_data Mesh;
	std::ifstream Stream(FilePath);
	std::string Line;
	while (std::getline(Stream, Line)) {
		std::istringstream iss(Line);
		std::vector<std::string> Parts;
		for (std::string s; iss >> s;) {
			Parts.push_back(s);
		}
		if (!Parts.empty()) {
			if (Parts[0] == "v") {
				glm::vec3 Pos;
				Pos.x = std::stof(Parts[1]);
				Pos.y = std::stof(Parts[2]);
				Pos.z = std::stof(Parts[3]);
				Mesh.Position.push_back(Pos);
			}
			if (Parts[0] == "vn") {
				glm::vec3 Nor;
				Nor.x = std::stof(Parts[1]);
				Nor.y = std::stof(Parts[2]);
				Nor.z = std::stof(Parts[3]);
				Mesh.Normals.push_back(Nor);
			}

			//This gets ugly................. Probabyly should make it better one day..........
			if (Parts[0] == "f") {
				mesh_indicies Indicies;
				for (uint32 i = 1; i <= 3; ++i) {
					uint32 internal_i = 0;
					std::string tmp_val = "";
					for (uint32 a = 0; a <= Parts[i].size(); ++a) {
						auto& Part = Parts[i];
						if (a < Part.size() && Part[a] != '/') {
							tmp_val += Part[a];
						}
						else {
							if (internal_i == 0) {
								Indicies.vert_idx[i-1] = std::stoi(tmp_val) - 1;

							}
							else if (internal_i == 2) {
								Indicies.normal_idx[i-1] = std::stoi(tmp_val) - 1;
							}
							internal_i += 1;
							tmp_val = "";
						}
					}
				}
				Mesh.Indicies.push_back(Indicies);
			}
		}
	}
	return Mesh;
}

