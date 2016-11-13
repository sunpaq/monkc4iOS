#version 300 es

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
//out vec2 texturecoord;
out vec3 calculatedNormal;
out vec3 modelPosition;
out vec3 viewPosition;

void main()
{
    //Vertex Color
    vertexcolor = color;
    
    //Texture Coordinate
    //texturecoord = texcoord;
    texcoord;
    
    //Normal fix the non-uniform scale issue
    calculatedNormal = normalize(mat3(transpose(inverse(model.model))) * normal);
    //calculatedNormal = normalize(model.normal * normal);
    //calculatedNormal = normalize(normal);
    
    //Eye normal
    //eyeNormal = normalize(model.normal * normal);
    
    //Specular Light
    modelPosition = vec3(model.model * position);
    viewPosition  = view.position;
    
    //Position
    gl_Position = view.projection * view.view * model.model * position;
}
