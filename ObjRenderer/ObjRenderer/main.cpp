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
void processInput();
void prepareGroupsVAO();

GLFWwindow *window;

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
        void processInput();
        
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        for (int i=0; i < mesh->getGroups().size(); i++) {
            GLuint VAO = mesh->getGroups()[i]->getVAOIndex();
            
            mesh->getShader()->use();
            // create transformations
            glm::mat4 projection, model, view;
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            // retrieve the matrix uniform locations
            unsigned int modelLoc = glGetUniformLocation(mesh->getShader()->ID, "model");
            unsigned int viewLoc  = glGetUniformLocation(mesh->getShader()->ID, "view");
            // pass them to the shaders (3 different ways)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            projection = glm::perspective(glm::radians(45.0f), (float)1000 / (float)1000, 0.1f, 100.0f);
            mesh->getShader()->setMat4("projection", projection);
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
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
