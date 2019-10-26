#version 150

in  vec4 vPosition;
in  vec4 vColor;
in  vec2 vTexCoord;

out vec4 color;
out vec2 texCoord;

void main() 
{
  gl_Position = vPosition;
  color = vColor;
  texCoord = vTexCoord;
} 
