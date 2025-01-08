#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
in vec4 position_world;
in vec4 normal;
in vec4 position_model;
in vec2 texcoords;
in float lambert_gouraud;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define SUBMARINE 0
#define FISH 1
#define SHARK 2
#define SAND 3
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;  // Submarine texture
uniform sampler2D TextureImage1;  // Fish texture
uniform sampler2D TextureImage2;  // Shark texture
uniform sampler2D TextureImage3;  // Sand texture

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 p = position_world;
    vec4 n = normalize(normal);
    vec4 light = vec4(0.0, 1.0, 0.0, 0.0); 
    vec4 l = normalize(light);
    vec4 v = normalize(camera_position - p);

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0);

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.3, 0.3, 0.3);

    // Obtemos a refletância difusa a partir da textura apropriada
    vec3 Kd0;
    float U = texcoords.x;
    float V = texcoords.y;
    
    // Seleciona a textura baseada no object_id
    if(object_id == SUBMARINE)
    {
        Kd0 = texture(TextureImage0, vec2(U,V)).rgb;
    }
    else if(object_id == FISH)
    {
        Kd0 = texture(TextureImage1, vec2(U,V)).rgb;
    }
    else if (object_id == SHARK)
    {
        Kd0 = texture(TextureImage2, vec2(U,V)).rgb;
    }
    else
    {
        Kd0 = texture(TextureImage3, vec2(U,V)).rgb;
            U = texcoords.x * 0.1;
            V = texcoords.y * 0.1;
    }
    
    // Equação de Iluminação de lambert
    vec3 termo_difuso_lambert = max(0,dot(n,l)) * I * Kd0;

    color.a = 1;

    // Lógica de iluminação para diferentes objetos
    if(object_id == SUBMARINE)
    {
        color.rgb = lambert_gouraud * I * Kd0;
    }
    else if(object_id == SAND) {
        color.rgb = Kd0;
    }
    else
    {
        float q_linha = 80.0; 

        vec4 h = normalize(v + l);

        vec3 termo_ambiente = Kd0 * Ia;
        vec3 termo_especular_blinn_phong = pow(dot(n,h), q_linha) * I * Kd0;

        color.rgb = termo_difuso_lambert + termo_ambiente + termo_especular_blinn_phong;
    }

    // Cor final com correção gamma, considerando monitor sRGB.
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}