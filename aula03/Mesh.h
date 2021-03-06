#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <vector> 
#include <iostream>
#include <glm/glm.hpp>
#include "Group.h"
#include "Shader.h"

using namespace std;

class Mesh {
    private:
        vector<glm::vec2> mappings;
        vector<glm::vec3> vertex;
        vector<glm::vec3> normals;
        vector<Group*> groups;
        int activeGroup;

    public:
        Mesh();
        void addMapping(float x, float y);
        void addVertex(float x, float y, float z);
        void addNormal(float x, float y, float z);
        int newGroup(string name, string material);
        void setActiveGroup(int groupPointer);
        void addFace(vector<int> verts, vector<int> norms, vector<int> texts);
        void addFace(Face* face);
        void prepareGroupsVAO();
        void draw();
};

#endif // MESH_H