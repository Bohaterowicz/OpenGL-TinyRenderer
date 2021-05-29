#pragma once
#include "tiny_renderer_c.h"
#include<string>
#include<unordered_map>
#include<glm/glm.hpp>

class shader_program
{
private:
	uint32 GLID;
	std::string VertexShaderSourcePath;
	std::string FragmentShaderSourcePath;
	std::unordered_map<std::string, int32> UniformMap;
public:
	shader_program(std::string VertexShaderSource, std::string FragmentShaderSource);
	~shader_program();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& Name, int32 v1);
	void SetUniform1f(const std::string& Name, real32 v1);
	void SetUniform3f(const std::string& Name, real32 v1, real32 v2, real32 v3);
	void SetUniform3f(const std::string& Name, glm::vec3& Values);
	void SetUniform4f(const std::string& Name, real32 v1, real32 v2, real32 v3, real32 v4);
	void SetUniform4f(const std::string& Name, glm::vec4& Values);
	void SetUniform2f(const std::string& Name, real32 v1, real32 v2);
	void SetUniformMtx3(const std::string& Name, glm::mat3& Values);
	void SetUniformMtx4(const std::string& Name, real32* Values);
	void SetUniformMtx4(const std::string& Name, glm::mat4& Values);
	void BindUniformBlock(std::string UniformBlockName, uint32 BindingIndex);
	int32 GetUniformLocation(const std::string& Name);
	uint32 CompileShader(std::string Source, uint32 Type);

	inline uint32 GetID() const { return GLID; }
	inline std::string GetVertexShaderSourcePath() const { return VertexShaderSourcePath; }
	inline std::string GetFragmentShaderSourcePath() const { return FragmentShaderSourcePath; }
};

