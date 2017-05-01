#if !defined(shaderUtil_h__)
#define shaderUtil_h__

void readVertexShaderSource(const char *kernelFilename, char **kernelSource);
void readFragmentShaderSource(const char *kernelFilename, char **kernelSource);
void readGeometryShaderSource(const char *kernelFilename, char **kernelSource); 
void pexit(std::string message);
void reportShaderErrors(GLuint shader);
bool reportGlError(std::string message);
#endif