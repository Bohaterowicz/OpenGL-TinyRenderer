#include "shader_program.h"
#include<GL/glew.h>
#include<iostream>
#include "opengl_util.h"
#include "glm/gtc/type_ptr.hpp"

shader_program::shader_program(std::string VertexShaderSource, std::string FragmentShaderSource) : GLID(0), VertexShaderSourcePath(VertexShaderSource), FragmentShaderSourcePath(FragmentShaderSource)
{
	GLCall(GLID = glCreateProgram());
	uint32 VertexShader = CompileShader(VertexShaderSource, GL_VERTEX_SHADER);
	uint32 FragmentShader = CompileShader(FragmentShaderSource, GL_FRAGMENT_SHADER);

	if (FragmentShader && VertexShader) {
		GLCall(glAttachShader(GLID, VertexShader));
		GLCall(glAttachShader(GLID, FragmentShader));
		GLCall(glLinkProgram(GLID));
		GLCall(glValidateProgram(GLID));

		GLCall(glDeleteShader(VertexShader));
		GLCall(glDeleteShader(FragmentShader));
	}
	else {
		GLID = 0;
	}
}

uint32 shader_program::CompileShader(std::string Source, uint32 Type) {
	GLuint Shader = glCreateShader(Type);
	const char* src = Source.c_str();
	GLCall(glShaderSource(Shader, 1, &src, nullptr));
	GLCall(glCompileShader(Shader));

	int Result;
	GLCall(glGetShaderiv(Shader, GL_COMPILE_STATUS, &Result));

	if (Result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(Shader, length, &length, message));
		std::cout << "FAILED TO COMPILE SHADER: " << (Type == GL_VERTEX_SHADER ? "VERTEX_SHADER" : "FRAGMENT_SHADER") << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(Shader));
		return 0;
	}

	return Shader;
}

shader_program::~shader_program()
{
	GLCall(glDeleteProgram(GLID));
}

void shader_program::Bind() const
{
	GLCall(glUseProgram(GLID));
}

void shader_program::Unbind() const
{
	GLCall(glUseProgram(0));
}

void shader_program::SetUniform1i(const std::string& Name, int32 v1)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform1i(UniformLocation, v1));
	}
}

void shader_program::SetUniform1f(const std::string& Name, real32 v1)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform1f(UniformLocation, v1));
	}
}

void shader_program::SetUniform3f(const std::string& Name, real32 v1, real32 v2, real32 v3)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform3f(UniformLocation, v1, v2, v3));
	}
}

void shader_program::SetUniform3f(const std::string& Name, glm::vec3& Values)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform3fv(UniformLocation, 1, glm::value_ptr(Values)));
	}
}

void shader_program::SetUniform4f(const std::string& Name, real32 v1, real32 v2, real32 v3, real32 v4)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform4f(UniformLocation, v1, v2, v3, v4));
	}
}

void shader_program::SetUniform4f(const std::string& Name, glm::vec4& Values)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform4fv(UniformLocation, 1, glm::value_ptr(Values)));
	}
}

void shader_program::SetUniform2f(const std::string& Name, real32 v1, real32 v2)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		GLCall(glUniform2f(UniformLocation, v1, v2));
	}
}

void shader_program::SetUniformMtx3(const std::string& Name, glm::mat3& Values)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		glUniformMatrix3fv(UniformLocation, 1, false, glm::value_ptr(Values));
	}
}

void shader_program::SetUniformMtx4(const std::string& Name, real32* Values)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		glUniformMatrix4fv(UniformLocation, 1, false, Values);
	}
}

void shader_program::SetUniformMtx4(const std::string& Name, glm::mat4& Values)
{
	int32 UniformLocation = GetUniformLocation(Name);
	if (UniformLocation != -1) {
		glUniformMatrix4fv(UniformLocation, 1, false, glm::value_ptr(Values));
	}
}

void shader_program::BindUniformBlock(std::string UniformBlockName, uint32 BindingIndex)
{
	GLCall(uint32 UniformBlockIndex = glGetUniformBlockIndex(GLID, UniformBlockName.c_str()));
	GLCall(glUniformBlockBinding(GLID, UniformBlockIndex, BindingIndex));
}

int32 shader_program::GetUniformLocation(const std::string& Name)
{
	if (UniformMap.find(Name) != UniformMap.end()) {
		return UniformMap[Name];
	}

	GLCall(int32 UniformLocation = glGetUniformLocation(GLID, Name.c_str()));
	if (UniformLocation == -1) {
		std::cout << "Warning: Uniform " << Name << " does not exist..." << std::endl;
	}
	UniformMap[Name] = UniformLocation;
	return UniformLocation;
}
