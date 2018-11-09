#include "Texture.h"

Texture::Texture(const std::string & file)
{
}

Texture::~Texture()
{
	unload();
}

bool Texture::load(const std::string & file)
{
	

	unsigned char* textureData;
	textureData = SOIL_load_image(file.c_str(), &this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		printf("TEXTURE BROKE: %s", file.c_str());
		return false;
	}

	// if it's a 2D texture (width and height are both greater than 1
	this->_Target = GL_TEXTURE_2D;
	this->_InternalFormat = GL_RGBA8;

	glGenTextures(1, &this->_TexHandle);
	this->bind();
	glTextureStorage2D(this->_TexHandle, 1, this->_InternalFormat, this->sizeX, this->sizeY);

	glTextureSubImage2D(this->_TexHandle, 0, 0, 0, this->sizeX, this->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MIN_FILTER, this->_FilterMin);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_MAG_FILTER, this->_FilterMag);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_S, this->_WrapX);
	glTextureParameteri(this->_TexHandle, GL_TEXTURE_WRAP_T, this->_WrapY);

	this->unbind();

	SOIL_free_image_data(textureData);

	return true;
}

bool Texture::unload()
{
	if (this->_TexHandle)
	{
		glDeleteTextures(1, &this->_TexHandle);
		this->_TexHandle = 0;
		return true;
	}

	return false;
}

#include <assert.h>
void Texture::setActiveSlot(int textureSlot) const
{
	assert(textureSlot >= 0 && textureSlot < 32);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_TexHandle);
}

void Texture::bind(int textureSlot) const
{
	setActiveSlot(textureSlot);
	bind();
}

void Texture::unbind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unbind(int textureSlot) const
{
	setActiveSlot(textureSlot);
	unbind();
}
