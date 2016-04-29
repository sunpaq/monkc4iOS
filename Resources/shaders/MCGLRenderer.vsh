#version 300 es

//vertex attributes
layout (location=0) in vec4 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 color;
layout (location=3) in vec2 texcoord;

//uniform variables from code
uniform mat4  modelViewMatrix;
uniform mat4  modelMatrix;
uniform mat4  viewMatrix;
uniform mat4  projectionMatrix;
uniform vec3  viewPosition;

uniform mat3  normalMatrix;

uniform float ambientLightStrength;
uniform vec3  ambientLightColor;

uniform vec3  diffuseLightPosition;
uniform vec3  diffuseLightColor;

uniform int   specularLightPower;
uniform float specularLightStrength;
uniform vec3  specularLightPosition;
uniform vec3  specularLightColor;

//varying variables use to pass value between vertex & fragment shader
out vec4 combinedlight;
out vec3 vertexcolor;
out vec2 texturecoord;

void main()
{
    //Ambient Light
    vec3 ambient = ambientLightStrength * ambientLightColor;
    
    //Diffuse strength
    vec3 eyeNormal = normalize(normalMatrix * normal);
    float diffuseStrength_NdotP = max(0.0, dot(eyeNormal, normalize(diffuseLightPosition)));
    
    //Diffuse Light
    vec3 diffuse = diffuseLightColor * diffuseStrength_NdotP;
    
    //Specular Light
    /*
    vec3 FragPos = vec3(modelMatrix * vec4(position, 1.0f));
    vec3 lightDir = normalize(specularLightPosition - FragPos);
    vec3 viewDir = normalize(viewPosition - FragPos);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularLightPower);//32
    vec3 specular = specularLightStrength * spec * specularLightColor;
    */
    
    //Combined Light
    combinedlight = vec4((ambient + diffuse), 1.0f);
    
    //Vertex Color
    vertexcolor = color;
    
    //Texture Coordinate
    texturecoord = texcoord;
    
    //Position
    gl_Position = projectionMatrix * modelViewMatrix * position;
}
