#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Matrices.h"
#include "Scenario.h"

bool PointPlaneCollision(glm::vec4 point_a, glm::vec4 point_b, Object plane){
    glm::vec4 c = plane.Position;
    glm::vec4 n = plane.Direction;
    float t = dotproduct(c-point_a,n)/dotproduct(point_b-point_a,n);
    if (t>=0 && t<= 1){
        glm::vec4 p = point_a + t*(point_b-point_a);
        p = p - (plane.Position - glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f));
        bool inX = ((p.x >= -plane.width) && (p.x <= plane.width)) || plane.width == 0;
        bool inY = ((p.y >= -plane.height) && (p.y <= plane.height)) || plane.height == 0;
        bool inZ = ((p.z >= -plane.length) && (p.z <= plane.length)) || plane.length == 0;
        if (inX && inY && inZ)
            return true;
    }
    return false;
}


bool CubePlaneCollision(Object cube, glm::vec4 direction, Object plane){
    glm::vec4 points[8];
    int x = 0;
    for (float i = -1; i <= 1; i+=2){
        for (float j = -1; j <= 1; j+=2){
            for (float k = -1; k <= 1; k+=2){
                points[x] = glm::vec4 (cube.Position.x+cube.width*i ,
                                       cube.Position.y+cube.height*j ,
                                       cube.Position.z+cube.length*k, 1.0f);
                x++;
            }
        }
    }

    glm::vec4 direction_x = glm::vec4 (direction.x, 0.0f, 0.0f, 0.0f);
    glm::vec4 direction_y = glm::vec4 (0.0f, direction.y, 0.0f, 0.0f);
    glm::vec4 direction_z = glm::vec4 (0.0f, 0.0f, direction.z, 0.0f);

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (i != j && PointPlaneCollision(points[i]+direction_x,points[j]+direction_x,plane))
                return true;
            if (i != j && PointPlaneCollision(points[i]+direction_y,points[j]+direction_y,plane))
                return true;
            if (i != j && PointPlaneCollision(points[i]+direction_z,points[j]+direction_z,plane))
                return true;
        }
    }

    return false;
}

bool PointCubeCollision(Object cube1, Object cube2, glm::vec4 direction){
    cube1.Position += direction;
    glm::vec4 original_Position = cube1.Position;
    cube1.Position -= (cube2.Position - glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f));
    cube2.Position -= (original_Position - glm::vec4 (0.0f, 0.0f, 0.0f, 1.0f));
    glm::vec4 points1[8];
    glm::vec4 points2[8];

    int x = 0;
    for (float i = -1; i <= 1; i+=2){
        for (float j = -1; j <= 1; j+=2){
            for (float k = -1; k <= 1; k+=2){
                points1[x] = glm::vec4 (cube1.Position.x+cube1.width*i ,
                                       cube1.Position.y+cube1.height*j ,
                                       cube1.Position.z+cube1.length*k, 1.0f);
                points2[x] = glm::vec4 (cube2.Position.x+cube2.width*i ,
                                       cube2.Position.y+cube2.height*j ,
                                       cube2.Position.z+cube2.length*k, 1.0f);
                x++;
            }
        }
    }

    bool inx = false, iny = false, inz = false;

    for (int i = 0; i < 8; i++){
        if ((points1[i].x >= -cube2.width && points1[i].x <= cube2.width) ||
            (points2[i].x >= -cube1.width && points2[i].x <= cube1.width))
            inx = true;

        if ((points1[i].y >= -cube2.height && points1[i].y <= cube2.height) ||
            (points2[i].y >= -cube1.height && points2[i].y <= cube1.height))
            iny = true;

        if ((points1[i].z >= -cube2.length && points1[i].z <= cube2.length) ||
            (points2[i].z >= -cube1.length && points2[i].z <= cube1.length))
            inz = true;

    }
    return (inx && iny && inz);
}

bool CubeCubeCollision(Object cube1, Object cube2, glm::vec4 direction){
    glm::vec4 direction_x = glm::vec4 (direction.x, 0.0f, 0.0f, 0.0f);
    glm::vec4 direction_y = glm::vec4 (0.0f, direction.y, 0.0f, 0.0f);
    glm::vec4 direction_z = glm::vec4 (0.0f, 0.0f, direction.z, 0.0f);

    if (PointCubeCollision(cube1, cube2, direction_x))
        return true;

    if (PointCubeCollision(cube1, cube2, direction_y))
        return true;

    if (PointCubeCollision(cube1, cube2, direction_z))
        return true;

    return false;
}
