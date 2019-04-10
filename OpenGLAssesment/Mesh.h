#pragma once
#include<glm/ext.hpp>
#include<string>
#include<vector>

class Shader;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

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

	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
	~Mesh();

	void Draw(Shader* _shader);

private:
	unsigned int VAO, VBO, IBO;
	void setupMesh();
};

