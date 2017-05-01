#version 450

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;

out VS_OUT {
    vec4 color;
    vec4 modelPos;
} vs_out;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
  vs_out.modelPos = ModelMatrix * in_Position;
  gl_Position = (ProjectionMatrix * ViewMatrix) * vs_out.modelPos;
  vs_out.color = in_Color;
}