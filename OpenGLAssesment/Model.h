#pragma once
#include<vector>
#include<string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include"Mesh.h"

class Shader;

using namespace std;

/// <summary>
/// Loads the texture from an Image
/// </summary>
/// <returns>
/// Returns the textureID after the texture is loaded
/// </returns>
/// <param name="path"> const char* </param>
/// <param name="directory"> const string & </param>
/// <param name="gamma"> bool </param>
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
	// Loads the model according to the path of the model
	Model(string path)
	{
		loadModel(path);
	}

	/// <summary>
	/// Just calls the meshes draw function
	/// </summary>
	/// <param name="shader"> Shader* </param>
	void Draw(Shader* shader);

private:
	//  Model Data  
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	/// <summary>
	/// Use the Assimp Importer to load the model and call processNode
	/// </summary>
	/// <param name="path"> string </param>
	void loadModel(string path);

	/// <summary>
	/// Process all the node's meshes including the children nodes
	/// </summary>
	/// <param name="node"> aiNode* </param>
	/// <param name="scene"> const aiScene* </param>
	void processNode(aiNode *node, const aiScene *scene);

	/// <summary>
	/// Process the mesh node and the materials 
	/// </summary>
	/// <returns>
	/// return a mesh object created from the extracted mesh data
	/// </returns>
	/// <param name="mesh"> aiMesh* </param>
	/// <param name="scene"> const aiScene* </param>
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	
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
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
};

