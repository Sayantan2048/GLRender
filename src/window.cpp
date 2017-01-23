#include "window.h"
#include "shaderUtil.h"
#include <iostream>
#include <map>
#include <sstream>

unsigned int BaseWindow::nWindows = 0;
static std::map<GLFWwindow*, BaseWindow*> windowMap;

static void glfwWindowResizeCb(GLFWwindow* window, int width, int height)
{
  BaseWindow *ptr = windowMap[window];
  ptr->setWindowSize(width, height);
}

static void glfwKeyCb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  BaseWindow *ptr = windowMap[window];
  ptr->setKeyDetails(key, scancode, action, mods);  
}

BaseWindow::BaseWindow(GLuint width, GLuint height, const char *windowTitle, GLuint glVer_Maj = 4, GLuint glVer_Min = 0) {
  if (nWindows == 0)
    glfwSetErrorCallback(glfwErrorCb);
  
  if (nWindows == 0 && !glfwInit())
    pexit("ERROR: could not start GLFW3.");
      
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVer_Maj);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVer_Min);
  
  // uncomment these lines if on Apple OS X
  /* glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
  window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
  if (nWindows == 0 && !window) {
    glfwTerminate();
    pexit("ERROR: could not open window with GLFW3.");
  }
  
  glfwSetWindowSizeCallback(window, glfwWindowResizeCb);
  glfwSetKeyCallback(window, glfwKeyCb);
  
  //Initialize context as the glewInit() needs a current context to load from.
  glfwMakeContextCurrent(window);
  
  // start GLEW extension handler
  if (nWindows == 0) {
    GLenum glewSuccess;
    
    glewExperimental = GL_TRUE;
    glewSuccess = glewInit();
    
    if (GLEW_OK != glewSuccess) {
      glfwDestroyWindow(window);
      glfwTerminate();
      pexit((const char *)glewGetErrorString(glewSuccess));
    }
        
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout<<"Renderer: "<<renderer<<std::endl;
    std::cout<<"OpenGL version supported: "<<version<<std::endl;
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  //For FPS counter.
  nFrames = 0;
  fpsRefresh = 0.25;
  fpsTimer = glfwGetTime();
  
  this->windowTitle = windowTitle;
  
  windowMap[window] = this;
  nWindows++;
}

void BaseWindow::showFps()
{
  double currentTime = glfwGetTime();
  nFrames++;

  if (currentTime - fpsTimer >= fpsRefresh) {
    std::stringstream title;
    title<<windowTitle<<" FPS:"<<double(nFrames)/(currentTime -fpsTimer);
    glfwSetWindowTitle(window, title.str().c_str());

    nFrames = 0;
    fpsTimer = glfwGetTime();
  }
}

void BaseWindow::glfwErrorCb(int error, const char* description)
{
  switch(error) {
    case GLFW_NOT_INITIALIZED: std::cerr<<"GLFW_NOT_INITIALIZED:"<<description<<std::endl; break;
    case GLFW_NO_CURRENT_CONTEXT: std::cerr<<"GLFW_NO_CURRENT_CONTEXT:"<<description<<std::endl; break;
    case GLFW_INVALID_ENUM: std::cerr<<"GLFW_INVALID_ENUM:"<<description<<std::endl; break;
    case GLFW_INVALID_VALUE: std::cerr<<"GLFW_INVALID_VALUE:"<<description<<std::endl; break;
    case GLFW_OUT_OF_MEMORY: std::cerr<<"GLFW_OUT_OF_MEMORY:"<<description<<std::endl; break;
    case GLFW_API_UNAVAILABLE: std::cerr<<"GLFW_API_UNAVAILABLE:"<<description<<std::endl; break;
    case GLFW_VERSION_UNAVAILABLE: std::cerr<<"GLFW_VERSION_UNAVAILABLE:"<<description<<std::endl; break;
    case GLFW_PLATFORM_ERROR: std::cerr<<"GLFW_PLATFORM_ERROR:"<<description<<std::endl; break;
    case GLFW_FORMAT_UNAVAILABLE: std::cerr<<"GLFW_FORMAT_UNAVAILABLE:"<<description<<std::endl; break;
    default: std::cerr<<"Urecognised error:"<<description<<std::endl; break;
  }
}

void BaseWindow::bindGlContext()
{
  glfwMakeContextCurrent(window);
}

BaseWindow::~BaseWindow()
{ 
  windowMap.erase(window);
  glfwDestroyWindow(window);
  
  if (nWindows == 1)
    // close GL context and any other GLFW resources
    glfwTerminate();
  nWindows--;
}
