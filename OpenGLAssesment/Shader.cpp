#include "Shader.h"

/// <summary>
/// Extracts shader code from the filepaths <paramref name="_vertexPath"/> and <paramref name="_fragmentPath"/> 
/// and call compileShader passing in the shader code.
/// </summary>
/// <returns>
/// Calls compileShader passing in the shader code
/// </returns>
/// <param name="_vertexPath"> const char* </param>
/// <param name="_fragmentPath"> const char* </param>
Shader::Shader(const char * _vertexPath, const char * _fragmentPath)
{
	// Declaring variables 
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Setting bitmask exceptions for both ifstreams
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Try 
	try
	{
		// Open files through the path passed in
		vShaderFile.open(_vertexPath);
		fShaderFile.open(_fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		
		// passes the pointer to the buffer object in each file
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Closes the files
		vShaderFile.close();
		fShaderFile.close();

		// turns the buffer to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	// if failed return an error
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	
	// Passes the string to compileShader 
		// So it can compile these strings to usable shader
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	compileShader(vShaderCode, fShaderCode);
}

Shader::~Shader()
{

}

void Shader::use()
{
	glUseProgram(ID);
}

/// <summary>
///  Gets the constant chars from the constructor <paramref name="_vcode"/> and <paramref name="_fcode"/> 
///  and tries to compile them 
/// </summary>
/// <returns>
/// Calls the createProgram passing in the shader IDs
/// </returns>
/// <param name="_vcode"> const char* </param>
/// <param name="_fcode"> const char* </param>
void Shader::compileShader(const char* _vcode, const char* _fcode)
{
	// Declaring variables
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// Creating a Shader and storing the ID 
	vertex = glCreateShader(GL_VERTEX_SHADER);

	// Replacing code in the shader we just created with
		// _vcode( passed in by the constructor)
	glShaderSource(vertex, 1, &_vcode, NULL);

	// Finally compile the shader
	glCompileShader(vertex);

	// Getting if the shader compiled or not
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	//if its not successful
		// return an error
	if (!success)
	{
		// Getting the error string
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	success = 0;

	// Creating a Shader and storing the ID 
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// Replacing code in the shader we just created with
		// _fcode( passed in by the constructor)
	glShaderSource(fragment, 1, &_fcode, NULL);

	// Finally compile the shader
	glCompileShader(fragment);

	// Getting if the shader compiled or not
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	//if its not successful
		// return an error
	if (!success)
	{
		// Getting the error string
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Calls createProgram passing in the ID
	createProgram(vertex, fragment);
}

/// <summary>
///  Gets shader IDs from compileShader function <paramref name="vertex"/> and <paramref name="fragment"/> 
///  and creates the shader program
/// </summary>
/// <param name="vertex"> unsigned int </param>
/// <param name="fragment"> unsigned int </param>
void Shader::createProgram(unsigned int vertex, unsigned int fragment)
{
	int success;
	char infoLog[512];

	// Creates the program and returns its ID
	ID = glCreateProgram();

	// Attaching the shaders to the program
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	// Linking the Program
	glLinkProgram(ID);

	// Getting if the shader compiled or not
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	//if its not successful
		// return an error
	if (!success)
	{
		// Getting the error string
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shader after u have linked them to the program
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
