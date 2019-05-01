#include "Model.h"
#include "Mesh.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/// <summary>
/// Just calls the meshes draw function
/// </summary>
/// <param name="shader"> Shader* </param>
void Model::Draw(Shader* shader)
{
	// Drawing every mesh in the list
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

/// <summary>
/// Use the Assimp Importer to load the model and call processNode
/// </summary>
/// <param name="path"> string </param>
void Model::loadModel(string path)
{
	// Assimp Importer
	Assimp::Importer import;

	// The main scene where the model is loaded
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// if the scene fails to create or incomplete
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}

	// store the directory of the model for further use
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

/// <summary>
/// Process all the node's meshes including the children nodes
/// </summary>
/// <param name="node"> aiNode* </param>
/// <param name="scene"> const aiScene* </param>
void Model::processNode(aiNode * node, const aiScene * scene)
{
	// process each node's meshes 
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// for children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

/// <summary>
/// Process the mesh node and the materials 
/// </summary>
/// <returns>
/// return a mesh object created from the extracted mesh data
/// </returns>
/// <param name="mesh"> aiMesh* </param>
/// <param name="scene"> const aiScene* </param>
Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	// Declaring Variables
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; 

		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// texture coordinates
		// if the mesh contains texture coordinates
		if (mesh->mTextureCoords[0]) 
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// now walk through each of the mesh's faces and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

/// <summary>
/// iterates over all the texture locations of the given texture type, retrieves the 
/// texture's file location and then loads and generates the texture
/// </summary>
/// <returns>
/// stores and ruturns a list of Texture struct
/// </returns>
/// <param name="mat"> aiMaterial* </param>
/// <param name="type"> aiTextureType </param>
/// <param name="typeName"> string </param>
vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;

	// Iterate on how many textures are stored in this mat
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		// get the ocation of the texture
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			// if the current texture is already in the textures_loaded list then we skip this texture
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{   // if texture hasn't been loaded already, load it by SOIL
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

/// <summary>
/// Loads the texture from an Image
/// </summary>
/// <returns>
/// Returns the textureID after the texture is loaded
/// </returns>
/// <param name="path"> const char* </param>
/// <param name="directory"> const string & </param>
/// <param name="gamma"> bool </param>
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	// Get the filepath
	string filename = string(path);
	filename = directory + '/' + filename;

	// Create a variable for the texture ID
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// Getting the data from the texture image
	int w, h, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &w, &h, &nrComponents, 0);

	// if the data is not null
	if (data)
	{
		// set which type of format the texture is in
			// by the data we get
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		// bind a named texture to a texturing target
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Specify a 2d Dimensional texture image
		// which is easy for the shader to read
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Setting all the Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	// else throw an error
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}