#if !defined(shaderClass_h__)
#define shaderClass_h__

#include <external/GL/glew.h> // extension handling.
#include <iostream>

class Shader {
  GLuint vertexShaderId;
  GLuint geometryShaderId;
  GLuint fragmentShaderId;
  GLuint programId;
  bool verbose;
  bool inUse;
public:
  Shader(std::string vertName, std::string geomName, std::string fragName, bool verbose) {
      if (verbose)
	std::cout<<"Creating Shader..."<<std::endl;
      this->verbose = verbose;
      
      vertexShaderId = 0;
      geometryShaderId = 0;
      fragmentShaderId = 0;
      programId = 0;
      
      programId = glCreateProgram();
      if (vertName.length() >= 1) {
	char *vertexShaderSrc = 0;
	readVertexShaderSource(vertName.c_str(), &vertexShaderSrc);
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderId);
	reportShaderErrors(vertexShaderId);
	glAttachShader(programId, vertexShaderId);
      }
      if (geomName.length() >= 1) {
	char *geomShaderSrc = 0;
	readGeometryShaderSource(geomName.c_str(), &geomShaderSrc);
	geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShaderId, 1, &geomShaderSrc, NULL);
	glCompileShader(geometryShaderId);
	reportShaderErrors(geometryShaderId);
	glAttachShader(programId, geometryShaderId);
      }
      if (fragName.length() >= 1) {
	char *fragShaderSrc = 0;
	readFragmentShaderSource(fragName.c_str(), &fragShaderSrc);
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShaderId);
	reportShaderErrors(fragmentShaderId);
	glAttachShader(programId, fragmentShaderId);
      }
       
      glLinkProgram(programId);
      inUse = false;
      
      reportGlError("Failed to create shader.");
  }
  
  void use() {
      glUseProgram(programId);
      inUse = true;
  }
  
  void stop() {
      glUseProgram(0);
      inUse = false;
  }
  
  GLint getUniformLocation(std::string uniformName) {
      GLuint id = glGetUniformLocation(programId, uniformName.c_str());
      reportGlError("Failed to get Uniform Location.");
      return id;
  }
  
  ~Shader() {
      if (verbose)
	std::cout<<"Destroying Shader..."<<std::endl;
      
      if (inUse)
	std::cout<<"Shader is still in Use. Unable to destroy shader."<<std::endl;
      else {
	if (vertexShaderId) {
	  glDetachShader(programId, vertexShaderId);
	  glDeleteShader(vertexShaderId);
	}
	
	if (geometryShaderId) {
	  glDetachShader(programId, geometryShaderId);
	  glDeleteShader(geometryShaderId);
	}
	
	if (fragmentShaderId) {
	  glDetachShader(programId, fragmentShaderId);
	  glDeleteShader(fragmentShaderId);
	}
	if (programId)
	  glDeleteProgram(programId);
  
	reportGlError("Failed to delete shader.");
      }
  }
};

#endif