#version 300 es

//uniform mat4  view.view;
//uniform mat4  view.projection;
//uniform vec3  view.position;

//uniform mat4  model.model;
//uniform mat3  model.normal;

//uniform vec3 light.ambient;
//uniform vec3 light.diffuse;
//uniform vec3 light.specular;
//uniform vec3 light.color;
//uniform vec3 light.position;

//uniform vec3 material.ambient;
//uniform vec3 material.diffuse;
//uniform vec3 material.specular;
//uniform int  material.shininess;

//uniform sampler2D texsampler;

//vertex attributes
layout (location=0) in vec4 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 color;
layout (location=3) in vec2 texcoord;

struct View {
    mat4 view;
    mat4 projection;
    vec3 position;
};

struct Model {
    mat4 model;
    mat3 normal;
};

//uniform variables from code
uniform View  view;
uniform Model model;

//varying variables use to pass value between vertex & fragment shader
out vec3 vertexcolor;
out vec2 texturecoord;
out vec3 calculatedNormal;
out vec3 modelPosition;
out vec3 viewPosition;

void main()
{
    //Vertex Color
    vertexcolor = color;
    
    //Texture Coordinate
    texturecoord = texcoord;
    
    //Normal fix the non-uniform scale issue
    calculatedNormal = mat3(transpose(inverse(model.model))) * normal;
    
    //Eye normal
    //eyeNormal = normalize(model.normal * normal);
    
    //Specular Light
    modelPosition = vec3(model.model * position);
    viewPosition  = view.position;
    
    //Position
    gl_Position = view.projection * view.view * model.model * position;
}
