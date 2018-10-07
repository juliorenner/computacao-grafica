#include "Mesh.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "ObjReader.h"

using namespace std;

void read();
void drawScene();
void prepareVertexArray();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput();

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
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif

    window = glfwCreateWindow (1000, 1000, "OpenGL exercise", NULL, NULL);
    
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
    
    read();

    mesh->prepareGroupsVAO();

    drawScene();

    glfwTerminate();
    return 0;

    cout << "TESTE\n";
}

 void read() {
     ObjReader* reader = new ObjReader("cube.obj");
     mesh = reader->readFile();
 }

void drawScene() {
    while (!glfwWindowShouldClose(window)) {
        void processInput();
        
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        mesh->draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
