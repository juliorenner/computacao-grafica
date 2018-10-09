#include "Mesh.h"
#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "ObjReader.h"
#include "Material.h"
#include "MaterialReader.h"

using namespace std;


void read();
void drawScene();
void prepareVertexArray();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput();
void prepareGroupsVAO();

GLFWwindow *window;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

Mesh* mesh;
vector<Material*> materials;

string OBJ_FILE = "mesa.obj";

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

    window = glfwCreateWindow (SCR_WIDTH, SCR_HEIGHT, "OpenGL exercise", NULL, NULL);
    
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent (window);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // inicia manipulador da extensão GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    read();

    prepareGroupsVAO();
    
//    prepareTextures();

    drawScene();

    glfwTerminate();
    return 0;

    cout << "TESTE\n";
}

 void read() {
     ObjReader* objReader = new ObjReader(OBJ_FILE);
     mesh = objReader->readFile();
     MaterialReader* matReader = new MaterialReader(mesh->getMaterialFile());
     materials = matReader->readFile();
 }

void prepareGroupsVAO() {
    vector<float> vs;
    
    for (int i=0; i < mesh->getGroups().size(); i++) {
        vector<Face*> faces = mesh->getGroups()[i]->getFaces();
        for (int j=0; j < faces.size(); j++){
            vector<int> vertsIndex = faces[j]->getVerts();
            vector<int> normsIndex = faces[j]->getNorms();
            vector<int> textsIndex = faces[j]->getTexts();
            
            if (vertsIndex.size() != normsIndex.size()) {
                throw "Vertex and Normal length differ.";
            }
            
            for (int k=0; k < vertsIndex.size(); k++) {
                glm::vec3 v = mesh->getVertex()[vertsIndex[k]];
                glm::vec3 n = mesh->getNormals()[normsIndex[k]];
                glm::vec2 t = mesh->getMappings()[textsIndex[k]];
                
                vs.push_back(v.x);
                vs.push_back(v.y);
                vs.push_back(v.z);
                vs.push_back(n.x);
                vs.push_back(n.y);
                vs.push_back(n.z);
                vs.push_back(t.x);
                vs.push_back(t.y);
            }
        }
        
        GLuint VBO, VAO = 0;
        
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(float), &vs[0], GL_STATIC_DRAW);
        
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        mesh->getGroups()[i]->setVAOIndex(VAO);
    }
}

void drawScene() {
    
    while (!glfwWindowShouldClose(window)) {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput();
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mesh->getShader()->setMat4("projection", projection);
        
        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        mesh->getShader()->setMat4("view", view);
        
        for (int i=0; i < mesh->getGroups().size(); i++) {
            GLuint VAO = mesh->getGroups()[i]->getVAOIndex();
            glBindVertexArray(VAO);
            
            mesh->getShader()->use();
            
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f));
            float angle = 20.0f * 8;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            mesh->getShader()->setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, mesh->getGroups()[i]->getFaces().size() * 3);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}
