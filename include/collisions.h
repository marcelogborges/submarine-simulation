#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Forward declaration of Object struct if not already included
struct Object {
    glm::vec4 Position;
    float height;
    float width;
    float length;
    glm::vec4 Direction;
};

/**
 * Verifica colisão entre um ponto e um plano
 * @param point_a Ponto inicial
 * @param point_b Ponto final
 * @param plane Objeto que representa o plano
 * @return true se houver colisão, false caso contrário
 */
bool PointPlaneCollision(glm::vec4 point_a, glm::vec4 point_b, Object plane);

/**
 * Verifica e ajusta a colisão entre um cubo e um plano
 * @param cube Objeto que representa o cubo
 * @param direction Direção do movimento
 * @param plane Objeto que representa o plano
 * @return Direção ajustada após a colisão
 */
bool CubePlaneCollision(Object cube, glm::vec4 direction, Object plane);

/**
 * Verifica colisão entre dois cubos considerando uma direção de movimento
 * @param cube1 Primeiro cubo
 * @param cube2 Segundo cubo
 * @param direction Direção do movimento
 * @return true se houver colisão, false caso contrário
 */
bool PointCubeCollision(Object cube1, Object cube2, glm::vec4 direction);

/**
 * Verifica e ajusta a colisão entre dois cubos
 * @param cube1 Primeiro cubo
 * @param cube2 Segundo cubo
 * @param direction Direção do movimento
 * @return Direção ajustada após a colisão
 */
bool CubeCubeCollision(Object cube1, Object cube2, glm::vec4 direction);

/**
 * Calcula o produto escalar entre dois vetores
 * @param v1 Primeiro vetor
 * @param v2 Segundo vetor
 * @return Resultado do produto escalar
 */
inline float dotproduct(glm::vec4 v1, glm::vec4 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

#endif // COLLISIONS_H