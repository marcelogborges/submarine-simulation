#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"Matrices.h"
#include"Submarine.h"
#include"Inputs.h"

#define FREE_CAMERA 1
#define LOOKAT_CAMERA 0

class Camera
{
public:
	glm::vec4 Position;
	glm::vec4 Up;
	glm::vec4 View;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	float screenRatio;
	Submarine *submarine;

	bool type;

	bool initializationFreeCamera = true;
	bool initializationLookatCamera = false;
	float cursorXPos;
	float cursorYPos;
	float lastCursorXPos;
	float lastCursorYPos;
	float rotate_z;
	float rotate_y;

	Inputs inputs;

	void Update(float screenRatio, Submarine& submarine, Inputs inputs, bool cameraType);
	void Matrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime);
	
private:
	void LookatUpdate(float screenRatio, Submarine& submarine);
	void FreeUpdate(float screenRatio, Inputs inputs);

	void LookatMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime);
	void FreeMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime);

	// Free Camera Methods
	glm::vec4 RotateView(float deltaTime); 
	glm::vec4 ChangePosition(float deltaTime);

};
#endif