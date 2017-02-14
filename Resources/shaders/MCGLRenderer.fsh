#version 300 es

//varying variables use to pass value between vertex & fragment shader
in lowp vec3 vertexcolor;
in lowp vec2 texturecoord;
in lowp vec3 calculatedNormal;
in lowp vec3 modelPosition;
in lowp vec3 viewPosition;

out lowp vec4 FragColor;

//Android GLSL doesn't support struct
uniform lowp vec3 light_ambient;
uniform lowp vec3 light_diffuse;
uniform lowp vec3 light_specular;
uniform lowp vec3 light_color;
uniform lowp vec3 light_position;

uniform lowp vec3  material_ambient;
uniform lowp vec3  material_diffuse;
uniform lowp vec3  material_specular;
uniform lowp float material_dissolve;
uniform lowp float material_shininess;

//texture sampling must in fragment shader
uniform sampler2D texsampler;
uniform bool usetexture;
uniform int illum;

void main()
{
    lowp vec3 lightDir = normalize(light_position - modelPosition);
    
    //Ambient Light
    lowp vec3 ambient = (light_ambient * light_color) * material_ambient;
    
    //Diffuse Light
    lowp float diffuseDot = max(0.0, dot(normalize(calculatedNormal), normalize(light_position)));
    lowp vec3  diffuse    = (light_diffuse * light_color) * (diffuseDot * material_diffuse);
    
    //Specular Light
    lowp vec3 viewDir  = normalize(viewPosition - modelPosition);
    lowp vec3 reflectDir = reflect(-lightDir, calculatedNormal);
    
    lowp float specularDot = max(0.0, dot(viewDir, reflectDir));
    lowp float specExp = pow(specularDot, material_shininess);//32 material.shininess
    
    lowp vec3 specular = (light_specular * light_color) * (specExp * material_specular) ;
    
    //Color Output
    lowp vec3 color = vertexcolor;
    lowp float alpha = 1.0;
    if (usetexture) {
        lowp vec4 texcolor = texture(texsampler, vec2(texturecoord.x, 1.0-texturecoord.y));
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
