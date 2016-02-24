#version 300 core

//varying variables use to pass value between vertex & fragment shader
in  lowp vec4 combinedcolor;
out lowp vec4 FragColor;

void main()
{
    //Color Output
    FragColor = combinedcolor;
}
