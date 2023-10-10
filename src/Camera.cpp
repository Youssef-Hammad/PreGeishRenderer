#include "Camera.h"
#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>

Camera::Camera(float  width, float height, float yaw, float pitch)
	: lastX(width/2.0f), lastY(height/2.0f), Yaw(yaw), Pitch(pitch),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(DEFAULT_ZOOM)
{
	SetPosition(DEFAULT_POSITION);
	firstMouseMove = true;
	Update_Camera_Vectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	// Camera Position, Target Position, Up Vector relative to the world
	return glm::lookAt(Position, Position + Front, WORLD_UP);
}

void Camera::Process_Keyboard(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		deltaTime *= 0.2f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		deltaTime *= 20.0f;


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Move(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Move(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Move(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Move(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Move(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Move(DOWN, deltaTime);
	/*float velocity = MovementSpeed * deltaTime;
	glm::vec3 newValue = glm::vec3(0);
	switch (direction)
	{
	case FORWARD:
		newValue += Front * velocity;
		break;
	case BACKWARD:
		newValue -= Front * velocity;
		break;
	case RIGHT:
		newValue += Right * velocity;
		break;
	case LEFT:
		newValue -= Right * velocity;
		break;
	case UP:
		newValue += Up * velocity;
		break;
	case DOWN:
		newValue -= Up * velocity;
		break;
	}
	Position += newValue;*/
}

void Camera::Process_Mouse_Movement(float xoffset, float yoffset, bool constraintPitch)
{

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constraintPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	Update_Camera_Vectors();
}

void Camera::Process_Mouse_Scroll(float yoffset)
{
	Zoom -= yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::SetPosition(glm::vec3 newPos)
{
	Position = newPos;
}

inline void Camera::Move(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	glm::vec3 newValue = glm::vec3(0);
	switch (direction)
	{
	case FORWARD:
		newValue += Front * velocity;
		break;
	case BACKWARD:
		newValue -= Front * velocity;
		break;
	case RIGHT:
		newValue += Right * velocity;
		break;
	case LEFT:
		newValue -= Right * velocity;
		break;
	case UP:
		newValue += Up * velocity;
		break;
	case DOWN:
		newValue -= Up * velocity;
		break;
	}
	Position += newValue;
}

void Camera::Update_Camera_Vectors()
{
	// Converting the Yaw and Pitch values to a 3D direction vector
	// See https://learnopengl.com/Getting-started/Camera "Look Around" Section
	// It's explained there better than any comment I'll write
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	newFront.y = sin(glm::radians(Pitch));
	newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(newFront);

	// Cross product between 2 vectors yields a third vector perpendicular to both
	// That's how the right and up vector are calculated
	Right = glm::normalize(glm::cross(Front, WORLD_UP));
	Up = glm::normalize(glm::cross(Right, Front));
}
