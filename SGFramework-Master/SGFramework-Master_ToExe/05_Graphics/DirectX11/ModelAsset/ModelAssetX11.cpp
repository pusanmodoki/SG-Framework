#include "ModelAssetX11.hpp"
#include "../../../04_Asset/InstantiateAsset.hpp"
#include "../../Animation/Animator.hpp"

// Framework namespace
namespace SGFramework
{
	//フレームワークの管理をするAdministrator namespace
	namespace Administrator
	{
		//GraphicsDetail namespace
		namespace GraphicsDetail
		{
			void ModelAssetX11::Open(WeakPointer<GameObject> loadObject,
				SharedPointer<Administrator::GraphicsDetail::VertexShaderAssetX11> defaultVertexShader,
				SharedPointer<Administrator::GraphicsDetail::PixelShaderAssetX11> defaultPixelShader, uint drawPriority)
			{
				m_drawPriority = drawPriority;

				m_rootObject = loadObject;
				
				m_rootAnimator = m_rootObject->FindComponent<Animator>();
				if (IS_FALSE(m_rootAnimator.getIsValid())) m_rootAnimator = m_rootObject->AddComponent<Animator>();

				m_rootAnimator->m_priority = m_drawPriority;

				std::string str = name().to_std_string();

				m_scene = aiImportFile(str.c_str(),
					aiProcess_ConvertToLeftHanded |
					aiProcessPreset_TargetRealtime_Fast
					//aiProcessPreset_TargetRealtime_MaxQuality
				);

				if (m_scene == nullptr)
				{
					auto error = aiGetErrorString();
					throw InvalidArgument(L"Error!! ModelAsset->Open", aiGetErrorString());
				}
				
				
				m_rootAnimator->m_scene = m_scene;


				if (m_scene->HasMaterials())
				{
					for (uint i = 0, numMaterials = m_scene->mNumMaterials;
						i < numMaterials; ++i)
					{
						aiMaterial* modelMaterial = m_scene->mMaterials[i];
						aiColor4D getData;
						float power;

						m_materials.emplace_back(new MaterialX11());
						MaterialX11& material = *m_materials.back();

						modelMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, getData);
						material.diffuse.r = getData.r;
						material.diffuse.g = getData.g;
						material.diffuse.b = getData.b;
						material.diffuse.a = getData.a;

						modelMaterial->Get(AI_MATKEY_COLOR_AMBIENT, getData);
						material.ambient.r = getData.r;
						material.ambient.g = getData.g;
						material.ambient.b = getData.b;
						material.ambient.a = getData.a;

						modelMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, getData);
						material.emission.r = getData.r;
						material.emission.g = getData.g;
						material.emission.b = getData.b;
						material.emission.a = getData.a;

						modelMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, getData);
						material.manualAlpha = getData.a;

						modelMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, power);
						material.power = power;

						material.vertexShader = defaultVertexShader;
						material.pixelShader = defaultPixelShader;

						material.name = modelMaterial->GetName().C_Str();

