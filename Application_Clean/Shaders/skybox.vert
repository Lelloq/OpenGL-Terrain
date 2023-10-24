#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

//No model matrix since the skybox will be following the camera and wont be in a fixed position
uniform mat4 projection;
uniform mat4 view;

void main()
{
	texCoords = aPos;
	gl_Position = projection * view * vec4(aPos,1.0);
}