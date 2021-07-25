#pragma once

#include "Common.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model() {}
	~Model() {}

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

private:
	std::vector<Mesh*> m_meshes;
	std::vector<Texture*> m_textures;
	std::vector<unsigned int> m_meshToTexture;

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
};

