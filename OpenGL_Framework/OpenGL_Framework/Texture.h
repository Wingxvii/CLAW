#pragma once
#include "GL/glew.h"
#pragma comment(lib, "SOIL_ext.lib")
#include "SOIL/SOIL.h"
#include <string>
#include <vector>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string &file);
	~Texture();

	bool load(const std::string &file); // Use to load 1D or 2D textures
	bool unload();

	void setActiveSlot(int textureSlot) const;
	void bind() const;
	void bind(int textureSlot) const;
	void unbind() const;
	void unbind(int textureSlot) const;

private:
	GLuint _TexHandle;

	std::string _Filename;

	GLenum _Target;
	GLenum _InternalFormat;

	// There's a comple more
	GLenum _FilterMag = GL_LINEAR; //GL_NEAREST
	GLenum _FilterMin = GL_LINEAR;
	GLenum _WrapX = GL_REPEAT; //GL_MIRRORED_REPEAT
	GLenum _WrapY = GL_REPEAT; //GL_CLAMP_TO_EDGE

	int sizeX;
	int sizeY;
	int channels;
};