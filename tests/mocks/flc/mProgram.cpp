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

#include <flw/flc/pipeline/Program.h>
#include <flw/flc/pipeline/Attribute.h>

namespace flw {
namespace flc {

Program::Program(const std::vector<flc::Shader*>& shaders, GLboolean skipLinking)
  : mDelayedLinking(skipLinking)
  , mShaders(shaders) {
  load();
}

Program::~Program() {
  unload();
}

void Program::attach(flc::Shader* shader) {
  if (mHandle) {

  }
}

void Program::detach(flc::Shader* shader) {
  if (mHandle) {

  }
}

GLuint Program::getHandle() const {
  return mHandle;
}

void Program::link() {

}

void Program::disusePrograms() {

}

void Program::useProgram(GLuint handle) {

}

void Program::use() const {

}

void Program::uniformPush(const std::string& name, GLint data) {

}

void Program::uniformPush(const std::string& name, GLint *data, GLint count) {

}

void Program::uniformPush(const std::string& name, GLfloat data) {

}

void Program::uniformPush(const std::string& name, GLfloat *data, GLint count) {

}

void Program::uniformPush(const std::string& name, glm::mat3 data) {

}

void Program::uniformPush(const std::string& name, glm::mat4 data) {

}

void Program::uniformPush(const std::string& name, glm::mat4 *data, GLuint size) {

}

void Program::uniformPush(const std::string& name, glm::vec2 data) {

}

void Program::uniformPush(const std::string& name, glm::vec3 data) {

}

void Program::uniformPush(const std::string& name, glm::vec3 *data, GLuint size) {

}

void Program::uniformPush(const std::string& name, glm::vec4 data) {

}

inline void Program::getUniforms() {

}

GLint Program::getUniformLocation(const std::string& name) {
  return FILLWAVE_UNIFORM_NOT_FOUND;
}

#if defined(FILLWAVE_BACKEND_OPENGL_ES_20)
#else
void Program::getUniformBlock(const std::string& name, GLuint bindingPoint) {

}

void Program::uniformBlockPush(const std::string& name, GLfloat *data, size_t size) {

}

#endif

void Program::log(const std::string& programName) const {

}

void Program::load() {

}

void Program::unload() {

}

void Program::reload() {

}

} /* flc */
} /* flw */
