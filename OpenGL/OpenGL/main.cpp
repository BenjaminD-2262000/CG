#include "Model.h"
#include "Landscape.h"
#include <random>
#include "ShadowMap.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGTH 1000
#define RENDER_DISTANCE 100.0f




float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

float quadVertices[] = {
	// Positions      // Texture coordinates
	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // Top-left
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom-left
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom-right

	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, // Top-left
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom-right
	 0.5f,  0.5f, 0.0f,  1.0f, 1.0f  // Top-right
};

std::vector <glm::mat4> randomInstanceMatrix(unsigned int instanceCount, Landscape& LS);
std::vector <glm::mat4> createInstanceMatrix(const std::vector<glm::vec2>& positions, Landscape& LS);


std::vector <glm::mat4> randomInstanceMatrix(unsigned int instanceCount, Landscape& LS);


int main()
{

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "test", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH);





	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	Shader instanceShader("instanced.vert", "default.frag");
	Shader instanceShader2("instanced.vert", "default.frag");

	//Skybox Shader object
	Shader skyboxShader("skybox.vert", "skybox.frag");

	Shader crosshairShader("crosshair.vert", "crosshair.frag");


	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 12.0f, 5.9f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	instanceShader.Activate();
	glUniform4f(glGetUniformLocation(instanceShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(instanceShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	instanceShader2.Activate();
	glUniform4f(glGetUniformLocation(instanceShader2.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(instanceShader2.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);


	Shader landScapeShader("terrain.vert", "terrain.frag");
	landScapeShader.Activate();
	glUniform4f(glGetUniformLocation(landScapeShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(landScapeShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Shader LightShader("light.vert", "light.frag");


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object


	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// All the faces of the cubemap (make sure they are in this exact order)
	std::string facesCubemap[6] =
	{
		"recourses/Skybox/right.jpg",
		"recourses/Skybox/left.jpg",
		"recourses/Skybox/top.jpg",
		"recourses/Skybox/bottom.jpg",
		"recourses/Skybox/front.jpg",
		"recourses/Skybox/back.jpg"
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		if (i == 0) { stbi_set_flip_vertically_on_load(true); }

		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		stbi_set_flip_vertically_on_load(false);
		if (data)
		{
			
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

	unsigned int instanceCount = 100;

	Landscape LS("recourses/Heigtmaps/heightmap.png", 1);
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGTH, glm::vec3(0.0f, 0.0f, 2.0f), LS);

	std::vector <glm::mat4> instanceMatrix = randomInstanceMatrix(instanceCount, LS);

	unsigned int placedInstances = 0;
	std::vector <glm::vec2> positions;
	std::vector <glm::mat4> instanceMatrix2 = createInstanceMatrix(positions, LS);


	Model model("models/frog/scene.gltf", &LS, instanceCount, instanceMatrix);
	Model model2("models/bunny/scene.gltf", &LS);
	Model model3("models/bunny/scene.gltf", &LS, placedInstances, instanceMatrix2);


	// Create and bind a VAO
	unsigned int crosshairVAO;
	glGenVertexArrays(1, &crosshairVAO);
	glBindVertexArray(crosshairVAO);

	// Create and bind a VBO
	unsigned int crosshairVBO;
	glGenBuffers(1, &crosshairVBO);
	glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);

	// Fill the VBO with vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// Specify the vertex attributes
	// Positions attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coordinates attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	
	int crosshairWidth, crosshairHeight, crosshairChannels;
	unsigned char* crosshairData = stbi_load("recourses/Textures/crosshair2.png", &crosshairWidth, &crosshairHeight, &crosshairChannels, 0);
	if (!crosshairData) {
		std::cerr << "Failed to load crosshair texture" << std::endl;
		// Handle error
	}

	unsigned int crosshairTexture;
	glGenTextures(1, &crosshairTexture);
	glBindTexture(GL_TEXTURE_2D, crosshairTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, crosshairWidth, crosshairHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, crosshairData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(crosshairData);



	ShadowMap shadowMap(lightPos);
	shadowMap.sendLightSpaceMatrix(shaderProgram);
	// variable to keep track of time (gravity)
	float lastFrame = 0.0f;

	crosshairShader.Activate();
	crosshairShader.setInt("crosshairTexture", 0);
	

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Preparations for the Shadow Map
		shadowMap.drawShadows(model, camera);


		// Draw scene for shadow map
		Shader shadowShader = shadowMap.getShader();
		model.Draw(shadowShader, camera);
		LS.drawTerrain(shadowShader, camera);


		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Switch back to the default viewport
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH);

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		// Handles camera inputs
		camera.Inputs(window);
		
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.applyGravity(deltaTime);

		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 300.0f);

		//send lightSpaceMatrix to shader
		shadowMap.sendLightSpaceMatrix(shaderProgram);
		//bind shadow map to shader
		shadowMap.bind(shaderProgram);

		// Draw a model
		LS.drawTerrain(landScapeShader, camera);
		model.Draw(instanceShader, camera);
		model2.Draw(shaderProgram, camera, glm::vec2(0, 0));
		model3.Draw(instanceShader, camera);

		
		// places object on click
		int mouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		if (mouseButtonState == 1) {
			glm::vec2 intersectionPoint = camera.calculateIntersectionPoint();
			positions.push_back(intersectionPoint);
			instanceMatrix2 = createInstanceMatrix(positions, LS);
			model3.updateInstanceData(instanceMatrix2.size(), instanceMatrix2);
		}


		// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGTH, 0.1f, RENDER_DISTANCE);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);

		// Activate the crosshair shader program
		crosshairShader.Activate();
		crosshairShader.setInt("crosshairTexture", 0);
		crosshairShader.setFloat("scale", 0.1f);
		// Bind the crosshair texture to texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, crosshairTexture);

		// Draw the crosshair quad
		glBindVertexArray(crosshairVAO); // Bind the VAO containing the quad geometry
		glDrawArrays(GL_TRIANGLES, 0, 6); // Assuming the quad consists of two triangles (6 vertices)

		// Clean up
		glBindVertexArray(0); // Unbind the VAO
		


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	skyboxShader.Delete();
	crosshairShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

float randf(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

std::vector <glm::mat4> randomInstanceMatrix(unsigned int instanceCount, Landscape& LS)
{
	std::vector <glm::mat4> instanceMatrix;
	for (unsigned int i = 0; i < instanceCount; i++)
	{
		glm::vec3 tempTranslation;
		glm::quat tempRotation;
		glm::vec3 tempScale;

		tempTranslation.x = -100.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100.0f - (-100.0f))));
		tempTranslation.z = -100.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100.0f - (-100.0f))));
		tempTranslation.y = LS.getHeight(tempTranslation.x, tempTranslation.z);

		float randomAngle = randf(0.0f, glm::two_pi<float>());
		glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		tempRotation = glm::angleAxis(randomAngle, yAxis);

		float randomScale = randf(0.01f, 0.05f);
		tempScale = glm::vec3(randomScale, randomScale, randomScale);

		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		trans = glm::translate(trans , tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);
//instanceMatrix for placed objects (not random)
std::vector<glm::mat4> createInstanceMatrix(const std::vector<glm::vec2>& positions, Landscape& LS)
{
	std::vector<glm::mat4> instanceMatrix;
	for (const glm::vec2& position : positions)
	{
		glm::vec3 tempTranslation(position.x, 0.0f, position.y);
		tempTranslation.y = LS.getHeight(tempTranslation.x, tempTranslation.z);

		instanceMatrix.push_back(trans * rot * sca);
	}
	return instanceMatrix;
		glm::quat tempRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // No rotation

		glm::vec3 tempScale = glm::vec3(1.0f, 1.0f, 1.0f); // No scaling

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), tempTranslation);
		glm::mat4 rot = glm::mat4_cast(tempRotation);
		glm::mat4 sca = glm::scale(glm::mat4(1.0f), tempScale);

		instanceMatrix.push_back(trans * rot * sca);
	}
	return instanceMatrix;
}