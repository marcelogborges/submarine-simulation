#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "Matrices.h"
#include "shaderClass.h"
#include "Camera.h"
#include "Submarine.h"
#include "Inputs.h"
#include "Obj.h"
#include "VirtualScene.h"
#include "Scenario.h"
#include "textures.h"

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale = 1.0f);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
// void TextRendering(GLFWwindow* window, Submarine& submarine, Scenario &Scenario);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

float g_ScreenRatio = 1.0f;

Inputs g_Inputs;

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;
bool g_Reset = false;

int main()
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 800, "US GURI SUBMARINO", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, KeyCallback);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetWindowSize(window, 800, 800); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    // //
    LoadTextureImage("../../data/yellow.png");      // TextureImage0
    LoadTextureImage("../../data/fish.jpg");
    LoadTextureImage("../../data/shark.png");
    LoadTextureImage("../../data/sand.jpg");

    // Inicializamos o código para renderização de texto.

    Shader GpuProgram("../../src/shaders/shader_vertex.glsl", "../../src/shaders/shader_fragment.glsl");

    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Variáveis auxiliares utilizadas para chamada à função
    // TextRendering_ShowModelViewProjection(), armazenando matrizes 4x4.
    glm::mat4 the_projection;
    glm::mat4 the_model;
    glm::mat4 the_view;
    glm::mat4 model;

    Submarine Submarine(glm::vec4(0.0f,1.0f,0.0f,1.0f), 10.0f, 50.0f);

    float nearPlane = -0.1f;
    float farPlane  = -2000.0f;
    float fov = M_PI / 3.0f;
    Camera Camera;

    bool cameraType = LOOKAT_CAMERA;

    float deltaTime, lastFrame, currentFrame;

    VirtualScene VirtualScene;

    ObjModel submarine("../../data/submarine.obj");
    VirtualScene.BuildTriangles(submarine);
    
    ObjModel fish("../../data/fish.obj");
    VirtualScene.BuildTriangles(fish);

    ObjModel shark("../../data/shark.obj");
    VirtualScene.BuildTriangles(shark);

    ObjModel plane("../../data/plane.obj");
    VirtualScene.BuildTriangles(plane);

    Scenario Scenario(Submarine);


    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        
        glClearColor(0.2667f, 0.7255f, 1.0f, 1.0f); // R=68, G=185, B=255, A=1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(GpuProgram.ID);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwGetCursorPos(window, &g_Inputs.cursorXPos, &g_Inputs.cursorYPos);

        if(g_Inputs.keyPressedSpace)
            cameraType = FREE_CAMERA;
        else
            cameraType = LOOKAT_CAMERA;

        Camera.Update(g_ScreenRatio, Submarine, g_Inputs, cameraType);
        Camera.Matrix(fov, nearPlane, farPlane, GpuProgram, deltaTime);

        if(cameraType == LOOKAT_CAMERA)
            Submarine.Movimentation(g_Inputs, deltaTime, VirtualScene);

        if(g_Reset)
        {
            Submarine.Position = glm::vec4(0.0f,1.0f,0.0f,1.0f);
            g_Reset = false;
        }

        Submarine.Draw(VirtualScene, GpuProgram, deltaTime);

        Scenario.DrawFish(VirtualScene, GpuProgram);

        Scenario.DrawShark(VirtualScene, GpuProgram);

        Scenario.DrawSand(VirtualScene, GpuProgram);

        // TextRendering(window, Submarine, Scenario);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // ====================
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mod == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ====================

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_W)
        g_Inputs.keyPressedW = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A)
        g_Inputs.keyPressedA = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S)
        g_Inputs.keyPressedS = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D)
        g_Inputs.keyPressedD = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_LEFT_SHIFT)
        g_Inputs.keyPressedLeftShift = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_LEFT_CONTROL)
        g_Inputs.keyPressedLeftControl = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        g_Inputs.keyPressedSpace = !g_Inputs.keyPressedSpace;


    // // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    // if (key == GLFW_KEY_H && action == GLFW_PRESS)
    // {
    //     g_ShowInfoText = !g_ShowInfoText;
    // }

    // if (key == GLFW_KEY_R && action == GLFW_PRESS)
    // {
    //     g_Reset = !g_Reset;
    // }
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}
