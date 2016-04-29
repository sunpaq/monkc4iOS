#version 300 es
in lowp vec3 TexCoords;
out lowp vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoords);
}
