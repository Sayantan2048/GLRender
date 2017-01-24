#include <external/GL/glew.h> // extension handling.
#include <external/GLFW/glfw3.h> // window library.
#include <external/glm/vec4.hpp> //vec4
#include <stdio.h>
#include <iostream>
#include "shaderUtil.h"
#include "window.h"
using namespace glm;

// Use IBOs to draw complex shape 1. Using triangles 2. Using triangle strips.
// Note to self:
// To change vertex buffer, glBindVertexArray, this holds vertex information
// To change shader, glUseProgram
// To change ibo, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ), how vertices are connected.
// It is possible to change the geometry by simply changing the ibo while keeping vao same. 
// For using indices, use glDrawElements instead of glDrawArrays.
class ColorTri: public BaseWindow {
private:
  GLuint vaoId;
  GLuint vertexBufferId;
  GLuint indexBufferId;
  
  GLuint programId;
  GLuint vertexShaderId;
  GLuint fragmentShaderId;
  
  void setKeyDetails(int key, int scancode, int action, int mods){};
  void setWindowSize(GLuint width, GLuint height) {glViewport(0, 0, width, height);};
  void createBuffer1(); //Create buffer using ibo and triangles.
  void createBuffer2(); //Create buffer using ibo and triangle_strips.
  void destroyBuffer1();
  void createShader();
  void destroyShader();
public:
  ColorTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min);
  ~ColorTri()
  { 
    destroyBuffer1();
    destroyShader();
  }
  // Render using triangle
  void render1() {
    while(!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw points 0-48 from the currently bound IBO with current in-use shader
    glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, (GLvoid*)0);
    // update other events like input handling 
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    showFps();
    }
  }
  
  // Render using triangle_strips
  void render2() {
    while(!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw points 0-28 from the currently bound IBO with current in-use shader
    glDrawElements(GL_TRIANGLE_STRIP, 28, GL_UNSIGNED_BYTE, (GLvoid*)0);
    // update other events like input handling 
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    showFps();
    }
  }
  void bind();
};

//Complex, setup for triangle
void ColorTri::createBuffer1()
{
  typedef struct {
    vec4 position;
    vec4 color;
  } Vertex;
  
  Vertex Vertices[] = {
  { vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },
  // Top
  { vec4(-0.2f, 0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(0.2f, 0.8f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(0.0f, 0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(0.0f, 1.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  // Bottom
  { vec4(-0.2f, -0.8f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(0.2f, -0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(0.0f, -0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(0.0f, -1.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  // Left
  { vec4(-0.8f, -0.2f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(-0.8f, 0.2f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(-0.8f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(-1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  // Right
  { vec4(0.8f, -0.2f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(0.8f, 0.2f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(0.8f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) }
  };
  
  GLubyte Indices[] = {
	// Top
	0, 1, 3,
	0, 3, 2,
	3, 1, 4,
	3, 4, 2,
	// Bottom
	0, 5, 7,
	0, 7, 6,
	7, 5, 8,
	7, 8, 6,
	// Left
	0, 9, 11,
	0, 11, 10,
	11, 9, 12,
	11, 12, 10,
	// Right
	0, 13, 15,
	0, 15, 14,
	15, 13, 16,
	15, 16, 14
 };
  
  const size_t BufferSize = sizeof(Vertices);
  const size_t VertexSize = sizeof(Vertices[0]);
  const size_t RgbOffset = sizeof(Vertices[0].position);

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);

  glGenBuffers(1, &vertexBufferId);
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  glGenBuffers(1, &indexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  
  reportGlError("Failed to create buffer1.");
}

// Using triangle_strips require change only in Indices.
void ColorTri::createBuffer2()
{
  typedef struct {
    vec4 position;
    vec4 color;
  } Vertex;
  
  Vertex Vertices[] = {
  { vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },
  // Top
  { vec4(-0.2f, 0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(0.2f, 0.8f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(0.0f, 0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(0.0f, 1.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  // Bottom
  { vec4(-0.2f, -0.8f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(0.2f, -0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(0.0f, -0.8f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(0.0f, -1.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  // Left
  { vec4(-0.8f, -0.2f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(-0.8f, 0.2f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(-0.8f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(-1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  // Right
  { vec4(0.8f, -0.2f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { vec4(0.8f, 0.2f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { vec4(0.8f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
  { vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) }
  };
  
  GLubyte Indices[] = {
	// Top
	0, 1, 3, 4, 3, 2, 0,
	//Bottom
	0, 6, 7, 8, 7, 5, 0,
	//Left
	0, 9, 11, 12, 11, 10, 0,
	//Right
	0, 14, 15, 16, 15, 13, 0
  };
  
  const size_t BufferSize = sizeof(Vertices);
  const size_t VertexSize = sizeof(Vertices[0]);
  const size_t RgbOffset = sizeof(Vertices[0].position);

  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId);

  glGenBuffers(1, &vertexBufferId);
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  glGenBuffers(1, &indexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  
  reportGlError("Failed to create buffer2.");
}

void ColorTri::destroyBuffer1(void)
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

void ColorTri::createShader()
{
  char* vertexShaderSrc = 0;
  char* fragmentShaderSrc = 0;
  
  readVertexShaderSource("2.vert", &vertexShaderSrc);
  readFragmentShaderSource("2.frag", &fragmentShaderSrc);
  
  vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShaderId);
  reportShaderErrors(vertexShaderId);
  
  fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShaderId);
  reportShaderErrors(vertexShaderId);

  programId = glCreateProgram();
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  glUseProgram(programId); // This set of vertex and fragment shader will be used during rendering.
  
  reportGlError("Failed to create shader.");
}

void ColorTri::destroyShader(void)
{
  glUseProgram(0);
   
  glDetachShader(programId, vertexShaderId);
  glDetachShader(programId, fragmentShaderId);

  glDeleteShader(fragmentShaderId);
  glDeleteShader(vertexShaderId);

  glDeleteProgram(programId);
  
  reportGlError("Failed to delete shader.");
}

void ColorTri::bind()
{
  bindGlContext();
  glBindVertexArray(vaoId);
  glUseProgram(programId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
}

ColorTri::ColorTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min):BaseWindow(width, height, windowName, glVer_Maj, glVer_Min) 
{
  createBuffer2();
  createShader();  
}

int main() {
  ColorTri w1(640, 480, "A triangle", 4, 0);
  w1.bind();
   
  w1.render2();
  return 0;
}