#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 posVS; 
out vec3 eyePos;
out vec2 uvVS;

uniform vec3 viewPos;

void main()
{
		posVS = vec4(aPos, 1.0).xyz;
		eyePos = viewPos;
		uvVS = aTexCoords;
}

