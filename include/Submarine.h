#ifndef SUBMARINE_H
#define SUBMARINE_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <map>

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Matrices.h"
#include"Inputs.h"
#include"Obj.h"
#include"VirtualScene.h"
#include"collisions.h"

class Submarine
{
public:
    glm::vec4 Position = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 Direction = glm::vec4(0.0f,0.0f,-1.0f,0.0f);
    glm::vec4 Rotation = glm::vec4(0.0f,0.0f,-1.0f,0.0f);
    glm::mat4 Matrix;

    float height = 10.0f;
    float width = 10.0f;
    float length = 10.0f;

    float rotate = 0.0f;
    float speed = 0.0f;
    float vertical_speed = 0.0f;
    

    float acceleration = 10.0f;
    float max_speed = 50.0f;
    const float max_vertical_speed = 8.0f;

    void UpdateRotation(bool rotateRight, bool rotateLeft, float delta_time);
    void UpdateSpeed(bool accelerate, bool brake, bool moveUp, bool moveDown, float delta_time);
    void Movimentation(Inputs inputs, float delta_time, VirtualScene &VirtualScene);
    void Draw(VirtualScene &VirtualScene, Shader &GpuProgram, float delta_time);
    void Reset();
};
#endif
