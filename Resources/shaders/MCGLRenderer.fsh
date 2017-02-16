#version 300 es

precision highp float;
precision lowp int;

//varying variables use to pass value between vertex & fragment shader
in vec3 vertexcolor;
in vec2 texturecoord;
in vec3 calculatedNormal;
in vec3 modelPosition;
in vec3 viewPosition;

out vec4 FragColor;

//Android GLSL doesn't support struct
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;
uniform vec3 light_color;
uniform vec3 light_position;

uniform vec3  material_ambient;
uniform vec3  material_diffuse;
uniform vec3  material_specular;
uniform float material_dissolve;
uniform float material_shininess;

//texture sampling must in fragment shader
uniform sampler2D texsampler;
uniform bool usetexture;
uniform int illum;

//Ka*Ia + result
vec3 phoneReflectionForSingleLight(vec3 Kd, vec3 Ks, vec3 Id, vec3 Is, float Es,
                                   vec3 L, vec3 N, vec3 V)
{
    vec3 R = 2.0*max(0.0,dot(L,N))*N - L;
    return Kd*dot(L,N)*Id + Ks*pow(max(0.0,dot(R,V)),Es)*Is;
}

void main()
{
    vec3 lightDir = normalize(light_position - modelPosition);
    
    //Ambient Light
    vec3 ambient = (light_ambient * light_color) * material_ambient;
    
    //Diffuse Light
    float diffuseDot = max(0.0, dot(normalize(light_position), normalize(calculatedNormal)));
    vec3  diffuse    = (light_diffuse * light_color) * (diffuseDot * material_diffuse);
    
    //Specular Light
    vec3 viewDir  = normalize(viewPosition - modelPosition);
    vec3 reflectDir = reflect(-lightDir, calculatedNormal);
    
    float specularDot = max(0.0, dot(reflectDir, viewDir));
    float specExp = pow(specularDot, material_shininess);//32 material.shininess
    
    vec3 specular = (light_specular * light_color) * (specExp * material_specular) ;
    
    //Color Output
    vec3 color = vertexcolor;
    float alpha = 1.0;
    if (usetexture) {
        vec4 texcolor = texture(texsampler, vec2(texturecoord.x, 1.0-texturecoord.y));
        color = texcolor.rgb;
        alpha = texcolor.a;
    } else {
        alpha = material_dissolve;
    }
    
    //Illuminate Mode
    if (illum == 0) {
        //color = Kd
        FragColor = vec4((diffuse) * color, alpha);
        return;
    }
    else if (illum == 1) {
        //color = KaIa + Kd
        FragColor = vec4((ambient + diffuse) * color, alpha);
        return;
    }
    else if (illum == 2) {
        FragColor = vec4((ambient + diffuse + specular) * color, alpha);
        return;
    }
//    else if (illum == 3) {
//        
//    }
//    else if (illum == 4) {
//        
//    }
//    else if (illum == 5) {
//        
//    }
//    else if (illum == 6) {
//        
//    }
//    else if (illum == 7) {
//        
//    }
//    else if (illum == 8) {
//        
//    }
//    else if (illum == 9) {
//        
//    }
//    else if (illum == 10) {
//        
//    }
    
    FragColor = vec4((ambient + diffuse + specular) * color, alpha);

}
