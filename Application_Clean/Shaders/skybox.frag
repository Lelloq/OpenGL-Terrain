#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;//Samplercube because its a cubemap

void main()
{    
    FragColor = texture(skybox, texCoords);
}