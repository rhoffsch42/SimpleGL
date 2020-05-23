#version 410
in vec3 vertex_position_data;
in vec2 vertex_UV_data;
in vec3 vertex_color_data;

out vec2 UV;
out vec3 color;

uniform mat4 MVP;

void main()
{
    color = vertex_color_data;
    gl_Position = MVP * vec4(vertex_position_data, 1.0);
    UV = vertex_UV_data;
}
