#include <external/GL/glew.h> // include GLEW and new version of GL on Windows
#include <external/GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include "shaderUtil.h"
#include "window.h"
// Basic triangle drawing example.

class RenderTri: public BaseWindow {
private:
 void setKeyDetails(int key, int scancode, int action, int mods);
 void setWindowSize(GLuint width, GLuint height) {glViewport(0, 0, width, height);};
 void bind() {};
public:
  GLuint shader_programme;
  GLuint vao;
  RenderTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min);
  void render() {
    while(!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // update other events like input handling 
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
    showFps();
    }
  }
};

void RenderTri:: setKeyDetails(int key, int scancode, int action, int mods)
{
  std::cout<<"Hi"<<std::endl;
}

RenderTri::RenderTri(GLuint width, GLuint height, const char *windowName, GLuint glVer_Maj, GLuint glVer_Min):BaseWindow(width, height, windowName, glVer_Maj, glVer_Min) 
{
    float points[] = {
      0.0f,  0.5f,  0.0f,
      0.5f, -0.5f,  0.0f,
      -0.5f, -0.5f,  0.0f
    };
  
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
  
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  
    char* vertex_shader = 0;
    char* fragment_shader = 0;
  
    readVertexShaderSource("1.vert", &vertex_shader);
    readFragmentShaderSource("1.frag", &fragment_shader);
   
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    reportShaderErrors(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    reportShaderErrors(fs);
  
    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
}

int main() {
  RenderTri w1(640, 480, "A triangle", 4, 0);
  RenderTri w2(640, 480, "A triangle2", 4, 0);
  w2.bindGlContext();
   
  
  w2.render();
  
  
  return 0;
}