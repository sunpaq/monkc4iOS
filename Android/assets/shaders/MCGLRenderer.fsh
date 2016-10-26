#version 300 es

//varying variables use to pass value between vertex & fragment shader
in lowp vec3 vertexcolor;
in lowp vec2 texturecoord;
in lowp vec3 calculatedNormal;
in lowp vec3 modelPosition;
in lowp vec3 viewPosition;

out lowp vec4 FragColor;

struct Light {
    lowp vec3 ambient;
    lowp vec3 diffuse;
    lowp vec3 specular;
    lowp vec3 color;
    lowp vec3 position;
};

struct Material {
    lowp vec3 ambient;
    lowp vec3 diffuse;
    lowp vec3 specular;
    lowp float dissolve;
    lowp float shininess;
};

uniform Light    light;
uniform Material material;

//texture sampling must in fragment shader
uniform sampler2D texsampler;

void main()
{
    lowp vec3 lightDir = normalize(light.position - modelPosition);
    
    //Ambient Light
    lowp vec3 ambient = (light.ambient * light.color) * material.ambient;
    
    //Diffuse Light
    lowp float diffuseDot = max(0.0, dot(normalize(calculatedNormal), normalize(light.position)));
    lowp vec3  diffuse    = (light.diffuse * light.color) * (diffuseDot * material.diffuse);
    
    //Specular Light
    lowp vec3 viewDir  = normalize(viewPosition - modelPosition);
    lowp vec3 reflectDir = reflect(-lightDir, calculatedNormal);
    
    lowp float specularDot = max(0.0, dot(viewDir, reflectDir));
    lowp float specExp = pow(specularDot, material.shininess);//32 material.shininess
    
    lowp vec3 specular = (light.specular * light.color) * (specExp * material.specular);
    
    //Color Output
    FragColor = vec4((ambient + diffuse + specular) * vertexcolor, material.dissolve);
}
