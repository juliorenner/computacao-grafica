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

#define BUFSIZE 512

using namespace std;

GLFWwindow *window;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Shader* cpShader;
Shader* curvesShader;

GLuint CP_VBO, CP_SPLINE_VBO, INT_VBO, EXT_VBO;
GLuint CP_VAO, CP_SPLINE_VAO, INT_VAO, EXT_VAO;

// Modes: (E) edit = 0 (D) draw = 1 (Z) select mode = 2
GLuint renderMode = 0;
GLuint selectedPointIndex = -15;

vector<float> cpVertix, cpSplineVertix, iVertix, eVertix;

glm::mat4 projection;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void calculateControlPointSpline();
void calculateInternalAndExternalCurves();
int getNearestPoint(double x, double y);
void selectPoint(double x, double y);
void updateZIndex(bool isIncreasing);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void loadCurves(GLuint VAO, GLuint VBO, vector<float> &vertix);
void loadControlPoints();


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
    glfwSetKeyCallback(window, key_callback);
    
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
    
//    glGenBuffers(1, &CP_COLOR_VBO);

    curvesShader = new Shader("vs.glsl", "fs.glsl");
    cpShader = new Shader("vs_cp.glsl", "fs_cp.glsl");
    
    glPointSize(12);
    
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
        
        if (renderMode != 1) {
            cpShader->use();
            cpShader->setMat4("projection", projection);
            
            glBindVertexArray(CP_VAO);
            glDrawArrays(GL_POINTS, 0, cpVertix.size()/6);
            glDrawArrays(GL_LINE_STRIP, 0, cpVertix.size()/6);
        }
        
        if(cpVertix.size() >= 12) {
            curvesShader->use();
            curvesShader->setMat4("projection", projection);
            glBindVertexArray(CP_SPLINE_VAO);
            glDrawArrays(GL_LINE_STRIP, 0, cpSplineVertix.size()/3);

            if (renderMode == 1) {
                glBindVertexArray(INT_VAO);
                glDrawArrays(GL_LINE_STRIP, 0, iVertix.size()/3);
                
                glBindVertexArray(EXT_VAO);
                glDrawArrays(GL_LINE_STRIP, 0, eVertix.size()/3);
            }
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        renderMode = 0;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        renderMode = 1;
        calculateInternalAndExternalCurves();
        
        loadCurves(INT_VAO, INT_VBO, iVertix);
        loadCurves(EXT_VAO, EXT_VBO, eVertix);
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        selectedPointIndex = -15;
        renderMode = 2;
    }
    if ((key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) && action == GLFW_PRESS && renderMode == 2 && selectedPointIndex >= 0) {
        updateZIndex(key == GLFW_KEY_UP);
        loadControlPoints();
    }
}

void updateZIndex(bool isIncreasing) {
    if (isIncreasing) {
        if (cpVertix[selectedPointIndex + 5] > 0)
            cpVertix[selectedPointIndex + 5] -= 0.1;
        else if (cpVertix[selectedPointIndex + 4] < 1){
            cpVertix[selectedPointIndex + 4] += 0.1;
        }
    } else {
        if (cpVertix[selectedPointIndex + 4] > 0)
            cpVertix[selectedPointIndex + 4] -= 0.1;
        else if (cpVertix[selectedPointIndex + 5] < 1)
            cpVertix[selectedPointIndex + 5] += 0.1;
    }
    float zIndex = cpVertix[selectedPointIndex + 4] - cpVertix[selectedPointIndex + 5];
    if (zIndex > 1) {
        zIndex = 1.0f;
    } else if (zIndex < -1){
        zIndex = -1.0f;
    }
    
    cpVertix[selectedPointIndex + 2] = zIndex;
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
        
        if (renderMode == 0) {
            cpVertix.push_back((float)xpos);
            cpVertix.push_back((float)ypos);
            cpVertix.push_back(0);
            cpVertix.push_back(1);
            cpVertix.push_back(0);
            cpVertix.push_back(0);
        }
        
        if (renderMode == 2) {
            selectPoint(xpos, ypos);
        }
        
        if(cpVertix.size() >= 12) {
            calculateControlPointSpline();
            loadCurves(CP_SPLINE_VAO, CP_SPLINE_VBO, cpSplineVertix);
        }
        
        loadControlPoints();
    }
    
    glRenderMode(GL_RENDER);
}

void selectPoint(double x, double y) {
    selectedPointIndex = getNearestPoint(x, y);
    loadControlPoints();
}

int getNearestPoint(double x, double y) {
    int nearest = 0;
    double distance = INFINITY;
    for(int i = 0; i < cpVertix.size(); i += 6) {
        double d = sqrt( pow((cpVertix[i]-x),2) + pow(cpVertix[i+1],2) );
        if (d < distance) {
            distance = d;
            nearest = i;
        }
    }
    
    return nearest;
}

void calculateControlPointSpline() {
    cpSplineVertix.clear();
    double j = 0.05;
    for (int i = 0; i < cpVertix.size(); i+=6) {
        for (double t = 0; t <= 1; t+=j) {
            float x = ((
                        (-1*pow(t,3) +3*pow(t,2) -3*t +1) * (cpVertix[i]) +
                        ( 3*pow(t,3) -6*pow(t,2) +0*t +4) * (cpVertix[(i+(6*1)) % cpVertix.size()]) +
                        (-3*pow(t,3) +3*pow(t,2) +3*t +1) * (cpVertix[(i+(6*2)) % cpVertix.size()]) +
                        ( 1*pow(t,3) +0*pow(t,2) +0*t +0) * (cpVertix[(i+(6*3)) % cpVertix.size()])
                    )/6);
            float y = ((
                        (-1*pow(t,3) +3*pow(t,2) -3*t +1) * (cpVertix[i+1]) +
                        ( 3*pow(t,3) -6*pow(t,2) +0*t +4) * (cpVertix[((i+1)+(6*1)) % cpVertix.size()]) +
                        (-3*pow(t,3) +3*pow(t,2) +3*t +1) * (cpVertix[((i+1)+(6*2)) % cpVertix.size()]) +
                        ( 1*pow(t,3) +0*pow(t,2) +0*t +0) * (cpVertix[((i+1)+(6*3)) % cpVertix.size()])
                        )/6);
            cpSplineVertix.push_back(x);
            cpSplineVertix.push_back(y);
            cpSplineVertix.push_back(0);
        }
    }
}

void calculateInternalAndExternalCurves() {
    iVertix.clear();
    eVertix.clear();
    for (int i = 0; i < cpSplineVertix.size(); i+=3) {
        double ax = cpSplineVertix[i % cpSplineVertix.size()];
        double ay = cpSplineVertix[(i+1) % cpSplineVertix.size()];
        
        double bx = cpSplineVertix[(i+3) % cpSplineVertix.size()];
        double by = cpSplineVertix[(i+4) % cpSplineVertix.size()];
        
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
        iVertix.push_back(0);
        
        eVertix.push_back(eCX);
        eVertix.push_back(eCY);
        eVertix.push_back(0);
    }
    
}

void loadCurves(GLuint VAO, GLuint VBO, vector<float> &vertix)
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertix.size()*sizeof(float), vertix.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void loadControlPoints()
{
    glBindVertexArray(CP_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, CP_VBO);
    glBufferData(GL_ARRAY_BUFFER, cpVertix.size()*sizeof(float), cpVertix.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
//    glBindBuffer(GL_ARRAY_BUFFER, CP_COLOR_VBO);
//    glBufferData(GL_ARRAY_BUFFER, cpColors.size()*sizeof(float), cp`.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
