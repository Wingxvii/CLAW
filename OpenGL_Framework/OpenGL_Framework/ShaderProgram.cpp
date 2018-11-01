#include "ShaderProgram.h"

#include <vector>
#include <fstream>
#include <iostream>

bool ShaderProgram::_IsInitDefault = false;
GLuint ShaderProgram::_VertShaderDefault = 0;
GLuint ShaderProgram::_FragShaderDefault = 0;
GLuint ShaderProgram::_ProgramDefault = 0;


ShaderProgram::ShaderProgram()
{
	setDefault();
}

ShaderProgram::ShaderProgram(const std::string & vertFile, const std::string & fragFile)
{
	Load(vertFile,fragFile);
}

ShaderProgram::~ShaderProgram()
{
	if (_IsInit) {
		UnLoad();
	}

}

bool ShaderProgram::initDefault()
{
	if (!_IsInitDefault) {
		ShaderProgram errorShader;
		bool compileSuccess = errorShader.Load("./Assets/Shaders/error.vert", "./Assets/Shaders/error.frag");
		if (!compileSuccess) {
			std::cout << "Default Shader invalid\n";
			system("pause");
			exit(EXIT_FAILURE);
		}
		_VertShaderDefault = errorShader._VertexShader;
		_FragShaderDefault = errorShader._FragShader;
		_ProgramDefault = errorShader._Program;
		_IsInitDefault = true;

	}
	return _IsInitDefault;
}

void ShaderProgram::setDefault()
{
	_VertexShader = _VertShaderDefault;
	_FragShader = _FragShaderDefault;
	_Program = _ProgramDefault;
}

bool ShaderProgram::Load(const std::string & vertFile, const std::string & fragFile)
{
	_vertexFilename = vertFile;
	_fragFilename = fragFile;

	//create shader and program objects
	_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	_FragShader = glCreateShader(GL_FRAGMENT_SHADER);
	_Program = glCreateProgram();

	//load vert shader source code
	std::string source = ReadFile(vertFile);
	const GLchar *temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(_VertexShader, 1, &temp, NULL);

	//load fragment shader source code
	source = ReadFile(fragFile);
	temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(_FragShader, 1, &temp, NULL);

	//compile code
	if (!CompileShader(_VertexShader)) {
		std::cout << "Vertex shader failed to compile.\n";

		OutputShaderLog(_VertexShader);
		UnLoad();
		setDefault();
		return false;
	}
	if (!CompileShader(_FragShader)) {
		std::cout << "Fragment shader failed to compile.\n";

		OutputShaderLog(_FragShader);
		UnLoad();
		setDefault();
		return false;
	}
	
	//setup program object
	glAttachShader(_Program, _VertexShader);
	glAttachShader(_Program, _FragShader);

	//links program
	if (!LinkProgram()) 
	{
		std::cout << "Shader program failed to link.\n";

		OutputProgramLog();
		UnLoad();

		return false;
	}

	_IsInit = true;
	return true;

}

bool ShaderProgram::reload()
{
	return Load(_vertexFilename, _fragFilename);
}

bool ShaderProgram::isLoaded() const
{
	return _IsInit;
}

void ShaderProgram::UnLoad()
{
	if (_VertexShader != GL_NONE && _VertexShader != _VertShaderDefault) {
		glDetachShader(_Program, _VertexShader);
		glDeleteShader(_VertexShader);
		_VertexShader = GL_NONE;
	}
	if (_FragShader != GL_NONE && _VertexShader != _VertShaderDefault) {
		glDetachShader(_Program, _VertexShader);
		glDeleteShader(_VertexShader);
		_VertexShader = GL_NONE;
	}
	if (_Program != GL_NONE && _VertexShader != _VertShaderDefault) {
		glDeleteProgram(_Program);
		_Program = GL_NONE;
	}

}

bool ShaderProgram::LinkProgram()
{
	glLinkProgram(_Program);

	GLint success;
	glGetProgramiv(_Program, GL_LINK_STATUS, &success);

	return success == GL_TRUE;


}

void ShaderProgram::Bind() const
{
	glUseProgram(_Program);
}

void ShaderProgram::UnBind()
{
	glUseProgram(GL_NONE);
}

void ShaderProgram::AddAttribute(unsigned int index, const std::string & attribName)
{
	glBindAttribLocation(_Program, index, attribName.c_str());
}

void ShaderProgram::AddAttrib(unsigned int index, const std::string & attribName)
{
	glBindAttribLocation(_Program, index, attribName.c_str());
	LinkProgram();
}

int ShaderProgram::GetAttribLocation(const std::string & attribName)
{
	return glGetAttribLocation(_Program, attribName.c_str());
}

int ShaderProgram::GetUniformLocation(const std::string & uniformName)
{
	return glGetUniformLocation(_Program, uniformName.c_str());


}

void ShaderProgram::SendUniform(const std::string & name, int integer)
{
	GLint location = GetUniformLocation(name);
	glUniform1i(location, integer);

}

void ShaderProgram::SendUniform(const std::string & name, unsigned int uInteger)
{
	GLint location = GetUniformLocation(name);
	glUniform1ui(location, uInteger);

}

void ShaderProgram::SendUniform(const std::string & name, float scalar)
{
	GLint location = GetUniformLocation(name);
	glUniform1f(location, scalar);

}

void ShaderProgram::SendUniform(const std::string & name, const glm::vec2 & vector)
{
	GLint location = GetUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);

}

void ShaderProgram::SendUniform(const std::string & name, const glm::vec3 & vector)
{
	GLint location = GetUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::SendUniform(const std::string & name, const glm::vec4 & vector)
{
	GLint location = GetUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::SendUniformMat3(const std::string & name, float * matrix, bool transpose)
{
	GLint location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void ShaderProgram::SendUniformMat4(const std::string & name, const float * matrix, bool transpose)
{
	GLint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

std::string ShaderProgram::ReadFile(const std::string & fileName) const
{
	std::ifstream inStream(fileName);

	if (!inStream.good()) {
		std::cout << "Shader file not found\n";
		return std::string();
	}

	std::string data(std::istreambuf_iterator<char>(inStream),(std::istreambuf_iterator<char>()));
	return data;
}

bool ShaderProgram::CompileShader(GLuint shader) const
{
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	return success == GL_TRUE;

}

void ShaderProgram::OutputShaderLog(GLuint shader) const
{
	std::vector<char> infoLog;
	infoLog.resize(512);


	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	glGetShaderInfoLog(shader, sizeof(char)*512, &infoLen, &infoLog[0] );

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;

}

void ShaderProgram::OutputProgramLog() const
{
	std::vector<char> infoLog;
	infoLog.resize(512);


	GLint infoLen;
	glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &infoLen);

	glGetProgramInfoLog(_Program, sizeof(char) * 512, &infoLen, &infoLog[0]);

	std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;

}
