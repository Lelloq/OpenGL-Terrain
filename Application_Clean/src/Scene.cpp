#include "Scene.h"

/*Texture slots used:
0 - heightmap/noise
1 - skybox
2 - rock texture
3 - snow texture
4 - water normals
5 - water dudv
6 - reflection fbo
7 - refraction fbo
8 - depth fbo for water*/

Scene::Scene(GLFWwindow* window, std::shared_ptr<InputHandler> H) : 
m_window(window),
m_handler(H)
{
	glEnable(GL_DEPTH_TEST);
	// Shader
	m_terrainShader = new Shader("Shaders/terrain/terrainVert.vert", "Shaders/terrain/terrainFrag.frag", 
		"Shaders/terrain/tessControlShader.tcs", "Shaders/terrain/tessEvaluationShader.tes", 
		"Shaders/terrain/Norms.gs");
	m_skyShader = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag", nullptr, nullptr, nullptr);
	m_waterShader = new Shader("Shaders/water/water.vert", "Shaders/water/water.frag", nullptr, nullptr, nullptr);
	m_compShader = new Shader("Shaders/compute/terrain/noise.comp");
	m_cdmShader = new Shader("Shaders/compute/terrain/cdm.comp");

	m_water = std::make_shared<Water>(100.f,m_waterLevel);
	m_water->bindWaterTextures(5, 4);
	m_water->setAttachments(6, 0, 7, 0);
	m_water->setDepthAttachment(8);

	//Water
	m_waterShader->use();
	m_waterShader->setFloat("waterLevel", m_waterLevel);
	m_waterShader->setInt("waterNormals", 4);
	m_waterShader->setInt("waterDudv", 5);
	m_waterShader->setInt("reflection", 6);
	m_waterShader->setInt("refraction", 7);
	m_waterShader->setInt("depthMap", 8);
	m_waterShader->setInt("dudvTiling", 40);
	m_waterShader->setVec4("waterColour", glm::vec4(0.f, 0.0f, 0.01f, 1.0f));
	m_waterShader->setFloat("screenW", SCR_WIDTH);
	m_waterShader->setFloat("screenH", SCR_HEIGHT);
	m_waterShader->setFloat("near", NEAR_PLANE);
	m_waterShader->setFloat("far", FAR_PLANE);

	//Terrain
	m_terrainShader->use();
	m_terrainShader->setFloat("waterLevel", m_waterLevel);
	m_terrainShader->setInt("heightmap", 0);

	//Perlin noise
	std::srand(std::time(0));
	m_compShader->use();
	m_compShader->setInt("octave", 20);
	m_compShader->setInt("seed", std::rand() / 10);
	m_cdmShader->use();
	m_cdmShader->setFloat("scale", 120.0f);

	//Terrain texture
	m_rock = std::make_shared<Texture>();
	m_rock->loadTexture("../Images/terrain/ground.jpg");
	m_rock->Bind(2);
	m_terrainShader->use();
	m_terrainShader->setInt("rock", 2);
	m_snow = std::make_shared<Texture>();
	m_snow->loadTexture("../Images/terrain/snow.jpg");
	m_snow->Bind(3);
	m_terrainShader->setInt("snow", 3);
	m_terrainShader->setFloat("cutoff", 160.f);

	//Lighting
	setTerrainLighting();
	setWaterLighting();

	// Camera & Input
	m_camera = std::make_shared<FirstPersonCamera>(glm::vec3(260, 150, 300));   // xyz argument to set starting position of camera
	m_camera->attachHandler(window, H);
	// Terrain Grid
	m_terrain = std::make_shared<Terrain>();
	// Skybox
	m_skybox = std::make_shared<Skybox>();
	std::vector<std::string> sky
	{
		"../Images/skybox/right.jpg",
		"../Images/skybox/left.jpg",
		"../Images/skybox/top.jpg",
		"../Images/skybox/bottom.jpg",
		"../Images/skybox/front.jpg",
		"../Images/skybox/back.jpg",
	};
	m_skybox->loadCubemap(sky);
	m_skybox->Bind(1);
	m_skyShader->setInt("skybox", 1);

	//Create noise texture
	m_noise = std::make_shared<Texture>();
	m_noise->createTexture(2048, 2048);
	m_noise->bindImage(0, GL_WRITE_ONLY);
	dispatchComputeNoise();

	//Create normals texture from heightmap
	m_normals = std::make_shared<Texture>();
	m_normals->createTexture(2048, 2048);
	m_normals->bindImage(1, GL_WRITE_ONLY);
	dispatchComputeCDM();
}

Scene::~Scene()
{
	delete m_terrainShader;
	delete m_skyShader;
	delete m_compShader;
	delete m_cdmShader;
	delete m_waterShader;
}

