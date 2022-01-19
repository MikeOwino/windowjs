#include "gl.h"

#include "fail.h"

namespace {

const float kVertices[] = {
    // (x, y) positions, followed by (u, v) texture coordinates.

    -1.0f, 1.0f,  0.0f, 1.0f,  //
    -1.0f, -1.0f, 0.0f, 0.0f,  //
    1.0f,  -1.0f, 1.0f, 0.0f,  //

    -1.0f, 1.0f,  0.0f, 1.0f,  //
    1.0f,  -1.0f, 1.0f, 0.0f,  //
    1.0f,  1.0f,  1.0f, 1.0f,  //
};

const char kVertex[] = R"(
#version 330 core

in vec2 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;

uniform vec2 TexOffset;
uniform vec2 TexScale;

void main() {
  gl_Position = vec4(aPos * TexScale + TexOffset, 0, 1.0);
  TexCoords = aTexCoords;
}
)";

const char kFragment[] = R"(
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
  FragColor = texture(tex, TexCoords);
}
)";

}  // namespace

GLuint CompileShader(GLuint type, const char* source) {
  GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char log[1024];
    glGetShaderInfoLog(shader, 1024, nullptr, log);
    Fail("%s shader compile error: %s",
         type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", log);
  }

  return shader;
}

GLuint LinkShaders(GLuint vertex, GLuint fragment) {
  GLuint program = glCreateProgram();

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char log[1024];
    glGetProgramInfoLog(program, 1024, nullptr, log);
    Fail("Shader linking error: %s", log);
  }

  return program;
}

TextureShader::TextureShader() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), &kVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*) (2 * sizeof(float)));
  ASSERT_NO_GL_ERROR();

  GLuint vertex = CompileShader(GL_VERTEX_SHADER, kVertex);
  GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, kFragment);
  program_ = LinkShaders(vertex, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  ASSERT_NO_GL_ERROR();
}

TextureShader::~TextureShader() {
  glDeleteProgram(program_);
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}

void TextureShader::DrawTexture(GLuint texture, float sw, float sh, float dx,
                                float dy, bool combine_alphas) {
  if (combine_alphas) {
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA,
                        GL_DST_ALPHA);
  } else {
    glDisable(GL_BLEND);
  }

  glUseProgram(program_);
  glUniform2f(glGetUniformLocation(program_, "TexOffset"), dx, dy);
  glUniform2f(glGetUniformLocation(program_, "TexScale"), sw, sh);
  glBindVertexArray(vao_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Note: this fixes https://github.com/windowjs/windowjs/issues/35.
  glBindTexture(GL_TEXTURE_2D, 0);

  glEnable(GL_BLEND);
}
