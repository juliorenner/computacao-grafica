//
//  main.cpp
//  Síntese de Texturas
//
//  Created by Júlio Renner on 31/10/18.
//  Copyright © 2018 RENNERJ. All rights reserved.
//
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <math.h>

using namespace std;

GLFWwindow *window;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Shader* mainShader;

GLuint CP_VBO, CP_SPLINE_VBO, INT_VBO, EXT_VBO;
GLuint CP_VAO, CP_SPLINE_VAO, INT_VAO, EXT_VAO;

vector<float> cpVertix, cpSplineVertix, iVertix, eVertix;

glm::mat4 projection;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void calculateControlPointSpline();
void calculateInternalAndExternalCurves();
void load(GLuint VAO, GLuint VBO, vector<float> &vertix);

int main(int argc, const char * argv[]) {
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif
    
    window = glfwCreateWindow (SCR_WIDTH, SCR_HEIGHT, "Object Writer", NULL, NULL);
    
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent (window);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // inicia manipulador da extensão GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);

    glGenBuffers(1, &CP_VBO);
    glGenVertexArrays(1, &CP_VAO);
    
    glGenBuffers(1, &CP_SPLINE_VBO);
    glGenVertexArrays(1, &CP_SPLINE_VAO);
    
    glGenBuffers(1, &INT_VBO);
    glGenVertexArrays(1, &INT_VAO);

    glGenBuffers(1, &EXT_VBO);
    glGenVertexArrays(1, &EXT_VAO);

    mainShader = new Shader("vs.glsl", "fs.glsl");
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw our first triangle
        mainShader->use();
        
        mainShader->setMat4("projection", projection);
    
        glBindVertexArray(CP_VAO);
        glDrawArrays(GL_LINE_STRIP, 0, cpVertix.size()/2);
        
        if(cpVertix.size() >= 8) {
            
            glBindVertexArray(CP_SPLINE_VAO);
            glDrawArrays(GL_LINE_STRIP, 0, cpSplineVertix.size()/2);

            glBindVertexArray(INT_VAO);
            glDrawArrays(GL_LINE_STRIP, 0, iVertix.size()/2);
            
            glBindVertexArray(EXT_VAO);
            glDrawArrays(GL_LINE_STRIP, 0, eVertix.size()/2);
        }
        
        glBindVertexArray(0);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &CP_VAO);
    glDeleteBuffers(1, &CP_VBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        
        cpVertix.push_back((float)xpos);
        cpVertix.push_back((float)ypos);
        
        if(cpVertix.size() >= 8 && cpVertix.size() % 2 == 0) {
            calculateControlPointSpline();
            calculateInternalAndExternalCurves();
            load(CP_SPLINE_VAO, CP_SPLINE_VBO, cpSplineVertix);
            load(INT_VAO, INT_VBO, iVertix);
            load(EXT_VAO, EXT_VBO, eVertix);
        }
        
        load(CP_VAO, CP_VBO, cpVertix);
    }
}

void calculateControlPointSpline() {
    cpSplineVertix.clear();
    double j = 0.05;
    for (int i = 0; i < cpVertix.size(); i+=2) {
        for (double t = 0; t <= 1; t+=j) {
            float x = ((
                        (-1*pow(t,3) +3*pow(t,2) -3*t +1) * (cpVertix[i]) +
                        ( 3*pow(t,3) -6*pow(t,2) +0*t +4) * (cpVertix[(i+2) % cpVertix.size()]) +
                        (-3*pow(t,3) +3*pow(t,2) +3*t +1) * (cpVertix[(i+4) % cpVertix.size()]) +
                        ( 1*pow(t,3) +0*pow(t,2) +0*t +0) * (cpVertix[(i+6) % cpVertix.size()])
                    )/6);
            float y = ((
                        (-1*pow(t,3) +3*pow(t,2) -3*t +1) * (cpVertix[i+1]) +
                        ( 3*pow(t,3) -6*pow(t,2) +0*t +4) * (cpVertix[(i+3) % cpVertix.size()]) +
                        (-3*pow(t,3) +3*pow(t,2) +3*t +1) * (cpVertix[(i+5) % cpVertix.size()]) +
                        ( 1*pow(t,3) +0*pow(t,2) +0*t +0) * (cpVertix[(i+7) % cpVertix.size()])
                        )/6);
            cpSplineVertix.push_back(x);
            cpSplineVertix.push_back(y);
        }
    }
}

void calculateInternalAndExternalCurves() {
    iVertix.clear();
    eVertix.clear();
    for (int i = 0; i < cpSplineVertix.size(); i+=2) {
        double ax = cpSplineVertix[i % cpSplineVertix.size()];
        double ay = cpSplineVertix[(i+1) % cpSplineVertix.size()];
        
        double bx = cpSplineVertix[(i+2) % cpSplineVertix.size()];
        double by = cpSplineVertix[(i+3) % cpSplineVertix.size()];
        
        double w = bx - ax;
        double h = by - ay;
        
        double angle = atan(h/w);
        
        float internalAngle;
        float externalAngle;
        
        if (w<0) {
            internalAngle = angle + M_PI/2;
            externalAngle = angle - M_PI/2;
        } else {
            internalAngle = angle - M_PI/2;
            externalAngle = angle + M_PI/2;
        }
        
        float iCX = cos(internalAngle)*15 + ax;
        float iCY = sin(internalAngle)*15 + ay;
        
        float eCX = cos(externalAngle)*15 + ax;
        float eCY = sin(externalAngle)*15 + ay;
        
        iVertix.push_back(iCX);
        iVertix.push_back(iCY);
        
        eVertix.push_back(eCX);
        eVertix.push_back(eCY);
    }
    
}

void load(GLuint VAO, GLuint VBO, vector<float> &vertix)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertix.size()*sizeof(float), vertix.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
