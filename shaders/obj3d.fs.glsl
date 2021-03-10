#version 410
/*texture mod*/
in vec2 UV;
uniform sampler2D myTextureSampler;
uniform float tex_coef;

/*color mod*/
in vec3 color;
uniform int dismod;
uniform vec3 plain_color;

//ouput color
out vec4 frag_color;

void main()
{
    if (dismod == 0)
        frag_color = vec4(color, 1.0) * (1.0 - tex_coef) + tex_coef * texture(myTextureSampler, UV);
    else
        frag_color = vec4(plain_color, 1.0) * (1.0 - tex_coef) + tex_coef * texture(myTextureSampler, UV);
}
