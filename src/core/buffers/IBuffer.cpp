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

#include <fillwave/core/buffers/IBuffer.h>
#include <fillwave/Log.h>

FLOGINIT("Buffer", FERROR | FFATAL)

using namespace std;

namespace flw {
namespace flc {

IBuffer::IBuffer(GLuint target, GLuint drawType, GLuint index, GLsizei howMany)
    : GLObject(howMany)
    , mLoaded(GL_FALSE)
    , mIndex(index) {
  setTarget(target);
  setDrawType(drawType);
  reload();
}

IBuffer::~IBuffer() {
  glDeleteBuffers(mHowMany, mHandles);
}

void IBuffer::setElements(GLuint elements) {
  mTotalElements = elements;
}

void IBuffer::setSize(GLuint size) {
  mSize = size;
}

void IBuffer::setTarget(GLuint target) {
  if (target == GL_ARRAY_BUFFER_BINDING || target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER ||
      target == GL_UNIFORM_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER_BINDING || target == GL_PIXEL_PACK_BUFFER) {
    mTarget = target;
  } else {
    fLogE("Invalid target %d", mTarget);
  }
}

void IBuffer::setDrawType(GLuint dataStoreType) {
  if (dataStoreType == GL_STREAM_DRAW || dataStoreType == GL_STREAM_READ || dataStoreType == GL_STREAM_COPY ||
      dataStoreType == GL_STATIC_DRAW || dataStoreType == GL_STATIC_READ || dataStoreType == GL_STATIC_COPY ||
      dataStoreType == GL_DYNAMIC_DRAW || dataStoreType == GL_DYNAMIC_READ || dataStoreType == GL_DYNAMIC_COPY) {
    mDataStoreType = dataStoreType;
  } else {
    fLogE("Invalid data store type %d", mTarget);
  }
}

bool IBuffer::isLoaded() {
  return mLoaded;
}

void IBuffer::setLoaded(bool loaded) {
  mLoaded = loaded;
}

GLuint IBuffer::getElements() const {
  return mTotalElements;
}

GLuint IBuffer::getSize() const {
  return mSize;
}

GLvoid *IBuffer::getData() const {
  return mData;
}

void IBuffer::unmap() const {
  glUnmapBuffer(mTarget);
  fLogC("Could not unmap the buffer object");
}

void IBuffer::bind(GLuint id) const {
  glBindBuffer(mTarget, mHandles[id]);
  fLogC("Could not bind the buffer object");
}

void IBuffer::bindBase(GLuint id) const {
  glBindBufferBase(mTarget, mIndex, mHandles[id]);
  fLogC("Bind the uniform buffer object");
}

void IBuffer::unbind() {
  glBindBuffer(mTarget, 0);
  fLogC("Could not unbind the buffer object");
}

void IBuffer::bind(GLuint externalTarget, GLuint id) const {
  glBindBuffer(externalTarget, mHandles[id]);
  fLogC("Could not bind the buffer object");
}

void IBuffer::bindBase(GLuint externalTarget, GLuint id) const {
  glBindBufferBase(externalTarget, mIndex, mHandles[id]);
  fLogC("Bind the uniform buffer object");
}

void IBuffer::unbindBase(GLuint externalTarget) {
  glBindBufferBase(externalTarget, mIndex, 0);
  fLogC("Could not unbind the buffer object");
}

void IBuffer::send() {
  if (!mLoaded) {
    glBufferData(mTarget, mSize, mData, mDataStoreType);
    mLoaded = GL_TRUE;
    fLogC("Could not send the data");
  }
}

GLvoid *IBuffer::mapRange(GLenum access, GLuint size) {
  GLvoid *ptr = nullptr;
  if (size) {
    ptr = glMapBufferRange(mTarget, 0, size, access);
    fLogC("Could not map range of the buffer object");
  } else {
    ptr = glMapBufferRange(mTarget, 0, mSize, access);
    fLogC("Could not map range of the buffer object");
  }
  return ptr;
}

void IBuffer::reload() {
  glGenBuffers(mHowMany, mHandles);
  fLogC("reload");
}

/* Feature not available in OpenGL ES  < 3.1 */
#ifdef FILLWAVE_GLES_3_0
#else

GLvoid* IBuffer::map(GLenum access) const {
  GLvoid *ptr = glMapBuffer(mTarget, access);
  fLogC("Could not map the buffer object");
  return ptr;
}

#endif

void unbindBuffer(GLuint target) {
  glBindBuffer(target, 0);
  fLogC("Could not unbind the buffer object");
}

void bindBuffer(GLuint target, GLuint handle) {
  glBindBuffer(target, handle);
  fLogC("Could not bind the buffer object");
}

} /* flc */
} /* flw */
