#include "Camera.h"
#include <iostream>
#include <glad/glad.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	//std::cout << INIT_POSITION.x << " " << INIT_POSITION.y << " " << INIT_POSITION.z << "\n" << position.x << " " << position.y << " " << position.z << std::endl;
	Update_Camera_Vectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	// Camera Position, Target Position, Up Vector relative to the world
	return glm::lookAt(Position, Position + Front, WorldUp);
}

void Camera::Process_Keyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	//Update_Camera_Vectors();
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
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
