#version 300 core

//varying variables use to pass value between vertex & fragment shader
in  lowp vec4 combinedlight;
in  lowp vec3 vertexcolor;
in  lowp vec2 texturecoord;

out lowp vec4 FragColor;

//texture sampling must in fragment shader
uniform sampler2D texsampler;

void main()
{
    //Color Output
    lowp vec4 texturecolor = texture(texsampler, texturecoord);
    if (texturecolor.x < 0.1f) {
        //FragColor = vec4(vertexcolor, 1.0f);
        FragColor = combinedlight * texturecolor;

    }else{
        FragColor = combinedlight * texturecolor;
    }
}
