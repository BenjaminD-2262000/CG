#include "Camera.h"
#include "Landscape.h"

Camera::Camera(int width, int height, glm::vec3 position, Landscape& landscape)
	: width(width), height(height), Position(position), m_landscape(landscape)
{
	// No need to assign to m_landscape here, because it's already been initialized
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{

	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && Position.y <= m_landscape.getHeight(Position.x, Position.z) + cameraHeightOffset)
	{
		verticalVelocity = jumpSpeed; // jumpSpeed is the initial speed of the jump
		isJumping = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 horizontalMovement = speed * glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
		Position += horizontalMovement;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 horizontalMovement = speed * -glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
		Position += horizontalMovement;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(glm::vec3(Orientation.x, 0.0f, Orientation.z), Up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(glm::vec3(Orientation.x, 0.0f, Orientation.z), Up));
	}



	// Handles mouse inputs
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Prevents camera from jumping on the first frame
	if (firstClick)
	{
		glfwSetCursorPos(window, (width / 2), (height / 2));
		firstClick = false;
	}

	// Stores the coordinates of the cursor
	double mouseX;
	double mouseY;
	// Fetches the coordinates of the cursor
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	// and then "transforms" them into degrees 
	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	// Calculates upcoming vertical change in the Orientation
	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	// Decides whether or not the next vertical Orientation is legal or not
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		Orientation = newOrientation;
	}

	// Rotates the Orientation left and right
	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	glfwSetCursorPos(window, (width / 2), (height / 2));
}

float Camera::GetHeightCameraPosition()
{
	return m_landscape.getHeight(Position.x, Position.z);
}

void Camera::applyGravity(float deltaTime)
{
	float currentTerrainHeight = m_landscape.getHeight(Position.x, Position.z);

	if (!isJumping || currentTerrainHeight > previousTerrainHeight)
	{
		Position.y = currentTerrainHeight + cameraHeightOffset;
	}
	if (Position.y == currentTerrainHeight)
	{
		isJumping = false;
	}
	else
	{
		Position.y += verticalVelocity * deltaTime;
	}
	verticalVelocity -= gravity * deltaTime;
	Position.y += verticalVelocity * deltaTime;

	if (Position.y < m_landscape.getHeight(Position.x, Position.z) + cameraHeightOffset)
	{
		Position.y = m_landscape.getHeight(Position.x, Position.z) + cameraHeightOffset;
		verticalVelocity = 0.0f;
	}
	previousTerrainHeight = currentTerrainHeight;
}

glm::vec2 Camera::calculateIntersectionPoint()
{
	glm::vec3 rayOrigin = Position;
	glm::vec3 rayDirection = Orientation;
	float stepSize = 0.1f; // Adjust this to change the accuracy and performance of the intersection test
	float maxDistance = 1000.0f;

	glm::vec3 currentPosition = rayOrigin;
	float travelledDistance = 0.0f;
	while (travelledDistance <= maxDistance) {
		float terrainHeight = m_landscape.getHeight(currentPosition.x, currentPosition.z);
		if (currentPosition.y < terrainHeight) {
			// The ray intersects the terrain
			glm::vec3 intersectionPoint = currentPosition - stepSize * rayDirection; // The previous position is the intersection point
			glm::vec2 intersectionPoint2D = glm::vec2(intersectionPoint.x, intersectionPoint.z); // Convert to 2D
			return intersectionPoint2D;
		}
		currentPosition += stepSize * rayDirection;
		travelledDistance += stepSize;
	}
	return glm::vec2(0.0f, 0.0f);
}
