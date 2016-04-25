#version 300 core
in lowp vec3 TexCoords;
out lowp vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoords);
}
