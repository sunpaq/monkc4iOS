#version 300 es

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
