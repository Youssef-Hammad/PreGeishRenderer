#pragma once

#include <glm/glm/gtc/matrix_transform.hpp>

// Defines several possible options for CAMERA movement.
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct GLFWwindow;

// Default CAMERA values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.f;
const float SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 45.0f;
const glm::vec3 DEFAULT_POSITION = glm::vec3( 50.0f, 2.0f, 50.0f );
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
private:
	void Update_Camera_Vectors();

public:
	// Camera Attributes
	float lastX;
	float lastY;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	// Euler angles
	float Yaw; // Y axis (Right/Left)
	float Pitch; // X axis (Up/Down)

	//Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//flags
	bool firstMouseMove;


	Camera(float width, float height, float yaw = YAW, float pitch = PITCH);
	glm::mat4 GetViewMatrix();
	void Process_Keyboard(GLFWwindow* window, float deltaTime);
	void Process_Mouse_Movement(float xoffset, float yoffset, bool constraintPitch = true);
	void Process_Mouse_Scroll(float yoffset);
	inline void Move(Camera_Movement direction, float deltaTime);
	void SetPosition(glm::vec3 newPos);
	void invertPitch();

};
