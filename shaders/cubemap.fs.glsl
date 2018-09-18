#version 410

in vec3 texcoords;
uniform samplerCube cubemap;
out vec4 frag_color;

void main() {
  frag_color = texture(cubemap, texcoords);
}