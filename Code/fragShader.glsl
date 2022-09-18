STRINGIFY(
#version 330 core\n
out vec4 FragColor;

varying vec3 vNormal;

void main()
{
    FragColor = vec4(normalize(vNormal)*0.5+0.5,1.0);
}
)