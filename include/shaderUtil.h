#if !defined(shaderUtil_h__)
#define shaderUtil_h__

void readVertexShaderSource(const char *kernelFilename, char **kernelSource);
void readFragmentShaderSource(const char *kernelFilename, char **kernelSource);
void pexit(const char *message);
void reportShaderErrors(GLuint shader);
#endif