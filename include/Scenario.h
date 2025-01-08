#ifndef _SCENARIO_H
#define _SCENARIO_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <map>

#include <iostream>
#include <unordered_map> // Troque std::map por std::unordered_map
#include <vector>
#include <glm/glm.hpp>   // Certifique-se de incluir a biblioteca GLM
#include <cstdlib>       // Para rand() e srand()
#include <ctime>         // Para time()
#include <functional>
#include <utility>
#include <random>

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Matrices.h"
#include"Inputs.h"
#include"Obj.h"
#include"VirtualScene.h"
#include"Submarine.h"
#include"collisions.h"

class Scenario {
public:
    Scenario(Submarine& submarine);
    
    void DrawFish(VirtualScene& virtualScene, Shader& gpuProgram);
    void DrawShark(VirtualScene& virtualScene, Shader& gpuProgram);
    void DrawSand(VirtualScene& virtualScene, Shader& gpuProgram);
    void Collisions(float deltaTime, VirtualScene& virtualScene);
    
private:
    Submarine& submarine;
    Object fish;
    Object shark;
    Object sand;
    
    Object GetSubmarineObject() const;
};

#endif