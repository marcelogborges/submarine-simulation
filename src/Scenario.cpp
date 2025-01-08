#include "Scenario.h"
#include "collisions.h"

Scenario::Scenario(Submarine& sub) 
    : submarine(sub),
      fish({glm::vec4(30.0f, 0.0f, 0.0f, 1.0f), 1.0f, 1.0f, 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)}),
      shark({glm::vec4(0.0f, 0.0f, 30.0f, 1.0f), 1.0f, 1.0f, 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)}),
      sand({glm::vec4(0.0f, -30.0f, 0.0f, 1.0f), 200.0f, 1.0f, 200.0f, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)})
{
}

Object Scenario::GetSubmarineObject() const {
    return Object{
        submarine.Position,
        submarine.height,
        submarine.width,
        submarine.length,
        submarine.Direction
    };
}

void Scenario::DrawFish(VirtualScene& virtualScene, Shader& gpuProgram)
{
    glm::mat4 fishModel = Matrix_Scale(1.0f, 1.0f, 1.0f)
        * Matrix_Translate(fish.Position.x, fish.Position.y, fish.Position.z)
        * Matrix_Rotate_Y(-1.57f)
        * Matrix_Rotate_X(-1.57f);

    glUniformMatrix4fv(gpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(fishModel));
    glUniform1i(gpuProgram.object_id_uniform, 1);
    virtualScene.DrawVirtualObject("fish", gpuProgram);
}

void Scenario::DrawShark(VirtualScene& virtualScene, Shader& gpuProgram) 
{
    glm::mat4 sharkModel = Matrix_Scale(1.0f, 1.0f, 1.0f)
        * Matrix_Translate(shark.Position.x, shark.Position.y, shark.Position.z)
        * Matrix_Rotate_Y(1.57f);
    
    glUniformMatrix4fv(gpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(sharkModel));
    glUniform1i(gpuProgram.object_id_uniform, 2);
    virtualScene.DrawVirtualObject("shark", gpuProgram);
}

void Scenario::DrawSand(VirtualScene& virtualScene, Shader& gpuProgram)
{
    glm::mat4 sandModel = Matrix_Scale(200.0f, 1.0f, 200.0f)
        * Matrix_Translate(sand.Position.x, sand.Position.y, sand.Position.z);

    glUniformMatrix4fv(gpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(sandModel));
    glUniform1i(gpuProgram.object_id_uniform, 3);
    virtualScene.DrawVirtualObject("plane", gpuProgram);
}

void Scenario::Collisions(float deltaTime, VirtualScene& virtualScene)
{
    Object subObject = GetSubmarineObject();

    // Check collision with fish
    glm::vec4 fishCollision = CubeCubeCollision(subObject, fish, submarine.Direction);
    if (fishCollision != movement)  // If collision occurred
    {
        // Reset fish position
        fish.Position = glm::vec4(
            static_cast<float>(rand() % 100 - 50),
            0.0f,
            static_cast<float>(rand() % 100 - 50),
            1.0f
        );
    }

    // Check collision with shark
    glm::vec4 sharkCollision = CubeCubeCollision(subObject, shark, movement);
    if (sharkCollision != movement)  // If collision occurred
    {   
        // Reset submarine
        submarine.Position = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        submarine.Direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        submarine.speed = 0.0f;
        submarine.vertical_speed = 0.0f;
        submarine.rotate = 0.0f;
        return;  // Skip further collision checks
    }

    // Check collision with sand using CubePlaneCollision
    glm::vec4 sandCollision = CubePlaneCollision(subObject, movement, sand);
    if (sandCollision != movement)  // If collision occurred
    {
        // Reset submarine
        submarine.Position = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        submarine.Direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        submarine.speed = 0.0f;
        submarine.vertical_speed = 0.0f;
        submarine.rotate = 0.0f;
        return;
    }
}