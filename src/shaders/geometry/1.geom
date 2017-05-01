#version 450 core

layout (triangles) in;
layout (TRIANGLE_STRIP, max_vertices = 3) out;

in VS_OUT {
    vec4 color;
    vec4 modelPos;
} gs_in[];

out GS_OUT { 
  vec3 pNormal;
  vec4 color;
  vec4 modelPos;
} gs_out;

void main() {
    
    gs_out.pNormal = cross(gs_in[0].modelPos.xyz - gs_in[1].modelPos.xyz, gs_in[0].modelPos.xyz - gs_in[2].modelPos.xyz);
    
    gs_out.color = gs_in[0].color;
    gs_out.modelPos = gs_in[0].modelPos;
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    
    gs_out.color = gs_in[1].color;
    gs_out.modelPos = gs_in[1].modelPos;
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();
    
    gs_out.color = gs_in[2].color;
    gs_out.modelPos = gs_in[2].modelPos;
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();
    
    EndPrimitive();
}