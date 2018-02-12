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

#include <fillwave/core/buffers/VertexBufferBasic.h>

#ifdef FILLWAVE_MODEL_LOADER_ASSIMP

#include <fillwave/models/animations/Animator.h>

#endif

#include <fillwave/Log.h>

FLOGINIT("VertexBufferBasic", FERROR | FFATAL)

namespace flw {
namespace flc {

#ifdef FILLWAVE_MODEL_LOADER_ASSIMP

VertexBufferBasic::VertexBufferBasic(const aiMesh *shape, flf::Animator *animator, GLuint dataStoreModification)
    : TVertexBuffer<VertexBasic>(dataStoreModification) {

  mTotalElements = shape->mNumVertices;
  mDataVertices.resize(mTotalElements);

  {
#pragma omp parallel for schedule(guided) num_threads(2) if (mTotalElements > 1000)
    for (GLuint i = 0; i < mTotalElements; i++) {
      VertexBasic &vertex = mDataVertices[i];

      if (shape->HasVertexColors(0)) {
        vertex.mColor[0] = shape->mColors[0]->r;
        vertex.mColor[1] = shape->mColors[0]->g;
        vertex.mColor[2] = shape->mColors[0]->b;
        vertex.mColor[3] = shape->mColors[0]->a;
      } else {
        vertex.mColor[0] = 0.0f;
        vertex.mColor[1] = 0.0f;
        vertex.mColor[2] = 0.0f;
        vertex.mColor[3] = 1.0f;
      }

      vertex.mPosition[0] = shape->mVertices[i].x;
      vertex.mPosition[1] = shape->mVertices[i].y;
      vertex.mPosition[2] = shape->mVertices[i].z;
      vertex.mPosition[3] = 1.0f;

      /* One normal each triangle - on */
      if (shape->HasNormals()) {
        vertex.mNormal[0] = shape->mNormals[i].x;
        vertex.mNormal[1] = shape->mNormals[i].y;
        vertex.mNormal[2] = shape->mNormals[i].z;
      } else {
        vertex.mNormal[0] = 0;
        vertex.mNormal[1] = 0;
        vertex.mNormal[2] = 0;
      }

      if (shape->HasTextureCoords(0)) { //xxx what is this ?
        vertex.mTextureUV[0] = shape->mTextureCoords[0][i].x;
        vertex.mTextureUV[1] = shape->mTextureCoords[0][i].y;
      } else {
        vertex.mTextureUV[0] = 0;
        vertex.mTextureUV[1] = 0;
      }

      if (shape->HasTangentsAndBitangents()) {
        vertex.mNormalTangentMap[0] = shape->mTangents[i].x;
        vertex.mNormalTangentMap[1] = shape->mTangents[i].y;
        vertex.mNormalTangentMap[2] = shape->mTangents[i].z;
      } else {
        vertex.mNormalTangentMap[0] = 0;
        vertex.mNormalTangentMap[1] = 0;
        vertex.mNormalTangentMap[2] = 0;
      }
      for (int k = 0; k < FILLWAVE_MAX_BONES_DEPENDENCIES; k++) {
        vertex.mBoneID[k] = 0;
        vertex.mBoneWeight[k] = 0.0f;
      }
    }
  }

  mData = mDataVertices.data();
  mSize = mTotalElements * sizeof(VertexBasic);

  if (animator) {
    std::vector<int> boneIdForEachVertex;
    boneIdForEachVertex.resize(mDataVertices.size());
    for (size_t z = 0; z < mDataVertices.size(); z++) {
      boneIdForEachVertex[z] = 0;
    }
    /* Bones */
    for (GLuint i = 0; i < shape->mNumBones; i++) {
      for (GLuint j = 0; j < shape->mBones[i]->mNumWeights; j++) {
        GLuint VertexID = shape->mBones[i]->mWeights[j].mVertexId;
        float Weight = shape->mBones[i]->mWeights[j].mWeight;
        if (boneIdForEachVertex[VertexID] < FILLWAVE_MAX_BONES_DEPENDENCIES) {
          mDataVertices[VertexID].mBoneID[boneIdForEachVertex[VertexID]] = animator->getId(shape->mBones[i]->mName.C_Str());
          mDataVertices[VertexID].mBoneWeight[boneIdForEachVertex[VertexID]] = Weight;
          boneIdForEachVertex[VertexID]++;
        } else {
          fLogF("Crater can handle maximum %d bone dependencies.", FILLWAVE_MAX_BONES_DEPENDENCIES);
        }
      }
    }
  }
}

#else
VertexBufferBasic::VertexBufferBasic(tinyobj::shape_t& shape,
                         tinyobj::attrib_t& attr,
                         GLuint dataStoreModification) :
  TVertexBuffer<VertexBasic>(dataStoreModification) {

  mTotalElements = shape.mesh.indices.size();
  mDataVertices.resize(mTotalElements);

  // Loop over shapes
  // Loop over faces(polygon)
  size_t index_offset = 0;
  /* xxx OpenMP should be usefull here */
  for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
    int fv = shape.mesh.num_face_vertices[f];

    // Loop over vertices in the face.
    for (size_t v = 0; v < fv; v++) {
      // access to vertex
      size_t idxOut = index_offset + v;
      tinyobj::index_t idx = shape.mesh.indices[idxOut];
      mDataVertices[idxOut].mPosition[0] = attr.vertices[3 * idx.vertex_index + 0];
      mDataVertices[idxOut].mPosition[1] = attr.vertices[3 * idx.vertex_index + 1];
      mDataVertices[idxOut].mPosition[2] = attr.vertices[3 * idx.vertex_index + 2];
      mDataVertices[idxOut].mPosition[3] = 1.0f;
      mDataVertices[idxOut].mNormal[0] = attr.normals[3 * idx.normal_index + 0];
      mDataVertices[idxOut].mNormal[1] = attr.normals[3 * idx.normal_index + 1];
      mDataVertices[idxOut].mNormal[2] = attr.normals[3 * idx.normal_index + 2];
      mDataVertices[idxOut].mTextureUV[0] = attr.texcoords[2 * idx.texcoord_index +
                                0];
      mDataVertices[idxOut].mTextureUV[1] = attr.texcoords[2 * idx.texcoord_index +
                                1];
      mDataVertices[idxOut].mNormalTangentMap[0] = 0.0f;
      mDataVertices[idxOut].mNormalTangentMap[1] = 0.0f;
      mDataVertices[idxOut].mNormalTangentMap[2] = 0.0f;
      mDataVertices[idxOut].mColor[0] = 0.0f;
      mDataVertices[idxOut].mColor[1] = 0.0f;
      mDataVertices[idxOut].mColor[2] = 0.0f;
      mDataVertices[idxOut].mColor[3] = 1.0f;
      for (int k = 0; k < FILLWAVE_MAX_BONES_DEPENDENCIES; k++) {
        mDataVertices[idxOut].mBoneID[k] = 0.0f;
        mDataVertices[idxOut].mBoneWeight[k] = 0.0f;
      }
    }
    index_offset += fv;

    /* per-face material */
    /* shape.mesh.material_ids[f]; */
  }
  mData = mDataVertices.data();
  mSize = mTotalElements * sizeof(VertexBasic);
}

#endif /* FILLWAVE_MODEL_LOADER_ASSIMP */

VertexBufferBasic::VertexBufferBasic(flf::TerrainConstructor *constructor,
    GLint chunkDensity,
    GLfloat gapSize,
    const std::vector<GLuint> &indices,
    GLuint dataStoreModification)
    : TVertexBuffer<VertexBasic>(dataStoreModification) {

  flc::VertexBasic vertex;

  for (float z = 0; z <= chunkDensity; ++z) {
    for (float x = 0; x <= chunkDensity; ++x) {
      vertex.mColor[0] = 0.0f;
      vertex.mColor[1] = 0.0f;
      vertex.mColor[2] = 0.0f;
      vertex.mColor[3] = 1.0f;
      vertex.mPosition[0] = gapSize * (x - chunkDensity / 2);
      vertex.mPosition[2] = gapSize * (z - chunkDensity / 2);
      vertex.mPosition[3] = 1.0;
      vertex.mTextureUV[0] = x / chunkDensity;
      vertex.mTextureUV[1] = z / chunkDensity;
      vertex.mPosition[1] = constructor->calculateHeight(vertex.mTextureUV[0], vertex.mTextureUV[1]);
      mDataVertices.push_back(vertex);
    }
  }

  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> tangents;

  for (size_t i = 0; i < mDataVertices.size(); i++) {
    normals.push_back(glm::vec3(0.0));
    tangents.push_back(glm::vec3(0.0));
  }

  int j, z;

  for (size_t i = 0; i < indices.size(); i += 3) {

    /* Normals */
    j = i + 1;
    z = i + 2;

    const GLuint iIdx = indices[i];
    const GLuint jIdx = indices[j];
    const GLuint zIdx = indices[z];

    glm::vec3 v0(mDataVertices[iIdx].mPosition[0], mDataVertices[iIdx].mPosition[1], mDataVertices[iIdx].mPosition[2]);
    glm::vec3 v1(mDataVertices[jIdx].mPosition[0], mDataVertices[jIdx].mPosition[1], mDataVertices[jIdx].mPosition[2]);
    glm::vec3 v2(mDataVertices[zIdx].mPosition[0], mDataVertices[zIdx].mPosition[1], mDataVertices[zIdx].mPosition[2]);

    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

    normals[iIdx] += normal;
    normals[jIdx] += normal;
    normals[zIdx] += normal;

    /* Tangents */
    glm::vec3 deltaPosition;
    if (v0 == v1) {
      deltaPosition = v2 - v0;
    } else {
      deltaPosition = v1 - v0;
    }

    glm::vec2 deltaUV1(mDataVertices[jIdx].mTextureUV[0] - mDataVertices[iIdx].mTextureUV[0],
                       mDataVertices[jIdx].mTextureUV[1] - mDataVertices[iIdx].mTextureUV[1]);

    glm::vec3 tangent = deltaPosition / ( deltaUV1.s != 0.0f ? deltaUV1.s : 1.0f );
    tangent = glm::normalize(tangent - glm::dot(normal, tangent) * normal);

    tangents[iIdx] += tangent;
    tangents[jIdx] += tangent;
    tangents[indices[z]] += tangent;
  }

  for (size_t i = 0; i < indices.size(); ++i) {
    glm::vec3 vector3_n = glm::normalize(normals[indices[i]]);
    glm::vec3 vector3_t = glm::normalize(tangents[indices[i]]);
    mDataVertices[indices[i]].mNormal[0] = vector3_n.x;
    mDataVertices[indices[i]].mNormal[1] = vector3_n.y;
    mDataVertices[indices[i]].mNormal[2] = vector3_n.z;
    mDataVertices[indices[i]].mNormalTangentMap[0] = vector3_t.x;
    mDataVertices[indices[i]].mNormalTangentMap[1] = vector3_t.y;
    mDataVertices[indices[i]].mNormalTangentMap[2] = vector3_t.z;
  }

  mTotalElements = mDataVertices.size();
  mData = mDataVertices.data();
  mSize = mTotalElements * sizeof(VertexBasic);
}

VertexBufferBasic::VertexBufferBasic(const std::vector<flc::VertexBasic> &vertices, GLuint dataStoreModification)
    : TVertexBuffer<VertexBasic>(vertices, dataStoreModification) {

}

glm::vec3 VertexBufferBasic::getOcclusionBoxSize() {
  glm::vec3 maximum(-10000.0, -10000.0, -10000.0);
  glm::vec3 minimum(10000.0, 10000.0, 10000.0);

  for (GLuint i = 0; i < mTotalElements; ++i) {
    if (mDataVertices[i].mPosition[0] > maximum.x) {
      maximum.x = mDataVertices[i].mPosition[0];
    }
    if (mDataVertices[i].mPosition[1] > maximum.y) {
      maximum.y = mDataVertices[i].mPosition[1];
    }
    if (mDataVertices[i].mPosition[2] > maximum.z) {
      maximum.z = mDataVertices[i].mPosition[2];
    }
    if (mDataVertices[i].mPosition[0] < minimum.x) {
      minimum.x = mDataVertices[i].mPosition[0];
    }
    if (mDataVertices[i].mPosition[1] < minimum.y) {
      minimum.y = mDataVertices[i].mPosition[1];
    }
    if (mDataVertices[i].mPosition[2] < minimum.z) {
      minimum.z = mDataVertices[i].mPosition[2];
    }
  }

  glm::vec3 result = maximum - minimum;

  if (result.x > result.y) {
    result = result.x > result.z ? glm::vec3(result.x) : glm::vec3(result.z);
  } else {
    result = result.y > result.z ? glm::vec3(result.y) : glm::vec3(result.z);
  }
  return result * 0.5f;
}

void VertexBufferBasic::log() const {
  for (auto it : mDataVertices) {
    fLogI("Vertex UV: ", it.mTextureUV[0], " ", it.mTextureUV[1]);
    fLogI("Vertex normal: ",it.mNormal[0], " ", it.mNormal[1], " ", it.mNormal[2]);
    fLogI("Vertex position: ",it.mPosition[0], " ", it.mPosition[1], " ", it.mPosition[2]);
  }
}

} /* flc */
} /* flw */
