#version 300 es

precision highp samplerCube;
precision highp float;
precision lowp int;
const float Epsilon = 0.0000001;

//input
in vec2 TexCoords;

//uniforms
uniform sampler2D cubeSampler;

//output
out vec4 color;

void main()
{
    color = texture(cubeSampler, TexCoords);
    //color = vec4(0.5,0.0,0.0,1.0);
}
