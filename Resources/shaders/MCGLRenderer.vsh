#version 300 core

//vertex attributes
in vec4 position;
in vec3 normal;
in vec3 color;

//uniform variables from code
uniform mat4  modelViewProjectionMatrix;
uniform mat3  normalMatrix;
uniform float ambientLightStrength;
uniform vec3  ambientLightColor;
uniform vec3  diffuseLightColor;
uniform vec3  diffuseLightPosition;

//varying variables use to pass value between vertex & fragment shader
out vec3 combinedcolor;

void main()
{
    //Ambient Light
    vec3 ambient = ambientLightStrength * ambientLightColor;
    
    //Diffuse strength
    vec3 eyeNormal = normalize(normalMatrix * normal);
    float diffuseStrength_NdotP = max(0.0, dot(eyeNormal, normalize(diffuseLightPosition)));
    
    //Diffuse Light
    vec3  diffuse = diffuseLightColor * diffuseStrength_NdotP;
    
    //Combined Color
    combinedcolor = (ambient + diffuse) * color;
    
    //Position
    gl_Position = modelViewProjectionMatrix * position;
}
