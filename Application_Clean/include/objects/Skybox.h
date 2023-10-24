/*\file Skybox.h
\brief a skybox class*/
#pragma once
#include <vector>
#include "rendering/Shader.h"
#include "rendering/VAO.h"
#include "rendering/VBO.h"
#include "rendering/EBO.h"

/*\class Skybox*/
class Skybox
{
private:
	unsigned int m_ID; //!<skybox texture ID
	VAO* m_VAO;//!< vao object
	VBO* m_VBO;//!< vbo object

    float m_skyboxVertices[108] = {
        // positions          
        -10.0f,  10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

        -10.0f,  10.0f, -10.0f,
         10.0f,  10.0f, -10.0f,
         10.0f,  10.0f,  10.0f,
         10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f, -10.0f,
         10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
         10.0f, -10.0f,  10.0f
    };
public:
	Skybox(); //!< Default constructor
	~Skybox(); //!< Default destructor
	void Bind(unsigned int unit); //!< Bind the cubemap to a slot
	void Unbind(); //!< Unbind the cubemap
	void Draw();//!<Draw the skybox
	void loadCubemap(std::vector<std::string> faces);//!<Load the cubemap from a file
};
