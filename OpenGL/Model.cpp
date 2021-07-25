#include "Model.h"

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		std::cout << "Model: " << fileName << " failed to load : " << importer.GetErrorString() << std::endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::RenderModel()
{
	for (unsigned int i{ 0 }; i < m_meshes.size(); i++)
	{
		unsigned int materialIndex = m_meshToTexture[i];

		if (materialIndex < m_textures.size() && m_textures[materialIndex])
		{
			m_textures[materialIndex]->UseTexture();
		}

		m_meshes[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (unsigned int i{ 0 }; i < m_meshes.size(); i++)
	{
		if (m_meshes[i])
		{
			delete m_meshes[i];
			m_meshes[i] = nullptr;
		}
	}

	for (unsigned int i{ 0 }; i < m_textures.size(); i++)
	{
		if (m_textures[i])
		{
			delete m_textures[i];
			m_textures[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i{ 0 }; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (unsigned int i{ 0 }; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}

}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	// Vertices
	for (unsigned int i{ 0 }; i < mesh->mNumVertices; i++)
	{
		// Position coordinates
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		
		// UV coordinates
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		// Normals
		// NOTE: the normals are all negative here because the fragment shader doesn't use negaive direction for the lights
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	// Indices
	for (unsigned int i{ 0 }; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j{ 0 }; j < face.mNumIndices; j++)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	m_meshes.emplace_back(newMesh);
	m_meshToTexture.emplace_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	m_textures.resize(scene->mNumMaterials);

	for (unsigned int i{ 0 }; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		m_textures[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				unsigned int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);
				std::string texPath = std::string("Textures\\") + fileName;

				m_textures[i] = new Texture(texPath.c_str());

				if (!m_textures[i]->LoadTexture())
				{
					std::cout << "Failed to load texture at: " << texPath << std::endl;
					delete m_textures[i];
					m_textures[i] = nullptr;
				}
			}
		}

		if (!m_textures[i])
		{
			m_textures[i] = new Texture("Textures\\plain.png");
			m_textures[i]->LoadTextureA();
		}
	}
}