						FindTexture(&material, modelMaterial,
							aiTextureType::aiTextureType_DIFFUSE, name());
					}
				}

				ProcessNodeCreateObject(m_scene->mRootNode);

				int counter = 0;
				for (auto& e : m_nodeObjectsKeyNode)
				{
					e.second->m_nodeID = counter;
					counter++;
				}
				
				ProcessNodeCreateMesh(m_scene->mRootNode);

				for (uint i = 0; i < m_scene->mNumAnimations; ++i)
				{
					m_rootAnimator->AddAnimation(m_scene->mAnimations[i]->mName.C_Str(),
						m_scene->mAnimations[i]);
				}
			}

			void ModelAssetX11::OpenAnimation(const sgstring & filePath)
			{
				std::string str = filePath.to_std_string();

				auto scene = aiImportFile(str.c_str(),
					aiProcess_ConvertToLeftHanded |
					aiProcessPreset_TargetRealtime_MaxQuality |
					aiProcess_PopulateArmatureData);

				if (scene == nullptr)
				{
					auto error = aiGetErrorString();
					throw InvalidArgument(L"Error!! ModelAsset->Open", aiGetErrorString());
				}

				m_animationScenes.try_emplace(filePath, scene);


				for (uint i = 0; i < scene->mNumAnimations; ++i)
				{
					m_rootAnimator->AddAnimation(scene->mAnimations[i]->mName.C_Str(),
						scene->mAnimations[i]);
				}

				OpenBaseAsset();
			}

			//----------------------------------------------------------------------------------
			//[Reload]
			//アセットのリロードを行う
			void ModelAssetX11::Reload()
			{

			}

			//----------------------------------------------------------------------------------
			//[Close]
			//ファイルを閉じる
			void ModelAssetX11::Close()
			{

			}



			void ModelAssetX11::CreateMesh(aiNode* node, aiMesh * mesh, int meshidx)
			{
				m_meshs.emplace_back(new MeshX11<Vertex::Model, MaterialX11*>());
				
				MeshX11<Vertex::Model, MaterialX11*>* meshReference = m_meshs.back().getPointer();

				m_rootAnimator->m_meshRenderers.emplace_back(
					m_nodeObjectsKeyName.at(node->mName.C_Str())->AddMeshRenderer(m_meshs.back(), m_drawPriority));

				meshReference->m_material = m_materials[mesh->mMaterialIndex].getPointer();

				for (uint i = 0; i < mesh->mNumVertices; ++i)
				{
					Vertex::Model vertex;

					vertex.position.x = mesh->mVertices[i].x;
					vertex.position.y = mesh->mVertices[i].y;
					vertex.position.z = mesh->mVertices[i].z;

					if (mesh->HasNormals())
					{
						vertex.normal.x = mesh->mNormals[i].x;
						vertex.normal.y = mesh->mNormals[i].y;
						vertex.normal.z = mesh->mNormals[i].z;
					}
					else
						vertex.normal = Const::Vector3::zero;

					for (uint k = 0; k < Vertex::Model::cNumSupportedUVs; ++k)
					{
						if (mesh->HasTextureCoords(k))
						{
							vertex.uvArray[k].u = mesh->mTextureCoords[k][i].x;
							vertex.uvArray[k].v = mesh->mTextureCoords[k][i].y;
						}
						else
							vertex.uvArray[k] = Const::UV::zero;
					}

					meshReference->m_vertexes.emplace_back(vertex);
				}

				for (uint i = 0; i < mesh->mNumFaces; ++i)
				{
					aiFace face = mesh->mFaces[i];

					for (uint k = 0; k < face.mNumIndices; ++k)
						meshReference->m_indexes.emplace_back(face.mIndices[k]);
				}

				for (uint i = 0; i < mesh->mNumBones; ++i)
				{
					aiBone* bone = mesh->mBones[i];

					m_nodeObjectsKeyName.at(bone->mName.C_Str())->RegisterAnimationMatrix(bone->mOffsetMatrix);
					
					for (uint weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
					{
						aiVertexWeight weight = bone->mWeights[weightIndex];
						int boneIndex = meshReference->m_vertexes[weight.mVertexId].getInitialBoneIndex();

						if (boneIndex != Vertex::Model::cInvalidBoneIndex)
						{
							meshReference->m_vertexes[weight.mVertexId].boneIndexes[boneIndex] = 
								m_nodeObjectsKeyName.at(bone->mName.C_Str())->getNodeID();
							meshReference->m_vertexes[weight.mVertexId].weights[boneIndex] = weight.mWeight;					
						}
						else
						{
							uint minIndex = 0;
							float minWeight = 100.0f;
							for (uint i = 0; i < Vertex::Model::cNumSupportedBones; ++i)
							{
								if (meshReference->m_vertexes[weight.mVertexId].weights[i] < minWeight)
								{
									minIndex = i;
									minWeight = meshReference->m_vertexes[weight.mVertexId].weights[i];
								}
							}
							if (meshReference->m_vertexes[weight.mVertexId].weights[minIndex] > weight.mWeight)
							{
								meshReference->m_vertexes[weight.mVertexId].boneIndexes[minIndex] =
									m_nodeObjectsKeyName.at(bone->mName.C_Str())->getNodeID();
								meshReference->m_vertexes[weight.mVertexId].weights[minIndex] = weight.mWeight;
							}
						}
					}
				}

				for (auto& e : meshReference->m_vertexes)
				{
					float sum = 0.0f;
					for (uint i = 0; i < Vertex::Model::cNumSupportedBones; ++i)
						sum += e.weights[i];

					if (sum > 1.0f || sum  < 1.0f )
					{
						sum = 1.0f / sum;
						for (uint i = 0; i < Vertex::Model::cNumSupportedBones; ++i)
							e.weights[i] *= sum;
					}
					
					int nextIndex = e.getInitialBoneIndex();
					while (nextIndex != -1)
					{
						e.boneIndexes[nextIndex] = Vertex::Model::cIdentityBoneIndex;
						nextIndex = e.getInitialBoneIndex();
					}
				}

				meshReference->CreateBuffers();
			}




			void ModelAssetX11::ProcessNodeCreateObject(aiNode* node)
			{
				std::string nodeName = node->mName.C_Str();
				
				auto addObject = m_rootAnimator->AddAnimationObject( 
					node->mParent ? m_nodeObjectsKeyName.at(node->mParent->mName.C_Str())->transform.InstantiateShared() : SharedPointer<Transform>::getNull(),
					node->mTransformation, nodeName, 
					m_nodeObjectsKeyName.find(node->mName.C_Str()) != m_nodeObjectsKeyName.end() ? true : false);
				
				m_nodeObjectsKeyNode.try_emplace(node, addObject);
				m_nodeObjectsKeyName.try_emplace(nodeName, addObject);

	/*			Vector4x4 mat; 
				Quaternion rotation;
				Vector3 position, scale;
				memcpy_s(&mat, sizeof(Vector4x4), &node->mTransformation, sizeof(Vector4x4));
				mat.setTranspose();
				mat.Extraction(&position, &scale, &rotation);

				addObject->transform->setLocalPosition(position);
				addObject->transform->setLocalRotation(rotation);
				addObject->transform->setLocalScale(scale);*/
				
				//node->mTransformation;
				
				for (uint i = 0; i < node->mNumChildren; ++i)
				{
					ProcessNodeCreateObject(node->mChildren[i]);
				}
			}
			void ModelAssetX11::ProcessNodeCreateMesh(aiNode* node)
			{
				for (uint i = 0; i < node->mNumMeshes; ++i)
				{
					uint meshIndex = node->mMeshes[i];
					aiMesh* mesh = m_scene->mMeshes[meshIndex];

					CreateMesh(node, mesh, meshIndex);
				}

				for (uint i = 0; i < node->mNumChildren; ++i)
				{
					ProcessNodeCreateMesh(node->mChildren[i]);
				}
			}

			void ModelAssetX11::FindTexture(Administrator::GraphicsDetail::MaterialX11* material,
				aiMaterial* assimpMaterial, aiTextureType type, const sgstring& filePath)
			{
				uint maxLoadTextures = assimpMaterial->GetTextureCount(type);
				uint nowLoadTextures = 0;

				if (type == aiTextureType_NORMALS || type == aiTextureType_HEIGHT)
					maxLoadTextures = MathAF::Min(maxLoadTextures, 1U);
				else
					maxLoadTextures = MathAF::Min(maxLoadTextures,
						Administrator::GraphicsDetail::MaterialX11::cNumSupportedTextures);

				for (auto& e : material->textures)
					if (IS_TRUE(e.getIsValid())) ++nowLoadTextures;

				for (uint i = 0, textureIndex = nowLoadTextures, heightIndex = 0, normalIndex = 0;
					i < maxLoadTextures; ++i)
				{
					aiString aiFileName;
					sgstring fileName = filePath;

					assimpMaterial->GetTexture(type, i, &aiFileName);

					size_t find0 = fileName.find_last_of(L"/");
					size_t find1 = fileName.find_last_of(L"\\");
					size_t slashPoint = MathAF::Max(
						find0 != sgstring::npos ? find0 : 0,
						find1 != sgstring::npos ? find1 : 0);
					fileName.erase(fileName.begin() + slashPoint + 1, fileName.end());
					fileName += aiFileName.C_Str();

					switch (type)
					{
					case aiTextureType_NORMALS:
						if (normalIndex < MaterialX11::cNumSupportedNormalMaps)
						{
							material->normalMap[normalIndex] = Asset::InstantiateAsset<TextureAssetX11>(fileName, true);
							material->normalMap[normalIndex++]->Open();
						}
						break;
					case aiTextureType_HEIGHT:
						if (heightIndex < MaterialX11::cNumSupportedHeightMaps)
						{
							material->heightMap[heightIndex] = Asset::InstantiateAsset<TextureAssetX11>(fileName, true);
							material->heightMap[heightIndex++]->Open();
						}
						break;
					default:
						if (textureIndex < MaterialX11::cNumSupportedTextures)
						{
							material->textures[textureIndex] = Asset::InstantiateAsset<TextureAssetX11>(fileName, true);
							material->textures[textureIndex++]->Open();
						}
						break;
					}
				}
			}
		}
	}
}