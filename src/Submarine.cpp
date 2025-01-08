#include "Submarine.h"

void Submarine::Movimentation(Inputs inputs, float delta_time, VirtualScene &VirtualScene)
{
    // Atualiza a direção do submarino considerando a rotação
    Direction = Matrix_Rotate_Y(rotate) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    Direction = Direction / norm(Direction);

    // Atualiza rotação baseado nos inputs A/D
    UpdateRotation(inputs.keyPressedD, inputs.keyPressedA, delta_time);
    
    // Atualiza velocidade baseado nos inputs W/S/Shift/Ctrl
    UpdateSpeed(inputs.keyPressedW, inputs.keyPressedS, inputs.keyPressedLeftShift, inputs.keyPressedLeftControl, delta_time);
    
    // Calcula o movimento horizontal baseado na direção e velocidade
    glm::vec4 horizontalVelocity = Direction * speed * delta_time;
    
    // Calcula o movimento vertical
    glm::vec4 verticalVelocity = glm::vec4(0.0f, vertical_speed * delta_time, 0.0f, 0.0f);
    
    // Combina os movimentos
    Position += horizontalVelocity + verticalVelocity;

    // Atualiza a matriz de transformação do submarino
    Matrix = Matrix_Translate(Position.x, Position.y, Position.z) *
             Matrix_Rotate_Y(rotate);
}

void Submarine::UpdateSpeed(bool accelerate, bool brake, bool moveUp, bool moveDown, float delta_time)
{
    // Movimento horizontal
    if (accelerate && speed < max_speed)
    {
        speed += acceleration * delta_time;
    }

    if (brake && speed > 0)
    {
        speed -= acceleration * delta_time;
    }

    if (!accelerate && speed > 0)
    {
        speed -= acceleration * delta_time; // Desaceleração natural
    }

    // Movimento vertical
    if (moveUp)
    {
        vertical_speed = max_vertical_speed;
    }
    else if (moveDown)
    {
        vertical_speed = -max_vertical_speed;
    }
    else
    {
        vertical_speed = 0.0f;
    }
}

void Submarine::UpdateRotation(bool rotateRight, bool rotateLeft, float delta_time)
{
    const float ROTATION_SPEED = 1.0f;
    
    // Rotação lateral (rotate) usando A e D
    if (rotateLeft)
    {
        rotate += ROTATION_SPEED * delta_time;
    }
    
    if (rotateRight)
    {
        rotate -= ROTATION_SPEED * delta_time;
    }
    
    // Normaliza o ângulo rotate para mantê-lo entre -PI e PI
    rotate = fmod(rotate, 2 * M_PI);
    if (rotate > M_PI) 
        rotate -= 2 * M_PI;
    else if (rotate < -M_PI) 
        rotate += 2 * M_PI;
}

Submarine::Submarine(glm::vec4 position, float acceleration, float max_speed)
{
    Submarine::Position = position;
    Submarine::rotate = 0.0f;
    Submarine::acceleration = acceleration;
    Submarine::max_speed = max_speed;
    Submarine::speed = 0.0f;
}

void Submarine::Draw(VirtualScene &VirtualScene, Shader &GpuProgram, float delta_time)
{
    glm::mat4 bodyMatrix = Matrix;
    glUniformMatrix4fv(GpuProgram.model_uniform, 1, GL_FALSE, glm::value_ptr(bodyMatrix));
    glUniform1i(GpuProgram.object_id_uniform, 0);
    VirtualScene.DrawVirtualObject("submarine", GpuProgram);
}
