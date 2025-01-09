#include "Scenario.h"
#include "collisions.h"

Scenario::Scenario(Submarine& sub) 
    : submarine(sub),
      fish({glm::vec4(30.0f, 0.0f, 0.0f, 1.0f), 5.0f, 5.0f, 5.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)}),
      shark({glm::vec4(50.0f, 0.0f, 50.0f, 1.0f), 5.0f, 5.0f, 10.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)}),
      sand({glm::vec4(0.0f, -35.0f, 0.0f, 1.0f), 1.0f, 200.0f, 200.0f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)})
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
        * Matrix_Rotate_Y(shark_rotation);
    
    glUniformMatrix4fv(gpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(sharkModel));
    glUniform1i(gpuProgram.object_id_uniform, 2);
    virtualScene.DrawVirtualObject("shark", gpuProgram);
}

void Scenario::DrawSand(VirtualScene& virtualScene, Shader& gpuProgram)
{
    glm::mat4 sandModel = Matrix_Scale(200.0f, 1.0f, 200.0f)
        * Matrix_Translate(0.0f, -30.0f, 0.0f);

    glUniformMatrix4fv(gpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(sandModel));
    glUniform1i(gpuProgram.object_id_uniform, 3);
    virtualScene.DrawVirtualObject("plane", gpuProgram);
}

void Scenario::Collisions(float deltaTime, VirtualScene& virtualScene)
{
    Object subObject = GetSubmarineObject();

    // Check collision with fish
    bool fishCollision = CubeCubeCollision(subObject, fish, subObject.Direction);
    if (fishCollision)  // If collision occurred
    {
        // Reset fish position within specified ranges for X, Y, and Z
        fish.Position = glm::vec4(
            static_cast<float>(rand() % 201 - 100),  // X-axis: range [-100, 100]
            static_cast<float>(rand() % 50 - 25),    // Z-axis: range [-30, 30]
            static_cast<float>(rand() % 201 - 100),  // Y-axis: range [-100, 100]
            1.0f
        );
        num_fishes += 1;
    }

    bool sharkCollision = CubeCubeCollision(subObject, shark, subObject.Direction);
    if (sharkCollision)
    {   
        submarine.Reset();
        num_fishes = 0;
        return;
    }

    bool sandCollision = CubeCubeCollision(subObject, sand, subObject.Direction);
    if (sandCollision)
    {
        submarine.Reset();
        num_fishes = 0;
        return;
    }
}

void Scenario::BezierMovement(float delta_t) {
    if (t <= 1) {
        t += delta_t * 0.1f;
        
        // Raio do círculo
        float radius = 100.0f;
        // Constante mágica para círculo perfeito = 4/3 * tan(π/8) ≈ 0.5522847498
        float handle = radius * 0.5522847498f;
        
        // Definindo os pontos de controle para os 4 quadrantes
        glm::vec4 points[13];  // 4 curvas com 3 pontos de controle cada + ponto inicial
        
        // Primeiro quadrante (0° a 90°)
        points[0] = glm::vec4(radius, 0.0f, 0.0f, 1.0f);
        points[1] = glm::vec4(radius, 0.0f, handle, 1.0f);
        points[2] = glm::vec4(handle, 0.0f, radius, 1.0f);
        points[3] = glm::vec4(0.0f, 0.0f, radius, 1.0f);
        
        // Segundo quadrante (90° a 180°)
        points[4] = glm::vec4(-handle, 0.0f, radius, 1.0f);
        points[5] = glm::vec4(-radius, 0.0f, handle, 1.0f);
        points[6] = glm::vec4(-radius, 0.0f, 0.0f, 1.0f);
        
        // Terceiro quadrante (180° a 270°)
        points[7] = glm::vec4(-radius, 0.0f, -handle, 1.0f);
        points[8] = glm::vec4(-handle, 0.0f, -radius, 1.0f);
        points[9] = glm::vec4(0.0f, 0.0f, -radius, 1.0f);
        
        // Quarto quadrante (270° a 360°)
        points[10] = glm::vec4(handle, 0.0f, -radius, 1.0f);
        points[11] = glm::vec4(radius, 0.0f, -handle, 1.0f);
        points[12] = glm::vec4(radius, 0.0f, 0.0f, 1.0f);  // Volta ao início
        
        // Determinar qual segmento da curva usar baseado no valor de t
        int segment = int(t * 4);  // 4 segmentos
        float local_t = (t * 4) - segment;  // t local para o segmento atual
        if (segment >= 4) {
            segment = 0;
            local_t = 0;
        }
        
        // Índice inicial dos pontos para o segmento atual
        int idx = segment * 3;
        
        // Cálculo da curva de Bézier cúbica para o segmento atual
        glm::vec4 p0 = points[idx];
        glm::vec4 p1 = points[idx + 1];
        glm::vec4 p2 = points[idx + 2];
        glm::vec4 p3 = points[idx + 3];
        
        // Fórmula da curva de Bézier cúbica
        float t2 = local_t * local_t;
        float t3 = t2 * local_t;
        float mt = 1.0f - local_t;
        float mt2 = mt * mt;
        float mt3 = mt2 * mt;
        
        glm::vec4 ct = mt3 * p0 + 
                      3.0f * mt2 * local_t * p1 + 
                      3.0f * mt * t2 * p2 + 
                      t3 * p3;
        
        // Atualiza a posição
        shark.Position = ct;
        shark_rotation -= 0.01f;
    }
    else if (t > 1) {
        t = 0;
    }
}