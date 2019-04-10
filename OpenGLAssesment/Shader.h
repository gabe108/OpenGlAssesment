#pragma once
#include"gl_core_4_4.h"

#include<glm/ext.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* _vertexPath, const char* _fragmentPath);
	~Shader();

	void use();
	void compileShader(const char* _vcode, const char* _fcode);
	void createProgram(unsigned int vertex, unsigned int fragment);
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

