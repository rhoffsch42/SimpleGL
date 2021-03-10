#version 410

in vec3 vertex_position_data;
uniform mat4 VP;
out vec3 texcoords;

void main() {
    texcoords = vec3(-vertex_position_data.x, vertex_position_data.y, vertex_position_data.z);
    gl_Position = VP * vec4(vertex_position_data, 1.0);
}