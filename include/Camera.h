#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

// Defines several possible options for CAMERA movement.
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default CAMERA values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const glm::vec3 INIT_POSITION(0.0f, 2.0f, 0.0f);
const glm::vec3 INIT_UP(0.0f, 1.0f, 0.0f);

class Camera
{
private:
	void Update_Camera_Vectors();
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler angles
	float Yaw; // Y axis (Right/Left)
	float Pitch; // X axis (Up/Down)

	//Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;


	Camera(glm::vec3 position = glm::vec3(60.0f, 2.0f, 60.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	glm::mat4 GetViewMatrix();
	void Process_Keyboard(Camera_Movement direction, float deltaTime);
	void Process_Mouse_Movement(float xoffset, float yoffset, GLboolean constraintPitch = true);
	void Process_Mouse_Scroll(float yoffset);

};

#endif // !CAMERA_H
