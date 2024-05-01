#include"Model.h"
#include "Landscape.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGTH 1000

std::vector <Vertex> createSun();
std::vector <GLuint> createSunIndices();

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

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 5.9f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Shader landScapeShader("terrain.vert", "terrain.frag");
	landScapeShader.Activate();
	glUniform4f(glGetUniformLocation(landScapeShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(landScapeShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Shader LightShader("light.vert", "light.frag");

	std::vector<Vertex> sunVertices = createSun();
	std::vector<GLuint> sunIndices = createSunIndices();
	std::vector<Texture> sunTextures;
	Texture sunTexture("recourses/Textures/sun.jpg", "diffuse", 0);
	sunTextures.push_back(sunTexture);
	Mesh sun(sunVertices, sunIndices, sunTextures);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(SCREEN_WIDTH, SCREEN_HEIGTH, glm::vec3(0.0f, 0.0f, 2.0f));

	Landscape LS("recourses/Heigtmaps/heightmap.png", 1);
	Model model("models/bunny/scene.gltf");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 300.0f);

		// Draw a model
		LS.drawTerrain(landScapeShader, camera);
		model.Draw(shaderProgram, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


std::vector <Vertex> createSun()
{

	GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
	};


	std::vector<Vertex> vertices;

	// Iterate through the lightVertices array and fill in the Vertex struct
	for (int i = 0; i < sizeof(lightVertices) / sizeof(float); i += 3) {
		Vertex vertex;
		// Multiply each coordinate by 10 and assign it to the Position part of the vertex
		vertex.Position.x = lightVertices[i] * 20.0f;
		vertex.Position.y = lightVertices[i + 1] * 20.0f;
		vertex.Position.z = lightVertices[i + 2] * 20.0f;
		// Set the Color to (1, 1, 1)
		vertex.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		// Set the Normal and TexUV to default values (0, 0)
		vertex.Normal = glm::vec3(0.0f);
		vertex.TexUV = glm::vec2(0.0f);
		// Add the vertex to the vertices vector
		vertices.push_back(vertex);
	}
	return vertices;
}

std::vector <GLuint> createSunIndices() {
	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	std::vector<GLuint> indices(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	return indices;
}
