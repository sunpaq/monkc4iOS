#version 300 es

precision highp float;
precision mediump int;

//vertex attributes
layout (location=0) in vec3 position;
layout (location=1) in vec2 texcoord;

//uniforms
uniform mat4 boxViewMatrix;
uniform mat4 boxProjectionMatrix;

//output
out vec2 TexCoords;

void main()
{
    gl_Position = boxProjectionMatrix * boxViewMatrix * vec4(position, 1.0);
    TexCoords = texcoord;
//    float R  = 1.0;
//    float PI = 3.1415926;
//    TexCoords.y = acos(position.z / R) / PI;
//    TexCoords.x = - atan(position.y / position.x) / (2.0*PI);
}
