#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void viraCamera(float x, float y);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1200;

glm::mat4 view;
glm::mat4 projection;

// Camera settings original

glm::vec3 cameraPos = glm::vec3(-1.0f, 8.0f, 11.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float sensitivity = 0.75f;
float yaw = -90.0f;
float pitch = 0.0f;
unsigned char pixelColor[3];

int numeroVertices = 222;

std::vector<float> verticesOriginais;

struct CarState {
    glm::vec3 posicao;
    float angulo;
    float speed;
    float aceleracao;
    float maxSpeed;
    float curvaSpeed;

    CarState() {
        posicao = glm::vec3(0.0f);
        angulo = 0.0f;
        speed  = 0.0f;
        aceleracao = 0.01f;
        maxSpeed   = 0.01f;
        curvaSpeed = 1.5f;
    }
};


CarState Car;

// Dados de vértices para o chão
float verticesChao[] = {
        -10.0f, -0.6f, -10.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
         10.0f, -0.6f, -10.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
         10.0f, -0.6f,  10.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         10.0f, -0.6f,  10.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        -10.0f, -0.6f,  10.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        -10.0f, -0.6f, -10.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
};

float verticesCarro[] = {

    -0.5f,  0.0f, -0.2f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
     0.5f,  0.0f, -0.2f,   0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
     0.5f,  0.3f, -0.2f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
     0.5f,  0.3f, -0.2f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    -0.5f,  0.3f, -0.2f,   0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
    -0.5f,  0.0f, -0.2f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
    -0.5f,  0.0f,  0.2f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
     0.5f,  0.0f,  0.2f,   0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
     0.5f,  0.3f,  0.2f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
     0.5f,  0.3f,  0.2f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    -0.5f,  0.3f,  0.2f,   0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
    -0.5f,  0.0f,  0.2f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
    -0.5f,  0.0f,  0.2f,   1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
    -0.5f,  0.0f, -0.2f,   1.0f, 0.0f, 0.0f,       1.0f, 0.0f,
    -0.5f,  0.3f, -0.2f,   1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
    -0.5f,  0.3f, -0.2f,   1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
    -0.5f,  0.3f,  0.2f,   1.0f, 0.0f, 0.0f,       0.0f, 1.0f,
    -0.5f,  0.0f,  0.2f,   1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
     0.5f,  0.0f,  0.2f,   1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
     0.5f,  0.0f, -0.2f,   1.0f, 0.0f, 0.0f,       1.0f, 0.0f,
     0.5f,  0.3f, -0.2f,   1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
     0.5f,  0.3f, -0.2f,   1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
     0.5f,  0.3f,  0.2f,   1.0f, 0.0f, 0.0f,       0.0f, 1.0f,
     0.5f,  0.0f,  0.2f,   1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
    -0.5f,  0.3f, -0.2f,   0.0f, -1.0f, 0.0f,       0.0f, 0.0f,
     0.5f,  0.3f, -0.2f,   0.0f, -1.0f, 0.0f,       1.0f, 0.0f,
     0.5f,  0.3f,  0.2f,   0.0f, -1.0f, 0.0f,       1.0f, 1.0f,
     0.5f,  0.3f,  0.2f,   0.0f, -1.0f, 0.0f,       1.0f, 1.0f,
    -0.5f,  0.3f,  0.2f,   0.0f, -1.0f, 0.0f,       0.0f, 1.0f,
    -0.5f,  0.3f, -0.2f,   0.0f, -1.0f, 0.0f,       0.0f, 0.0f,
    -0.5f,  0.0f, -0.2f,   0.0f, -1.0f, 0.0f,       0.0f, 0.0f,
     0.5f,  0.0f, -0.2f,   0.0f, -1.0f, 0.0f,       1.0f, 0.0f,
     0.5f,  0.0f,  0.2f,   0.0f, -1.0f, 0.0f,       1.0f, 1.0f,
     0.5f,  0.0f,  0.2f,   0.0f, -1.0f, 0.0f,       1.0f, 1.0f,
    -0.5f,  0.0f,  0.2f,   0.0f, -1.0f, 0.0f,       0.0f, 1.0f,
    -0.5f,  0.0f, -0.2f,   0.0f, -1.0f, 0.0f,       0.0f, 0.0f,
    -0.25f,  0.3f, -0.1f,  0.0f, 0.0f, 1.0f,        0.0f, 0.0f,
     0.25f,  0.3f, -0.1f,  0.0f, 0.0f, 1.0f,        1.0f, 0.0f,
     0.25f,  0.45f, -0.1f, 0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
     0.25f,  0.45f, -0.1f, 0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
    -0.25f,  0.45f, -0.1f, 0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
    -0.25f,  0.3f, -0.1f,  0.0f, 0.0f, 1.0f,        0.0f, 0.0f,
    -0.25f,  0.3f,  0.1f,  0.0f, 0.0f, 1.0f,        0.0f, 0.0f,
     0.25f,  0.3f,  0.1f,  0.0f, 0.0f, 1.0f,        1.0f, 0.0f,
     0.25f,  0.45f,  0.1f, 0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
     0.25f,  0.45f,  0.1f, 0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
    -0.25f,  0.45f,  0.1f, 0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
    -0.25f,  0.3f,  0.1f,  0.0f, 0.0f, 1.0f,        0.0f, 0.0f,
    -0.25f,  0.3f,  0.1f,  1.0f, 0.0f, 0.0f,        0.0f, 0.0f,
    -0.25f,  0.3f, -0.1f,  1.0f, 0.0f, 0.0f,        1.0f, 0.0f,
    -0.25f,  0.45f, -0.1f, 1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
    -0.25f,  0.45f, -0.1f, 1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
    -0.25f,  0.45f,  0.1f, 1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
    -0.25f,  0.3f,  0.1f,  1.0f, 0.0f, 0.0f,        0.0f, 0.0f,
     0.25f,  0.3f,  0.1f,  1.0f, 0.0f, 0.0f,        0.0f, 0.0f,
     0.25f,  0.3f, -0.1f,  1.0f, 0.0f, 0.0f,        1.0f, 0.0f,
     0.25f,  0.45f, -0.1f, 1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
     0.25f,  0.45f, -0.1f, 1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
     0.25f,  0.45f,  0.1f, 1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
     0.25f,  0.3f,  0.1f,  1.0f, 0.0f, 0.0f,        0.0f, 0.0f,
    -0.25f,  0.45f, -0.1f, 0.0f, -1.0f, 0.0f,         0.0f, 0.0f,
     0.25f,  0.45f, -0.1f, 0.0f, -1.0f, 0.0f,         1.0f, 0.0f,
     0.25f,  0.45f,  0.1f, 0.0f, -1.0f, 0.0f,         1.0f, 1.0f,
     0.25f,  0.45f,  0.1f, 0.0f, -1.0f, 0.0f,         1.0f, 1.0f,
    -0.25f,  0.45f,  0.1f, 0.0f, -1.0f, 0.0f,         0.0f, 1.0f,
    -0.25f,  0.45f, -0.1f, 0.0f, -1.0f, 0.0f,         0.0f, 0.0f,
    -0.25f,  0.3f, -0.1f,  0.0f, -1.0f, 0.0f,        0.0f, 0.0f,
     0.25f,  0.3f, -0.1f,  0.0f, -1.0f, 0.0f,        1.0f, 0.0f,
     0.25f,  0.3f,  0.1f,  0.0f, -1.0f, 0.0f,        1.0f, 1.0f,
     0.25f,  0.3f,  0.1f,  0.0f, -1.0f, 0.0f,        1.0f, 1.0f,
    -0.25f,  0.3f,  0.1f,  0.0f, -1.0f, 0.0f,        0.0f, 1.0f,
    -0.25f,  0.3f, -0.1f,  0.0f, -1.0f, 0.0f,        0.0f, 0.0f,
    -0.4f, -0.2f, -0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
    -0.3f, -0.2f, -0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
    -0.3f,  0.0f, -0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    -0.3f,  0.0f, -0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    -0.4f,  0.0f, -0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
    -0.4f, -0.2f, -0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
     0.3f, -0.2f, -0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
     0.4f, -0.2f, -0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
     0.4f,  0.0f, -0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
     0.4f,  0.0f, -0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
     0.3f,  0.0f, -0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
     0.3f, -0.2f, -0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
    -0.4f, -0.2f,  0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
    -0.3f, -0.2f,  0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
    -0.3f,  0.0f,  0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    -0.3f,  0.0f,  0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    -0.4f,  0.0f,  0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
    -0.4f, -0.2f,  0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
     0.3f, -0.2f,  0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
     0.4f, -0.2f,  0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
     0.4f,  0.0f,  0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
     0.4f,  0.0f,  0.3f,   0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
     0.3f,  0.0f,  0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
     0.3f, -0.2f,  0.3f,   0.0f, 0.0f, 1.0f,       0.0f, 0.0f,


};

float verticesPoste[] = {
    // Front face
    -0.1f, -0.6f, -0.1f,  0.0f, 0.0f,
     0.1f, -0.6f, -0.1f,  1.0f, 0.0f,
     0.1f,  3.5f, -0.1f,  1.0f, 1.0f,
     0.1f,  3.5f, -0.1f,  1.0f, 1.0f,
    -0.1f,  3.5f, -0.1f,  0.0f, 1.0f,
    -0.1f, -0.6f, -0.1f,  0.0f, 0.0f,

    // Back face
    -0.1f, -0.6f,  0.1f,  0.0f, 0.0f,
     0.1f, -0.6f,  0.1f,  1.0f, 0.0f,
     0.1f,  3.5f,  0.1f,  1.0f, 1.0f,
     0.1f,  3.5f,  0.1f,  1.0f, 1.0f,
    -0.1f,  3.5f,  0.1f,  0.0f, 1.0f,
    -0.1f, -0.6f,  0.1f,  0.0f, 0.0f,

    // Left face
    -0.1f, -0.6f,  0.1f,  0.0f, 0.0f,
    -0.1f, -0.6f, -0.1f,  1.0f, 0.0f,
    -0.1f,  3.5f, -0.1f,  1.0f, 1.0f,
    -0.1f,  3.5f, -0.1f,  1.0f, 1.0f,
    -0.1f,  3.5f,  0.1f,  0.0f, 1.0f,
    -0.1f, -0.6f,  0.1f,  0.0f, 0.0f,

    // Right face
     0.1f, -0.6f,  0.1f,  0.0f, 0.0f,
     0.1f, -0.6f, -0.1f,  1.0f, 0.0f,
     0.1f,  3.5f, -0.1f,  1.0f, 1.0f,
     0.1f,  3.5f, -0.1f,  1.0f, 1.0f,
     0.1f,  3.5f,  0.1f,  0.0f, 1.0f,
     0.1f, -0.6f,  0.1f,  0.0f, 0.0f,

    // Top face
    -0.1f,  3.5f, -0.1f,  0.0f, 0.0f,
     0.1f,  3.5f, -0.1f,  1.0f, 0.0f,
     0.1f,  3.5f,  0.1f,  1.0f, 1.0f,
     0.1f,  3.5f,  0.1f,  1.0f, 1.0f,
    -0.1f,  3.5f,  0.1f,  0.0f, 1.0f,
    -0.1f,  3.5f, -0.1f,  0.0f, 0.0f,

    // Bottom face
    -0.1f, -0.6f, -0.1f,  0.0f, 0.0f,
     0.1f, -0.6f, -0.1f,  1.0f, 0.0f,
     0.1f, -0.6f,  0.1f,  1.0f, 1.0f,
     0.1f, -0.6f,  0.1f,  1.0f, 1.0f,
    -0.1f, -0.6f,  0.1f,  0.0f, 1.0f,
    -0.1f, -0.6f, -0.1f,  0.0f, 0.0f,
};


float verticesBandeira[] = {

     0.1f,  3.5f, -0.1f,  0.0f, 0.0f, // Próximo ao topo do poste
     2.1f,  3.5f, -0.1f,  1.0f, 0.0f,
     2.1f,  4.5f, -0.1f,  1.0f, 1.0f,
     2.1f,  4.5f, -0.1f,  1.0f, 1.0f,
     0.1f,  4.5f, -0.1f,  0.0f, 1.0f,
     0.1f,  3.5f, -0.1f,  0.0f, 0.0f,
};



float verticesCubo[] = {
    // Frente
    -0.1f, -0.1f,  0.1f,  0.0f, 0.0f,
     0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
     0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
     0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
    -0.1f,  0.1f,  0.1f,  0.0f, 1.0f,
    -0.1f, -0.1f,  0.1f,  0.0f, 0.0f,

    // Traseira
    -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
     0.1f, -0.1f, -0.1f,  1.0f, 0.0f,
     0.1f,  0.1f, -0.1f,  1.0f, 1.0f,
     0.1f,  0.1f, -0.1f,  1.0f, 1.0f,
    -0.1f,  0.1f, -0.1f,  0.0f, 1.0f,
    -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,

    // Esquerda
    -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
    -0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
    -0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
    -0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
    -0.1f,  0.1f, -0.1f,  0.0f, 1.0f,
    -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,

    // Direita
     0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
     0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
     0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
     0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
     0.1f,  0.1f, -0.1f,  0.0f, 1.0f,
     0.1f, -0.1f, -0.1f,  0.0f, 0.0f,

    // Cima
    -0.1f,  0.1f, -0.1f,  0.0f, 0.0f,
     0.1f,  0.1f, -0.1f,  1.0f, 0.0f,
     0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
     0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
    -0.1f,  0.1f,  0.1f,  0.0f, 1.0f,
    -0.1f,  0.1f, -0.1f,  0.0f, 0.0f,

    // Baixo
    -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
     0.1f, -0.1f, -0.1f,  1.0f, 0.0f,
     0.1f, -0.1f,  0.1f,  1.0f, 1.0f,
     0.1f, -0.1f,  0.1f,  1.0f, 1.0f,
    -0.1f, -0.1f,  0.1f,  0.0f, 1.0f,
    -0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
};


std::vector<float> originalCarVertices(verticesCarro, verticesCarro + sizeof(verticesCarro) / sizeof(float));

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Corrida", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cout << " Ocorreu um erro iniciando GLEW!" << std::endl;
    } else {
        std::cout << "GLEW OK!" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("vertex.glsl", "fragment.glsl");

    Shader carShader("vertex.glsl", "car_shader.glsl");

    Shader bandeiraShader("vertex.glsl", "bandeira_shader.glsl");

    Shader posteShader("vertex.glsl", "poste_shader.glsl");



    Shader cuboShader("vertex.glsl", "bandeira_shader.glsl");


    viraCamera(0.0f, -60.0f);

    GLuint VBOs[5], VAOs[5];
    glGenVertexArrays(5, VAOs);
    glGenBuffers(5, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesChao), verticesChao, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int texture1, texture2, texture3, texture4;
    unsigned char *data;

    //Configuraões dos shaders
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("res/images/pista.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture of race track" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("res/images/car_textura.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture of car" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("res/images/granito.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture of post" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("res/images/brasil.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture of flag" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    carShader.use();
    carShader.setInt("texture2", 0);

    posteShader.use();
    posteShader.setInt("texture3", 0);

    bandeiraShader.use();
    bandeiraShader.setInt("texture4", 0);

    cuboShader.use();
    cuboShader.setInt("texture4", 0);

    while (!glfwWindowShouldClose(window)) {

        // Configuração do carro
        glBindVertexArray(VAOs[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCarro), verticesCarro, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 model = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(70.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 220.0f);

        ourShader.use();
        glBindVertexArray(VAOs[0]);
        ourShader.setMat4("projection", projection);
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));


        ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.0f));
        ourShader.setVec3("objectColor", glm::vec3(0.83f, 0.68f, 0.21f));
        ourShader.setFloat("specularStrength", 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesChao) / (5 * sizeof(float)));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        model = glm::translate(model, glm::vec3(5.5f, -0.2f, 0.0f));

        carShader.use();
        glBindVertexArray(VAOs[1]);
        carShader.setMat4("projection", projection);
        glUniformMatrix4fv(glGetUniformLocation(carShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(carShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesCarro) / (5 * sizeof(float)));



        glBindVertexArray(VAOs[2]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPoste), verticesPoste, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        model = glm::translate(model, glm::vec3(-5.0f, -0.2f, 0.0f));

        posteShader.use();
        glBindVertexArray(VAOs[2]);
        posteShader.setMat4("projection", projection);
        glUniformMatrix4fv(glGetUniformLocation(posteShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(posteShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesPoste) / (5 * sizeof(float)));



        glBindVertexArray(VAOs[3]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBandeira), verticesBandeira, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);
        model = glm::translate(model, glm::vec3(-2.2f, -1.0f, 0.0f));

        bandeiraShader.use();
        glBindVertexArray(VAOs[3]);
        bandeiraShader.setMat4("projection", projection);
        glUniformMatrix4fv(glGetUniformLocation(bandeiraShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(bandeiraShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesBandeira) / (5 * sizeof(float)));




        glBindVertexArray(VAOs[4]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);
        model = glm::translate(model, glm::vec3(5.0f, 3.0f, 0.0f));

        cuboShader.use();
        glBindVertexArray(VAOs[4]);
        cuboShader.setMat4("projection", projection);
        glUniformMatrix4fv(glGetUniformLocation(cuboShader.ID, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(cuboShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesCubo) / (5 * sizeof(float)));


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    glfwTerminate();
    return 0;
}

void viraCamera(float x, float y)
{
    yaw += x * sensitivity;
    pitch += y * sensitivity;

    if(pitch > 89.0f)
        pitch = 89.0f;

    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}


void initializeCarVertices() {
    verticesOriginais.assign(verticesCarro, verticesCarro + sizeof(verticesCarro)/sizeof(float));
}

void updateCarVertices() {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, Car.posicao);
    transform = glm::rotate(transform, glm::radians(Car.angulo), glm::vec3(0.0f, 1.0f, 0.0f));

    for (int i = 0; i < originalCarVertices.size() / 5; ++i) {
        glm::vec4 vertex(
            originalCarVertices[i * 5], // x
            originalCarVertices[i * 5 + 1], // y
            originalCarVertices[i * 5 + 2], // z
            1.0f
        );

        // Transforma o vértice
        glm::vec4 transformed = transform * vertex;

        // Atualiza os vértices
        verticesCarro[i * 5] = transformed.x;
        verticesCarro[i * 5 + 1] = transformed.y;
        verticesCarro[i * 5 + 2] = transformed.z;
        verticesCarro[i * 5 + 3] = originalCarVertices[i * 5 + 3]; // TexCoord X
        verticesCarro[i * 5 + 4] = originalCarVertices[i * 5 + 4]; // TexCoord Y
    }
}

void moveCarFrente() {
    float deltaSpeed = Car.aceleracao;
    float newSpeed = std::max(Car.speed - deltaSpeed, -Car.maxSpeed);

    // Calcula a próxima posição
    float angleRad = glm::radians(Car.angulo);
    glm::vec3 nextPosition = Car.posicao;
    nextPosition.x += newSpeed * cos(angleRad);
    nextPosition.z -= newSpeed * sin(angleRad);

    Car.speed = newSpeed;
    Car.posicao = nextPosition;
    updateCarVertices();

}

void moveCarTras() {
    float deltaSpeed = Car.aceleracao;
    float newSpeed = std::min(Car.speed + deltaSpeed, Car.maxSpeed);

    // Calcula a próxima posição
    float angleRad = glm::radians(Car.angulo);
    glm::vec3 nextPosition = Car.posicao;
    nextPosition.x += newSpeed * cos(angleRad);
    nextPosition.z -= newSpeed * sin(angleRad);


    Car.speed = newSpeed;
    Car.posicao = nextPosition;
    updateCarVertices();

}

void moveCarDir() {
    Car.angulo -= Car.curvaSpeed * (Car.speed != 0 ? 1.0f : 0.0f);
    updateCarVertices();
}

void moveCarEsq() {
    Car.angulo += Car.curvaSpeed * (Car.speed != 0 ? 1.0f : 0.0f);
    updateCarVertices();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    const float cameraSpeed = 0.1f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveCarFrente();

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveCarTras();

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveCarEsq();

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveCarDir();

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += 3*cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        viraCamera(0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
        viraCamera(0.0f, -1.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        viraCamera(-1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        viraCamera(1.0f, 0.0f);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
