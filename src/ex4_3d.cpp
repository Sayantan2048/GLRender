#include <external/GL/glew.h> // extension handling.
#include <external/GLFW/glfw3.h> // window library.
#include <external/glm/vec4.hpp> //vec4
#include <external/glm/vec3.hpp> //vec4
#include <external/glm/mat4x4.hpp> //mat4
#include <external/glm/gtc/matrix_transform.hpp> //lookAt, perspective
#include <external/glm/gtc/type_ptr.hpp> //value_ptr
#include <stdio.h>
#include <iostream>
#include "shaderUtil.h"
#include "window.h"
#include "shaderClass.h"

using namespace glm;

class ColorTri: public BaseWindow {
private:
  GLuint vaoId;
  GLuint vertexBufferId;
  GLuint indexBufferId;
  
  Shader *shader;
  
  mat4 Model;
  mat4 Camera;
  mat4 Projection;
   
  GLuint ModelUniformLocation;
  GLuint CameraUniformLocation;
  GLuint ProjectionUniformLocation;
  
  void setKeyDetails(int key, int scancode, int action, int mods);
  void setWindowSize(GLuint width, GLuint height);
  void createBuffer();
  void destroyBuffer();
public:
  ColorTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min);
  ~ColorTri()
  { 
    destroyBuffer();
    shader->stop();
    delete shader;
  }
  void render() {
    while(!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw points 0-48 from the currently bound IBO with current in-use shader
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
    // update other events like input handling 
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    showFps();
    }
  }
  void bind();
};

void ColorTri::setWindowSize(GLuint width, GLuint height)
{
  glViewport(0, 0, width, height);
  Projection = perspective(radians(45.0f), float(width) / float(height), 0.1f, 100.f);
  shader->use();
  glUniformMatrix4fv(ProjectionUniformLocation, 1, GL_FALSE, value_ptr(Projection)); //Transfer matrix data to GPU.
  shader->stop();
}

void ColorTri::setKeyDetails(int key, int scancode, int action, int mods)
{
  
}

void ColorTri::createBuffer()
{
  typedef struct {
    vec4 position;
    vec4 color;
  } Vertex;
  
  Vertex Vertices[8] = {
    { vec4(-.5f, -.5f,  .5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(-.5f,  .5f,  .5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(.5f,  .5f,  .5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(.5f, -.5f,  .5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(-.5f, -.5f, -.5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(-.5f,  .5f, -.5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(.5f,  .5f, -.5f, 1), vec4(1, 0.8, 0.8, 1) },
    { vec4(.5f, -.5f, -.5f, 1), vec4(1, 0.8, 0.8, 1) }
  };

  GLubyte Indices[36] = {
    0,2,1,  0,3,2,
    4,3,0,  4,7,3,
    4,1,5,  4,0,1,
    3,6,2,  3,7,6,
    1,6,5,  1,2,6,
    7,5,6,  7,4,5
  };
  
  const size_t BufferSize = sizeof(Vertices);
  const size_t VertexSize = sizeof(Vertices[0]);
  const size_t RgbOffset = sizeof(Vertices[0].position);

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);

  glCreateBuffers(1, &vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferStorage(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_DYNAMIC_STORAGE_BIT);
  
  glVertexArrayVertexBuffer(vaoId, 0, vertexBufferId, 0, VertexSize);
  
  glVertexArrayAttribBinding(vaoId, 0, 0);
  glVertexArrayAttribBinding(vaoId, 1, 0);
  
  glVertexArrayAttribFormat(vaoId, 0, 4, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribFormat(vaoId, 1, 4, GL_FLOAT, GL_FALSE, RgbOffset);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  glCreateBuffers(1, &indexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_DYNAMIC_STORAGE_BIT);
  
  reportGlError("Failed to create buffer.");
}

void ColorTri::destroyBuffer(void)
{
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vertexBufferId);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &indexBufferId);
  
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vaoId);
  
  reportGlError("Failed to delete buffer.");
}

void ColorTri::bind()
{
  bindGlContext();
  glBindVertexArray(vaoId);
  shader->use();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
}

ColorTri::ColorTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min):BaseWindow(width, height, windowName, glVer_Maj, glVer_Min) 
{
  createBuffer();
  shader = new Shader("3.vert", "1.geom", "2.frag",  true);
  Model = translate(mat4(1.0f), vec3(0.0f, 0.0f, -10)) * rotate(mat4(1.0f), radians(30.0f), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(30.0f), vec3(0.0f, 1.0f, 0.0f)); //matrix, angle, axis(normalized)
  Camera = lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));//eye, center, up(normalized)
  Projection = perspective(radians(45.0f), float(width) / float(height), 0.1f, 100.f);
  
  ModelUniformLocation = shader->getUniformLocation("ModelMatrix"); 
  CameraUniformLocation = shader->getUniformLocation("ViewMatrix");
  ProjectionUniformLocation = shader->getUniformLocation("ProjectionMatrix");
  
  shader->use();
  glUniformMatrix4fv(ModelUniformLocation, 1, GL_FALSE, value_ptr(Model)); //Transfer matrix data to GPU.
  glUniformMatrix4fv(CameraUniformLocation, 1, GL_FALSE, value_ptr(Camera)); //Transfer matrix data to GPU.
  glUniformMatrix4fv(ProjectionUniformLocation, 1, GL_FALSE, value_ptr(Projection));
  shader->stop();
}

int main() {
  ColorTri w1(640, 480, "A triangle", 4, 0);
  w1.bind();
   
  w1.render();
  return 0;
}