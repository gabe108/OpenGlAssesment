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
	/// The public ID of the shader
	unsigned int ID;

	/// <summary>
	/// Extracts shader code from the filepaths <paramref name="_vertexPath"/> and <paramref name="_fragmentPath"/> 
	/// and call compileShader passing in the shader code.
	/// </summary>
	/// <returns>
	/// Calls compileShader passing in the shader code
	/// </returns>
	/// <param name="_vertexPath"> const char* </param>
	/// <param name="_fragmentPath"> const char* </param>
	Shader(const char* _vertexPath, const char* _fragmentPath);
	~Shader();

	/// <summary>
	/// Sets the current program to this shader
	/// </summary>
	void use();

	/// <summary>
	///  Gets the constant chars from the constructor 
	///  and tries to compile them 
	/// </summary>
	/// <returns>
	/// Calls the createProgram passing in the shader IDs
	/// </returns>
	/// <param name="_vcode"> const char* </param>
	/// <param name="_fcode"> const char* </param>
	void compileShader(const char* _vcode, const char* _fcode);

	/// <summary>
	///  Gets shader IDs from compileShader function <paramref name="vertex"/> and <paramref name="fragment"/> 
	///  and creates the shader program
	/// </summary>
	/// <param name="vertex"> unsigned int </param>
	/// <param name="fragment"> unsigned int </param>
	void createProgram(unsigned int vertex, unsigned int fragment);

	/// <summary>
	///  All of the following functions are to set uniform variables
	/// </summary>
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

};

