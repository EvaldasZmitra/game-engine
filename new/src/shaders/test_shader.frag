#version 330 core
out vec4 FragColor;
in vec3 norm;
in vec2 uv;

uniform sampler2D myTextureSampler;

void main() {
   vec3 light_dir = vec3(0, 0, 1);
   float c = dot(-light_dir, norm);
   FragColor = texture( myTextureSampler, uv ) * c;
}