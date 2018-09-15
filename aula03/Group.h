#ifndef GROUP_H
#define GROUP_H

#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h>
#include <string>
#include <vector> 
#include "Face.h"
#include <iostream>

using namespace std;

class Group {
    private:
        string name;
        string material;
        vector<Face*> faces;
        GLuint VAO = 0;

    public:
        Group(string name, string material);
        void addFace(Face* face);
        void addFace(vector<int> verts, vector<int> norms, vector<int> texts);
        void setMaterial(string material);
        void setName(string name);
        vector<Face*> getFaces();
        void setVAOIndex(GLuint vaoIndex);

};

#endif // GROUP_H