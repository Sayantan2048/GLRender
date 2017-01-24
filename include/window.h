#if !defined(window_h__)
#define window_h__

#include <external/GL/glew.h> // include GLEW and new version of GL on Windows
#include <external/GLFW/glfw3.h> // GLFW helper library
#include <string>

class BaseWindow {
private:
  static unsigned int nWindows;
  std::string windowTitle;
  
  unsigned int nFrames;
  double fpsTimer;
  double fpsRefresh;
  
  //Member callbacks must be static
  static void glfwErrorCb(int error, const char* description);
protected:
  GLFWwindow* window;
  void showFps();
public:
  BaseWindow(GLuint width, GLuint height, std::string windowTitle, GLuint glVer_Maj, GLuint glVer_Min);
  void bindGlContext();
  ~BaseWindow();
  virtual void setKeyDetails(int key, int scancode, int action, int mods) = 0;
  virtual void setWindowSize(GLuint width, GLuint height) = 0;
};

#endif