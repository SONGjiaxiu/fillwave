/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Filip Wasil and Fillwave community members
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <flw/flc/buffers/mVertexBufferBasic.h>
#include <flw/flc/buffers/IndexBuffer.h>

#include <flw/flf/models/shapes/PhysicsMeshBuffer.h>
#include <flw/cmn/Material.h>

#include <flw/flf/loaders/modelloader/assimp/ModelLoaderAssimp.h>

#include <flw/Log.h>
#include <flw/flf/models/Model.h>

FLOGINIT_DEFAULT()

namespace flw {
namespace flf {

ModelLoaderTraitsAssimp::SceneType::SceneType(const std::string& filename)
  : mImporter()
  , mScene(mImporter.ReadFile(filename, aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_CalcTangentSpace)) {
  if (!mScene) {
    fLogF("Model: ", filename, " could not be read");
  }
}

template<>
void TModelLoader<ModelLoaderTraitsAssimp>::getPhysicsBuffer(const char* assetPath, flf::PhysicsMeshBuffer& buffer) {

  SceneType scene(assetPath);

  if (nullptr == scene.mScene) {
    return;
  }

  if (1 != scene.mScene->mNumMeshes) {
    return;
  }

  const aiMesh* shape = scene.mScene->mMeshes[0];
  buffer.mNumFaces = shape->mNumFaces;
  buffer.mVertices.reserve(shape->mNumVertices);
  buffer.mIndices.reserve(shape->mNumFaces*  3);
  for (GLuint j = 0; j < shape->mNumFaces; ++j) {
    buffer.mIndices.push_back(shape->mFaces[j].mIndices[0]);
    buffer.mIndices.push_back(shape->mFaces[j].mIndices[1]);
    buffer.mIndices.push_back(shape->mFaces[j].mIndices[2]);
  }
  for (GLuint z = 0; z < shape->mNumVertices; ++z) {
    const auto& v = shape->mVertices[z];
    buffer.mVertices[z] = { { v.x, v.y, v.z } };
  }
}

template<>
const Material TModelLoader<ModelLoaderTraitsAssimp>::getMaterial(const MaterialType& mat) {
  Material material;
  aiColor4D color;
  if (AI_SUCCESS == aiGetMaterialColor(&mat, AI_MATKEY_COLOR_AMBIENT, &color)) {
    material.mAmbient = glm::vec4(color.r, color.g, color.b, color.a);
  }
  if (AI_SUCCESS == aiGetMaterialColor(&mat, AI_MATKEY_COLOR_DIFFUSE, &color)) {
    material.mDiffuse = glm::vec4(color.r, color.g, color.b, color.a);
  }
  if (AI_SUCCESS == aiGetMaterialColor(&mat, AI_MATKEY_COLOR_SPECULAR, &color)) {
    material.mSpecular = glm::vec4(color.r, color.g, color.b, color.a);
  }
  return material;
}

template<>
flc::IndexBuffer* TModelLoader<ModelLoaderTraitsAssimp>::getIndexBuffer(const ShapeType* shape) {

  std::vector<GLuint> indices;

  indices.resize(shape->mNumFaces * 3);

  #pragma omp parallel for schedule(guided) num_threads(2) if (shape->mNumFaces > 1000)
    for (GLuint i = 0; i < shape->mNumFaces; ++i) {
      const GLuint idx = 3 * i;
      indices[idx] = shape->mFaces[i].mIndices[0];
      indices[idx + 1] = shape->mFaces[i].mIndices[1];
      indices[idx + 2] = shape->mFaces[i].mIndices[2];
    }

  return new ::flw::flc::IndexBuffer(indices);
}

template<>
flw::flc::VertexBufferBasic*
  TModelLoader<ModelLoaderTraitsAssimp>::getVertexBuffer(const ShapeType* sh, Animator* a) {
  std::vector<flc::VertexBasic> vertices;

  const size_t elements = sh->mNumVertices;

  vertices.resize(elements);

  {
#pragma omp parallel for schedule(guided) num_threads(2) if (sh->mNumVertices > 1000)
    for (GLuint i = 0; i < elements; i++) {
      flc::VertexBasic &vertex = vertices[i];

      if (sh->HasVertexColors(0)) {
        vertex.mColor[0] = sh->mColors[0]->r;
        vertex.mColor[1] = sh->mColors[0]->g;
        vertex.mColor[2] = sh->mColors[0]->b;
        vertex.mColor[3] = sh->mColors[0]->a;
      } else {
        vertex.mColor[0] = 0.0f;
        vertex.mColor[1] = 0.0f;
        vertex.mColor[2] = 0.0f;
        vertex.mColor[3] = 1.0f;
      }

      vertex.mPosition[0] = sh->mVertices[i].x;
      vertex.mPosition[1] = sh->mVertices[i].y;
      vertex.mPosition[2] = sh->mVertices[i].z;
      vertex.mPosition[3] = 1.0f;

      /* One normal each triangle - on */
      if (sh->HasNormals()) {
        vertex.mNormal[0] = sh->mNormals[i].x;
        vertex.mNormal[1] = sh->mNormals[i].y;
        vertex.mNormal[2] = sh->mNormals[i].z;
      } else {
        vertex.mNormal[0] = 0;
        vertex.mNormal[1] = 0;
        vertex.mNormal[2] = 0;
      }

      if (sh->HasTextureCoords(0)) {
        vertex.mTextureUV[0] = sh->mTextureCoords[0][i].x;
        vertex.mTextureUV[1] = sh->mTextureCoords[0][i].y;
      } else {
        vertex.mTextureUV[0] = 0;
        vertex.mTextureUV[1] = 0;
      }

      if (sh->HasTangentsAndBitangents()) {
        vertex.mNormalTangentMap[0] = sh->mTangents[i].x;
        vertex.mNormalTangentMap[1] = sh->mTangents[i].y;
        vertex.mNormalTangentMap[2] = sh->mTangents[i].z;
      } else {
        vertex.mNormalTangentMap[0] = 0;
        vertex.mNormalTangentMap[1] = 0;
        vertex.mNormalTangentMap[2] = 0;
      }
      for (int k = 0; k < TModelLoader::COUNT_BONES_USED; k++) {
        vertex.mBoneID[k] = 0;
        vertex.mBoneWeight[k] = 0.0f;
      }
    }
  }

  if (a) {
    std::vector<int> boneIdForEachVertex;
    boneIdForEachVertex.resize(vertices.size());
    for (size_t z = 0; z < vertices.size(); z++) {
      boneIdForEachVertex[z] = 0;
    }
    /* Bones */
    for (GLuint i = 0; i < sh->mNumBones; i++) {
      for (GLuint j = 0; j < sh->mBones[i]->mNumWeights; j++) {
        GLuint VertexID = sh->mBones[i]->mWeights[j].mVertexId;
        float Weight = sh->mBones[i]->mWeights[j].mWeight;
        if (boneIdForEachVertex[VertexID] < TModelLoader::COUNT_BONES_USED) {
          vertices[VertexID].mBoneID[boneIdForEachVertex[VertexID]] = a->getBoneId(sh->mBones[i]->mName.C_Str());
          vertices[VertexID].mBoneWeight[boneIdForEachVertex[VertexID]] = Weight;
          boneIdForEachVertex[VertexID]++;
        } else {
          fLogF("Fillwave can handle maximum ", TModelLoader::COUNT_BONES_USED, " bone dependencies.");
        }
      }
    }
  }
  return new flc::VertexBufferBasic (vertices);
}

template<>
void TModelLoader<ModelLoaderTraitsAssimp>::setTransformation(const NodeType* node, Entity* entity) {
  aiVector3t<float> scale;
  aiQuaterniont<float> rot;
  aiVector3t<float> pos;
  node->mTransformation.Decompose(scale, rot, pos);
  entity->scaleTo(glm::vec3(scale.x, scale.y, scale.z));
  entity->rotateTo(glm::quat(rot.w, rot.x, rot.y, rot.z));
  entity->moveTo(glm::vec3(pos.x, pos.y, pos.z));
}

const std::string getTextureName (ModelLoader::TextureType type, const aiMaterial* mat) {
  aiString s;
  return
    mat->GetTexture(
      type, 0, &s, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS ? s.data : "128_128_128.color";
}

template<>
AnimatorAssimp* TModelLoader<ModelLoaderTraitsAssimp>::getAnimator(const SceneType& scene) {
  if (0 == scene.mScene->mNumAnimations) {
    return nullptr;
  }
  return new AnimatorAssimp(scene.mScene);
}

template<>
std::vector<TModelLoader<ModelLoaderTraitsAssimp>::MeshCreationInfo>
  TModelLoader<ModelLoaderTraitsAssimp>::getMeshes(const NodeType* node, const SceneType& scene) {

  vec<MeshCreationInfo> meshes;
  meshes.reserve(node->mNumMeshes);

  for (GLuint i = 0; i < node->mNumMeshes; ++i) {
    const auto mesh = scene.mScene->mMeshes[node->mMeshes[i]];

    if (nullptr == mesh) {
      continue;
    }

    const auto material = scene.mScene->mMaterials[mesh->mMaterialIndex];

    meshes.push_back(ModelLoader::MeshCreationInfo {
      mesh
      , getMaterial(*material)
      , getTextureName(aiTextureType_DIFFUSE, material)
      , getTextureName(aiTextureType_NORMALS, material)
      , getTextureName(aiTextureType_SPECULAR, material) } );
  }
  return meshes;
}

template<>
std::vector<TModelLoader<ModelLoaderTraitsAssimp>::NodeType*>
  TModelLoader<ModelLoaderTraitsAssimp>::getChildren(const NodeType* node) {
  std::vector<NodeType*> children;
  children.reserve(node->mNumChildren);
  for (size_t i = 0; i < node->mNumChildren; ++i) {
    children.push_back(node->mChildren[i]);
  }
  return children;
}

template<>
TModelLoader<ModelLoaderTraitsAssimp>::NodeType* TModelLoader<ModelLoaderTraitsAssimp>::getRootNode(const SceneType& scene) {
  return scene.mScene->mRootNode;
}

template
class TModelLoader<ModelLoaderTraitsAssimp>;

} /* flf */
} /* flw */
