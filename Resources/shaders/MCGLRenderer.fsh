#version 300 es

//varying variables use to pass value between vertex & fragment shader
in  lowp vec4 combinedlight;
in  lowp vec3 vertexcolor;
in  lowp vec2 texturecoord;

out lowp vec4 FragColor;

//texture sampling must in fragment shader
uniform sampler2D texsampler;

bool isVec4Zero(lowp vec4 v)
{
    if (v.r == 0.0f && v.g == 0.0f && v.b == 0.0f)
        return true;
    else
        return false;
}

void main()
{
//    lowp vec4 color = texture(texsampler, texturecoord);
//    
//    if (isVec4Zero(color)) {
//        color = vec4(vertexcolor, 1.0f);
//    }
    //Color Output
    lowp vec4 color = vec4(vertexcolor, 1.0f);
    FragColor = combinedlight * color;
}
