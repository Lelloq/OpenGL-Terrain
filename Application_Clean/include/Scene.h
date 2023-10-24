#pragma once
#include "FPSCamera.h"
#include "objects/objects.h"
#include "Shader.h"
/*
Everything in scene goes here
I'd suggest using this as a base class and extending for specific scenes
*/

class Scene {

public:
	Scene(GLFWwindow* window, std::shared_ptr<InputHandler> H);
	~Scene();
	void update(float dt) ;
private:
	GLFWwindow* m_window;
	std::shared_ptr<InputHandler> m_handler;
	std::shared_ptr<FirstPersonCamera> m_camera;
	//Objects
	std::shared_ptr<Terrain> m_terrain;
	std::shared_ptr<Skybox> m_skybox;
	std::shared_ptr<Texture> m_rock;
	std::shared_ptr<Texture> m_snow;
	std::shared_ptr<Texture> m_noise;
	std::shared_ptr<Texture> m_normals;
	std::shared_ptr<Water> m_water;

	Shader* m_terrainShader;   
	Shader* m_waterShader;   
	Shader* m_skyShader;   
	Shader* m_compShader;
	Shader* m_cdmShader;
	glm::mat4 m_model, m_view, m_projection;

	int m_WorkGroupSize;
	float m_elapsedTime = 0.f;
	float m_waterLevel = 95.f;

	void renderterrain();
	void renderwater(float dt);
	void renderskybox();
	void dispatchComputeNoise();
	void dispatchComputeCDM();
	void setTerrainLighting();
	void setWaterLighting();
};
