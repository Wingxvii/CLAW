#pragma once
#include <GL\glew.h>
#include <string>

class Texture {
	public:
		Texture() = default;
		~Texture();

		bool Load(const std::string &file);
		void Unload();

		void Bind();
		void UnBind();

		//Handle to texture
		GLuint TextObj = 0;
};