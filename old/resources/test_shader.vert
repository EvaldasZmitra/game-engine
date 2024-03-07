#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 vertexUV;
layout (location = 9) in vec3 aNorm;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec3 norm;
out vec2 uv;

void main() {
   vec3 a = getv();
   norm = (m * vec4(aNorm, 0)).xyz;
   gl_Position = p * v * m * vec4(aPos, 1.0);
   uv = vertexUV;
}