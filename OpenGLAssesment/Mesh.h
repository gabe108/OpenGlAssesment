#pragma once
#include<glm/ext.hpp>
#include<string>
#include<vector>

class Shader;

// Vertex struct that contains its 
// Position, Normal, Texture Coordinates
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

// Texture struct containing the ID,
// type, path
struct Texture
{
	unsigned int ID;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	/// <summary>
	/// Constructor to set member variables
	/// </summary>
	/// <param name="_vertices"> std::vector<Vertex> </param>
	/// <param name="_indices"> std::vector<unsigned int> </param>
	/// <param name="_textures"> std::vector<Texture> </param>
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
	~Mesh();

	/// <summary>
	/// TO the draw the mesh itself
	/// </summary>
	/// <param name="_shader"> Shader* </param>
	void Draw(Shader* _shader);

private:
	unsigned int VAO, VBO, IBO;

	/// <summary>
	/// Setup the VAO, VBO and IBO by binding them and generating arrays
	/// </summary>
	void setupMesh();
};

