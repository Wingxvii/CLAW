#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

bool ShaderProgram::Load(const std::string & vertFile, const std::string & fragFile)
{
	return false;
}

bool ShaderProgram::isLoaded() const
{
	return false;
}

void ShaderProgram::UnLoad()
{
}

bool ShaderProgram::LinkProgram()
{
	return false;
}

void ShaderProgram::Bind() const
{
}

void ShaderProgram::UnBind()
{
}

void ShaderProgram::AddAttribute(unsigned int index, const std::string & attribName)
{
}

int ShaderProgram::GetAttribLocation(const std::string & attribName)
{
	return 0;
}

int ShaderProgram::GetUniformLocation(const std::string & uniformName)
{
	return 0;
}

void ShaderProgram::SendUniform(const std::string & name, int integer)
{
}

void ShaderProgram::SendUniform(const std::string & name, unsigned int uInteger)
{
}

void ShaderProgram::SendUniform(const std::string & name, float scalar)
{
}

void ShaderProgram::SendUniform(const std::string & name, const glm::vec2 & vector)
{
}

void ShaderProgram::SendUniform(const std::string & name, const glm::vec3 & vector)
{
}

void ShaderProgram::SendUniform(const std::string & name, const glm::vec4 & vector)
{
}

void ShaderProgram::SendUniformMat3(const std::string & name, float * matrix, bool transpose)
{
}

void ShaderProgram::SendUniformMat4(const std::string & name, float * matrix, bool transpose)
{
}

std::string ShaderProgram::ReadFile(const std::string & fileName) const
{
	return std::string();
}

bool ShaderProgram::CompileShader(GLuint shader) const
{
	return false;
}

void ShaderProgram::OutputShaderLog(GLuint shader) const
{
}

void ShaderProgram::OutputProgramLog() const
{
}
