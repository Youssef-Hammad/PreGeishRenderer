#include "Renderer.h"

void Renderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (!wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = false;
		}
	}
}

Renderer::~Renderer()
{
	delete camera;
	delete objectShaderProgram;
	delete terrainShaderProgram;
	delete skyboxShaderProgram;
	glDeleteBuffers(1, &SkyBoxVBO);
	glDeleteBuffers(1, &SkyBoxEBO);
	glDeleteVertexArrays(1, &SkyBoxVAO);
	glDeleteTextures(1, &SkyBoxTexture);
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	for (int i = 0; i < terrains.size(); i++)
		delete terrains[i];
	glfwTerminate();
}

void Renderer::DrawSkyBox()
{
	glDepthFunc(GL_LEQUAL);

	skyboxShaderProgram->SetActive();
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);
	skyboxShaderProgram->setMat4("view", view);
	skyboxShaderProgram->setMat4("projection", projection);

	glBindVertexArray(SkyBoxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

void Renderer::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 1.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = deltaTime * 0.6f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraSpeed = deltaTime * 20.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->Process_Keyboard(FORWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->Process_Keyboard(BACKWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->Process_Keyboard(RIGHT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->Process_Keyboard(LEFT, cameraSpeed);
}

void Renderer::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouseMove)
	{
		firstMouseMove = false;
		lastX = xpos;
		lastY = ypos;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera->Process_Mouse_Movement(xoffset, yoffset);
}

void Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->Process_Mouse_Scroll(yoffset);
}

Renderer::Renderer(int width, int height, std::string window_name)
{
	Width = width;
	Height = height;
	lastX = width / 2.0f;
	lastY = height / 2.0f;
	firstMouseMove = true;
	wireframe = false;
	deltaTime = 0.0f;
	lastFrame = 0.0f;
	currentTextureNumber = GL_TEXTURE0;
	renderSkyBox = false;

	camera = new Camera(glm::vec3(0.0f, 2.0f, 6.0f));

	stbi_set_flip_vertically_on_load(true);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Pre Geish Renderer", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		initSuccess = false;
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD!" << std::endl;
		glfwTerminate();
		initSuccess = false;
		return;
	}
	initSuccess = true;

	glViewport(0, 0, width, height);

	std::string objectVertPath = "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\ObjectVertexShader.vert";
	std::string objectFragPath = "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\ObjectFragmentShader.frag";

	std::string terrainVertPath = "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\TerrainVertexShader.vert";
	std::string terrainFragPath = "C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\TerrainFragmentShader.frag";

	std::string skyboxVertPath = "SkyBoxVertexShader.vert";
	std::string skyboxFragPath = "SkyBoxFragmentShader.frag";

	skyboxShaderProgram = new Shader(skyboxVertPath, skyboxFragPath);

	InitSkyBox();

	terrainShaderProgram = new Shader(terrainVertPath, terrainFragPath);

	objectShaderProgram = new Shader(objectVertPath, objectFragPath);
	objectShaderProgram->SetActive();
	objectShaderProgram->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, 1.0f));
	objectShaderProgram->setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	objectShaderProgram->setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
	objectShaderProgram->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	terrainShaderProgram->SetActive();
	objectShaderProgram->setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, 1.0f));
	objectShaderProgram->setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	objectShaderProgram->setVec3("dirLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
	objectShaderProgram->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	glEnable(GL_DEPTH_TEST);

}

void Renderer::render_scene()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processInput(window);
	glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objectShaderProgram->SetActive();
	
	glm::mat4 view = camera->GetViewMatrix();
	objectShaderProgram->setMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
	objectShaderProgram->setMat4("projection", projection);

	objectShaderProgram->setVec3("viewPos", camera->Position);

	for (int i = 0; i < objects.size(); i++)
	{
		//std::cout << "Drawing" << std::endl;
		objects[i]->draw();
	}

	terrainShaderProgram->SetActive();

	view = camera->GetViewMatrix();
	terrainShaderProgram->setMat4("view", view);

	projection = glm::perspective(glm::radians(camera->Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
	terrainShaderProgram->setMat4("projection", projection);

	terrainShaderProgram->setVec3("viewPos", camera->Position);

	for (int i = 0; i < terrains.size(); i++)
	{
		terrains[i]->draw();
	}

	if(renderSkyBox)
		DrawSkyBox();

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void Renderer::AddObj(std::string path, glm::vec3 pos)
{
	RenderableObject* temp = new RenderableObject(path, objectShaderProgram, pos);
	objects.push_back(temp);
}

void Renderer::InitSkyBox()
{
	float skyboxvertices[] =
	{
		-1.0f,	-1.0f,	1.0f,
		1.0f,	-1.0f,	1.0f,
		1.0f,	-1.0f,	-1.0f,
		-1.0f,	-1.0f,	-1.0f,
		-1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	-1.0f,
		-1.0f,	1.0f,	-1.0f
	};

	unsigned int skyboxIndicies[] =
	{
		//Right
		1,2,6,
		6,5,1,

		//Left
		0,4,7,
		7,3,0,

		//Top
		4,5,6,
		6,7,4,

		//Bottom
		0,3,2,
		2,1,0,

		//Back
		0,1,5,
		5,4,0,

		//Front
		3,7,6,
		6,2,3
	};


	glGenVertexArrays(1, &SkyBoxVAO);
	glGenBuffers(1, &SkyBoxVBO);
	glGenBuffers(1, &SkyBoxEBO);

	glBindVertexArray(SkyBoxVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, SkyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxvertices), &skyboxvertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SkyBoxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndicies), &skyboxIndicies, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	skyboxShaderProgram->SetActive();
	skyboxShaderProgram->setInt("skyboxTex", 0);

	std::string cubeMaps[] =
	{
		"test_resources\\skybox\\right.jpg",
		"test_resources\\skybox\\left.jpg",
		"test_resources\\skybox\\top.jpg",
		"test_resources\\skybox\\bottom.jpg",
		"test_resources\\skybox\\front.jpg",
		"test_resources\\skybox\\back.jpg"
	};

	glGenTextures(1, &SkyBoxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* img = stbi_load(cubeMaps[i].c_str(), &width, &height, &nrChannels, 0);
		if (img)
		{
			// Cube maps are expected to start from top left instead of bottom left
			// That's why we disable flipping
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
			stbi_image_free(img);
		}
		else
		{
			std::cout << "Failed to load: " << cubeMaps[i] << std::endl;
			stbi_image_free(img);
		}
	}
	stbi_set_flip_vertically_on_load(true);
	
}

void Renderer::ToggleSkyBox()
{
	renderSkyBox = !renderSkyBox;
}

void Renderer::AddTerrain()
{
	Terrain* terrain = new Terrain(0,0,"C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\test_resources\\grass.png",terrainShaderProgram,"C:\\Users\\youss\\source\\repos\\PreGeishRenderer\\test_resources\\JnBQ0y9Q.png");
	terrains.push_back(terrain);
}
