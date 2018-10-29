#pragma once
#include "GL\glew.h"

#include "glm/glm.hpp"
#include <string>

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(const std::string &vertFile, const std::string &fragFile);

	~ShaderProgram();

	static bool initDefault();
	void setDefault();

	//loads a vertex shader and a fragment shader and places them in a program
	bool Load(const std::string &vertFile, const std::string &fragFile);
	bool reload();
	bool isLoaded() const;
	//clears all data from opengl
	void UnLoad();

	//allows for manual linking
	bool LinkProgram();

	//binds shader from use
	void Bind()const;
	//detaches shader from use
	void UnBind();

	//requires a relink before opengl registers new attribute
	void AddAttribute(unsigned int index, const std::string &attribName);
	//version that auto-relinks(use for single add)
	void AddAttrib(unsigned int index, const std::string &attribName);



	//returns -1 if attribute does not exist in shader
	int GetAttribLocation(const std::string &attribName);
	//returns -1 if uniform does not exist
	int GetUniformLocation(const std::string &uniformName);

	//send data to shaders
	void SendUniform(const std::string &name, int integer);
	void SendUniform(const std::string &name, unsigned int uInteger);
	void SendUniform(const std::string &name, float scalar);
	void SendUniform(const std::string &name, const glm::vec2 &vector);
	void SendUniform(const std::string &name, const glm::vec3 &vector);
	void SendUniform(const std::string &name, const glm::vec4 &vector);
	void SendUniformMat3(const std::string &name, float *matrix, bool transpose);
	void SendUniformMat4(const std::string &name, float *matrix, bool transpose);



private:
	bool _IsInit = false;
	GLuint _VertexShader = 0;
	GLuint _FragShader = 0;
	GLuint _Program = 0;

	std::string _vertexFilename;
	std::string _fragFilename;

	static bool _IsInitDefault;
	static GLuint _VertShaderDefault;
	static GLuint _FragShaderDefault;
	static GLuint _ProgramDefault;

	//pulls entire shader file into one string
	std::string ReadFile(const std::string &fileName)const;
	//compiles shaders
	bool CompileShader(GLuint shader) const;
	//outputs shader error list
	void OutputShaderLog(GLuint shader) const;
	//outputs program linking error list
	void OutputProgramLog() const;


};