void Scene::update(float dt)
{
	m_camera->update(dt); // check for input
	float dist = 2 * (m_camera->getPosition().y - m_waterLevel);
	
	m_water->enableClipPlane();
	//Draw terrain in reflection render pass
	m_water->bindReflection();
	//To set refraction normal must be -1 and level must be inverted i.e -95 to 95
	m_terrainShader->use();
	m_terrainShader->setVec4("clipPlane", glm::vec4(0.f, 1.f, 0.f, -m_waterLevel + 1.f));//Added offset for seamless reflection
	m_camera->getPosition().y -= dist;
	m_camera->invertPitch();
	m_camera->updateCameraVectors();
	renderskybox();
	renderterrain();

	//Draw terrain in refraction render pass
	m_water->bindRefraction();
	//To set refraction normal must be -1 and level must be inverted i.e -95 to 95
	m_terrainShader->use();
	m_terrainShader->setVec4("clipPlane", glm::vec4(0.f, -1.f, 0.f, m_waterLevel));
	m_camera->getPosition().y += dist;
	m_camera->invertPitch();
	m_camera->updateCameraVectors();
	renderterrain();
	m_water->unbind();

	//Draw depth map for water edges
	m_water->bindDepth();
	renderterrain();
	m_camera->updateCameraVectors();
	m_water->unbindDepth();

	m_water->disableClipPlane();
	m_terrainShader->setVec4("clipPlane", glm::vec4(0.f, 0.f, 0.f, 0.f));

	//Draw everything in default render pass
	renderskybox();
	renderterrain();
	renderwater(dt);

	if(m_handler->isKeyPressed(GLFW_KEY_TAB))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Scene::renderterrain()
{
	m_terrainShader->use();

	m_projection = glm::perspective(m_camera->getFOV(), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);  // values from properties header
	m_view = m_camera->getViewMatrix();
	m_model = glm::mat4(1.0f);

	m_terrainShader->setMat4("projection", m_projection);
	m_terrainShader->setMat4("view", m_view);
	m_terrainShader->setMat4("model", m_model);
	m_terrainShader->setVec3("viewPos", m_camera->getPosition());

	m_terrain->render();
}

void Scene::renderwater(float dt)
{
	m_waterShader->use();
	m_elapsedTime += dt;

	m_projection = glm::perspective(m_camera->getFOV(), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);  // values from properties header
	m_view = m_camera->getViewMatrix();
	m_model = glm::mat4(1.0f);

	m_waterShader->setMat4("projection", m_projection);
	m_waterShader->setMat4("view", m_view);
	m_waterShader->setMat4("model", m_model);
	m_waterShader->setVec3("viewPos", m_camera->getPosition());
	m_waterShader->setFloat("elapsed", m_elapsedTime);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_water->Draw();
}

void Scene::renderskybox()
{
	m_skyShader->use();

	m_projection = glm::perspective(m_camera->getFOV(), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);  // values from properties header
	m_view = glm::mat4(glm::mat3(m_camera->getViewMatrix()));

	m_skyShader->setMat4("projection", m_projection);
	m_skyShader->setMat4("view", m_view);

	m_skybox->Draw();
}

void Scene::dispatchComputeNoise()
{
	//Start writing to the noise texture using compute shader
	m_compShader->use();
	m_compShader->dispatch(2048 / 16, 2048 / 32, 1);
	m_compShader->setMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Scene::dispatchComputeCDM()
{
	m_cdmShader->use();
	m_cdmShader->dispatch(2048 / 16, 2048 / 32, 1);
	m_cdmShader->setMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Scene::setTerrainLighting()
{
	m_terrainShader->use();
	m_terrainShader->setVec3("dirLight.direction", glm::vec3(.5f, -0.5f, 1.0f));
	m_terrainShader->setVec3("dirLight.ambient", glm::vec3(0.7f, 0.7f, 0.7f));
	m_terrainShader->setVec3("dirLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
	m_terrainShader->setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	m_terrainShader->setFloat("mat.shininess", .5f);
	m_terrainShader->setVec3("mat.ambient", glm::vec3(0.2f, 0.3f, 0.5f));
	m_terrainShader->setVec3("mat.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	m_terrainShader->setVec3("mat.specular", glm::vec3(0.2f, 0.3f, 0.3f));
}

void Scene::setWaterLighting()
{
	m_waterShader->use();
	m_waterShader->setVec3("dirLight.direction", glm::vec3(.5f, -0.5f, 1.0f));
	m_waterShader->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	m_waterShader->setFloat("mat.specularity", 100.f);
	m_waterShader->setVec3("mat.specular", glm::vec3(1.0f, 1.0f, 1.0f));
}
