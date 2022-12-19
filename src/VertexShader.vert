#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 aColor;
uniform mat4 V;
uniform mat4 P;
out vec4 color;

void main(){
  gl_Position =  P * V * vec4(pos, 1.0);
  color = aColor;
}
