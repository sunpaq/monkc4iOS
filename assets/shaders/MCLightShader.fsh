#version 300 es
precision mediump float;
uniform vec4 Ambient;

in vec4 Color;

out vec4 FragColor;

void main()
{
    vec4 light = Ambient;
    FragColor = min(Color * light, vec4(1.0));
}
