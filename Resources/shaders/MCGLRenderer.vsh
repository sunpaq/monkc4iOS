
//vertex attributes
attribute vec4 position;
attribute vec3 normal;
//attribute vec3 color;

//uniform variables from code
uniform mat4  modelViewProjectionMatrix;
uniform mat3  normalMatrix;
//uniform float ambientLightStrength;
//uniform vec3  ambientLightColor;

//varying variables use to pass value between vertex & fragment shader
//varying lowp vec3 Normal;
varying lowp vec4 colorVarying;

void main()
{
    //Light
    //vec3 ambient = ambientLightStrength * ambientLightColor;
    
    vec3 eyeNormal = normalize(normalMatrix * normal);
    vec3 lightPosition = vec3(1.0, 1.0, 1.0);
    vec4 diffuseColor = vec4(0.4, 0.1, 0.1, 1.0);
    
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
                 
    colorVarying = diffuseColor * nDotVP;
    
    gl_Position = modelViewProjectionMatrix * position;
    //Normal = normal;
}
