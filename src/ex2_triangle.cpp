#include <external/GL/glew.h> // include GLEW and new version of GL on Windows
#include <external/GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include "shaderUtil.h"
#include "window.h"

class ColorTri: public BaseWindow {
private:
  GLuint vaoId;
  GLuint colorBufferId;
  GLuint vertexBufferId;
  
  GLuint programId;
  GLuint vertexShaderId;
  GLuint fragmentShaderId;
  
  void setKeyDetails(int key, int scancode, int action, int mods){};
  void setWindowSize(GLuint width, GLuint height) {glViewport(0, 0, width, height);};
  void createBuffer();
  void destroyBuffer();
  void createShader();
  void destroyShader();
public:
  ColorTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min);
  ~ColorTri()
  { 
    destroyBuffer();
    destroyShader();
  }
  void render() {
    while(!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // update other events like input handling 
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    showFps();
    }
  }
  void bind();
};

void ColorTri::createBuffer()
{
  GLfloat Vertices[] = {
    -0.8f, -0.8f, 0.0f, 1.0f,
     0.0f,  0.8f, 0.0f, 1.0f,
     0.8f, -0.8f, 0.0f, 1.0f
  };

  GLfloat Colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
  };
  
  glGenVertexArrays(1, &vaoId);
  glBindVertexArray(vaoId); //Bind vaoId as input to GPU.

   glGenBuffers(1, &vertexBufferId);
  // glBindBuffer(set transfer destination) and glBufferData(do the transfer) 
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId); //Bind GL_ARRAY_BUFFER with vertexBufferId
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // Transfer data to currently binded buffer of type GL_ARRAY_BUFFER
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); // 0th input to vertex shader
  glEnableVertexAttribArray(0);
  
  glGenBuffers(1, &colorBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);// 1st input to vertex shader
  glEnableVertexAttribArray(1);
  
  reportGlError("Failed to create buffer.");
}

void ColorTri::destroyBuffer(void)
{
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDeleteBuffers(1, &colorBufferId);
  glDeleteBuffers(1, &vertexBufferId);

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
}

ColorTri::ColorTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min):BaseWindow(width, height, windowName, glVer_Maj, glVer_Min) 
{
  createBuffer();
  createShader();  
}

int main() {
  ColorTri w1(640, 480, "A triangle", 4, 0);
  w1.bind();
   
  w1.render();
  return 0;
}