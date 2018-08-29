#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

using namespace std;

GLFWwindow *window;
double previous_seconds;
int frame_count; 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput();
void updateFPS ();

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
    glewInit ();

    float vertices[] = {
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
        0.2f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
        -0.75f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
        0.2f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
        0.4f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint VBO, VAO = 0;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::string basePath("/Users/juliorenner/Documents/git/computacao-grafica/aula01/");
    std::string vertexPath = basePath + std::string("vs.glsl");
    std::string fragmentPath = basePath + std::string("fs.glsl");
    Shader mainShader(vertexPath.c_str(), fragmentPath.c_str());

    // Exercise B
    std::string fragmentBorderPath = basePath + std::string("fs_bordas.glsl");
    Shader border(vertexPath.c_str(), fragmentBorderPath.c_str());
    // Exercise B

    while(!glfwWindowShouldClose(window))
    {
        processInput();

        updateFPS();

        glClearColor(0.5f, 0.73f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mainShader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Exercise B
        border.use();
        glDrawArrays(GL_LINE_LOOP, 0, 6);
        // Exercise B

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glfwTerminate();
    return 0;
}

void processInput()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void updateFPS () {
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;

    char tmp[128];
    
    double fps = (double) frame_count / elapsed_seconds;
    sprintf (tmp, "opengl @ fps: %.2f", fps);

    glfwSetWindowTitle (window, tmp);

    previous_seconds = current_seconds;
    frame_count++;
} 