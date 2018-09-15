#include "Mesh.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

using namespace std;

void read();
void drawScene();
void prepareVertexArray();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow *window;

Mesh* mesh;

int main () {

    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    /* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
    /* Definir como 3.2 para Apple OS X */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

    window = glfwCreateWindow (640, 480, "OpenGL exercise", NULL, NULL);
    
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent (window);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback); 

    // inicia manipulador da extensão GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    mesh = new Mesh();

    read();

    mesh->prepareGroupsVAO();

    while (!glfwWindowShouldClose(window)) {
        // mesh->draw();

    }

    cout << "TESTE\n";
}

void read() {
    mesh->addVertex(1,1,1);
    mesh->addVertex(-1,1,1);
    mesh->addVertex(-1,-1,1);
    mesh->addVertex(1,-1,1);
    mesh->addVertex(1,1,-1);
    mesh->addVertex(-1,1,-1);
    mesh->addVertex(-1,-1,-1);
    mesh->addVertex(1,-1,-1);

    mesh->addNormal(1,0,0);
    mesh->addNormal(-1,0,0);
    mesh->addNormal(0,1,0);
    mesh->addNormal(0,-1,0);
    mesh->addNormal(0,0,1);
    mesh->addNormal(0,0,-1);

    vector<int> vertex1;
    vertex1.push_back(0);
    vertex1.push_back(1);
    vertex1.push_back(2);
    vertex1.push_back(3);

    vector<int> norm1;
    norm1.push_back(4);
    norm1.push_back(4);
    norm1.push_back(4);
    norm1.push_back(4);

    vector<int> text1;

    mesh->addFace(vertex1, norm1, text1);

    vector<int> vertex2;
    vertex2.push_back(0);
    vertex2.push_back(3);
    vertex2.push_back(7);
    vertex2.push_back(4);

    vector<int> norm2;
    norm2.push_back(0);
    norm2.push_back(0);
    norm2.push_back(0);
    norm2.push_back(0);

    vector<int> text2;

    mesh->addFace(vertex2, norm2, text2);

    vector<int> vertex3;
    vertex3.push_back(0);
    vertex3.push_back(4);
    vertex3.push_back(5);
    vertex3.push_back(1);

    vector<int> norm3;
    norm3.push_back(2);
    norm3.push_back(2);
    norm3.push_back(2);
    norm3.push_back(2);

    vector<int> text3;

    mesh->addFace(vertex3, norm3, text3);

    vector<int> vertex4;
    vertex4.push_back(2);
    vertex4.push_back(1);
    vertex4.push_back(5);
    vertex4.push_back(6);

    vector<int> norm4;
    norm4.push_back(1);
    norm4.push_back(1);
    norm4.push_back(1);
    norm4.push_back(1);

    vector<int> text4;

    mesh->addFace(vertex4, norm4, text4);

    vector<int> vertex5;
    vertex5.push_back(3);
    vertex5.push_back(2);
    vertex5.push_back(6);
    vertex5.push_back(7);

    vector<int> norm5;
    norm5.push_back(3);
    norm5.push_back(3);
    norm5.push_back(3);
    norm5.push_back(3);

    vector<int> text5;

    mesh->addFace(vertex5, norm5, text5);

    vector<int> vertex6;
    vertex6.push_back(7);
    vertex6.push_back(6);
    vertex6.push_back(5);
    vertex6.push_back(4);

    vector<int> norm6;
    norm6.push_back(5);
    norm6.push_back(5);
    norm6.push_back(5);
    norm6.push_back(5);

    vector<int> text6;

    mesh->addFace(vertex6, norm6, text6);
}

// void prepareVertexArray() {
//     mesh->
// }

void drawScene() {

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}