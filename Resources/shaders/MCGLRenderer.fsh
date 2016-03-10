#version 300 core

//varying variables use to pass value between vertex & fragment shader
in  lowp vec4 combinedcolor;
in  lowp vec2 texturecoord;

out lowp vec4 FragColor;

uniform sampler2D texsampler;

void main()
{
    //Color Output
    FragColor = texture(texsampler, texturecoord);
}
