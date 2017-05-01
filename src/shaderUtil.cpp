#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "external/GL/glew.h"
#include "GL/glu.h"
#include "shaderUtil.h"

void reportShaderErrors(GLuint shader)
{
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  
  if (success == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

    std::cerr<<&errorLog[0]<<std::endl;
  }
}

void pexit(std::string message)
{
  std::cerr<<message<<std::endl;
  exit(0);
}

static void readSource(const char *kernelFilename, char **kernelSource)
{
  FILE *fp;
  size_t sourceSize, readSize;

  fp = fopen(kernelFilename, "rb");
	
  if (!fp)
    pexit("Can't read source kernel");

  fseek(fp, 0, SEEK_END);
  sourceSize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  if ((*kernelSource) != NULL)
    free((*kernelSource));
  
  *kernelSource = (char *)malloc(sourceSize + 1);
  readSize = fread(*kernelSource, sizeof(char), sourceSize, fp);
  if (readSize != sourceSize) {
    fprintf(stderr,
      "Error reading source: expected %Zu, got %Zu bytes.\n",
	sourceSize, readSize);
    pexit("Can't read source kernel");
  }
  
  fclose(fp);
}

void readVertexShaderSource(const char *kernelFilename, char **kernelSource) {
  char fullPath[200];
  sprintf(fullPath, "./src/shaders/vertex/%s", kernelFilename);
  readSource(fullPath, kernelSource);
}

void readFragmentShaderSource(const char *kernelFilename, char **kernelSource) {
  char fullPath[200];
  sprintf(fullPath, "./src/shaders/fragment/%s", kernelFilename);
  readSource(fullPath, kernelSource);
}

void readGeometryShaderSource(const char *kernelFilename, char **kernelSource) {
  char fullPath[200];
  sprintf(fullPath, "./src/shaders/geometry/%s", kernelFilename);
  readSource(fullPath, kernelSource);
}

bool reportGlError(std::string message)
{
  GLenum errorCheckValue = glGetError();
  if (errorCheckValue != GL_NO_ERROR) {
    std::cerr<<message<<":"<<errorCheckValue<<std::endl;
    return false;
  }
  return true;
}
