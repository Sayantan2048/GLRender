#include <external/glm/vec4.hpp>
#include <external/glm/mat4x4.hpp>

struct Vertex {
  vec4 v;
  vec4 color;
}

struct Triangle {
  
}

class RigidObject {
    GLubyte *indices; // link vertex indices to form triangles
    vec4 *vertices;
    vec4 *color;
    size_t nVertices;
        
    mat4 mTransform;
  
    
    // For animation
    vec4 cm;
    
}