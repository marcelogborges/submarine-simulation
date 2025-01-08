#include"Camera.h"
#include"Matrices.h"

void Camera::Update(float screenRatio, Submarine& submarine, Inputs inputs, bool cameraType)
{
	Camera::type = cameraType;
	if(cameraType == LOOKAT_CAMERA)
		LookatUpdate(screenRatio, submarine);
	else if(cameraType == FREE_CAMERA)
		FreeUpdate(screenRatio, inputs);
}

void Camera::LookatUpdate(float screenRatio, Submarine& submarine)
{
	Camera::screenRatio = screenRatio;
	Camera::submarine = &submarine;
	initializationFreeCamera = true;
}

void Camera::FreeUpdate(float screenRatio, Inputs inputs)
{
	Camera::inputs = inputs;
	Camera::screenRatio = screenRatio;
	initializationLookatCamera = true;
	if(initializationFreeCamera)
	{
		float r = norm(View);
		Camera::rotate_y = acos(-View.y/r) - M_PI/2;
		Camera::rotate_z = -atan2(View.z, View.x) + M_PI/2;
		Camera::lastCursorXPos = inputs.cursorXPos;
		Camera::lastCursorYPos = inputs.cursorYPos;
		initializationFreeCamera = false;
	}
}

void Camera::Matrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
	if(type == LOOKAT_CAMERA)
		LookatMatrix(fov, nearPlane, farPlane, shader, deltaTime);
	else if(type == FREE_CAMERA)
		FreeMatrix(fov, nearPlane, farPlane, shader, deltaTime);

	ViewMatrix = Matrix_Camera_View(Position, View, Up);		
	ProjectionMatrix = Matrix_Perspective(fov, screenRatio, nearPlane, farPlane);

	GLint view_uniform            = glGetUniformLocation(shader.ID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    GLint projection_uniform      = glGetUniformLocation(shader.ID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(view_uniform, 1 , GL_FALSE , glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(ProjectionMatrix));
}


void Camera::LookatMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
    // Converte Position do submarino para vetor direção
    glm::vec4 subDirection = submarine->Direction / norm(submarine->Direction);
    
    // Calcula a posição da câmera
    Position = submarine->Position - (subDirection * 30.0f); // 10 unidades atrás
    Position.y += 15.0f; // Ligeiramente acima do submarino

    // Calcula o vetor View como direção do olhar
    View = submarine->Position - Position;
    View = View / norm(View); // Normaliza o vetor View

    Up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

void Camera::FreeMatrix(float fov, float nearPlane, float farPlane, Shader& shader, float deltaTime)
{
	View = RotateView(deltaTime);
	Position = ChangePosition(deltaTime);
	Up = glm::vec4(0.0f,1.0f,0.0f,0.0f);
}

glm::vec4 Camera::RotateView(float deltaTime)
{
	float dx = inputs.cursorXPos - lastCursorXPos;
    float dy = inputs.cursorYPos - lastCursorYPos;

    rotate_z -= deltaTime*dx/2;
    rotate_y -= deltaTime*dy/2;

    float rotate_yMax = M_PI/2;
    float rotate_yMin = -rotate_yMax;

    if (rotate_y > rotate_yMax)
        rotate_y = rotate_yMax;

    if (rotate_y < rotate_yMin)
        rotate_y = rotate_yMin;

	rotate_z = fmod(rotate_z, 2 * M_PI);
    if (rotate_z > M_PI) rotate_z -= 2 * M_PI;
    else if (rotate_z < -M_PI) rotate_z += 2 * M_PI;

	lastCursorXPos = inputs.cursorXPos;
	lastCursorYPos = inputs.cursorYPos;

	float y = sin(rotate_y);
	float z = cos(rotate_y)*cos(rotate_z);
	float x = cos(rotate_y)*sin(rotate_z);

	glm::vec4 newView = glm::vec4(x,y,z,0.0f);
   
	return newView;
}

glm::vec4 Camera::ChangePosition(float deltaTime)
{
	float speed = 10 * deltaTime;

	glm::vec4 w = -View;
    glm::vec4 u = crossproduct(Up,w);
    w /= norm(w);
    u /= norm(u);
	glm::vec4 newPosition = Position;

	if(inputs.keyPressedW)
		newPosition -= w*speed;
	if(inputs.keyPressedA)
		newPosition -= u*speed;
	if(inputs.keyPressedS)
		newPosition += w*speed;
	if(inputs.keyPressedD)
		newPosition += u*speed;
	
	if(newPosition.y < 0)
		newPosition.y = 0;

	return newPosition;
}
