STRINGIFY(
#version 330 core\n
out vec4 FragColor;

varying vec3 worldPos;
varying vec3 vNormal;

uniform vec3 lightPos;
uniform vec3 camPos;



void main()
{
    vec3 normal = normalize(vNormal);

    vec3 invLightDir = normalize(lightPos - worldPos);

    vec4 lightColor = vec4(1, 0, 0, 1);
    
    vec4 specColor = vec4(0, 0, 1, 1);
    float specStr = 1.0f;
    vec3 viewDir = normalize(camPos - worldPos);
    

    float spec = pow(max(dot(normalize(viewDir + invLightDir), normal), 0.0), 32);
    float fSpec = specStr * spec;

    float diffuse = max(dot(normal, invLightDir), 0);

    FragColor = (lightColor * diffuse) + (specColor * fSpec);
}
)