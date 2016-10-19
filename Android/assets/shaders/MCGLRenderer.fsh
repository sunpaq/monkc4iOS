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
    int  shininess;
};

uniform Light    light;
uniform Material material;

//texture sampling must in fragment shader
uniform sampler2D texsampler;

bool isVec4Zero(lowp vec4 v)
{
    if (v.r == 0.0f && v.g == 0.0f && v.b == 0.0f)
        return true;
    else
        return false;
}

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
    
    lowp float specularDot = min(max(0.0001, dot(viewDir, reflectDir)), 1.0);
    lowp float specExp = pow(specularDot, float(material.shininess));//32
    
    lowp vec3 specular = (light.specular * light.color) * (specExp * material.specular);
    
//    lowp vec4 color = texture(texsampler, texturecoord);
//    
//    if (isVec4Zero(color)) {
//        color = vec4(vertexcolor, 1.0f);
//    }
    
    //Color Output
    FragColor = vec4((diffuse), 1.0f);
}
