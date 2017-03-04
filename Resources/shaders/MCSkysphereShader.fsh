#version 300 es

precision highp sampler2D;
precision highp float;
precision lowp int;
const float Epsilon = 0.0000001;

//input
in vec3 TexCoords;

//uniforms
uniform sampler2D sampler;

//output
out vec4 color;

void main()
{
    color = texture(sampler, TexCoords);
}
