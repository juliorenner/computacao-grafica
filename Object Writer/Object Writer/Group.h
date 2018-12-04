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
        GLuint VAO, textureIndex = 0;

    public:
        int vertexLength = 0;
        Group(string name, string material);
        void addFace(Face* face);
        void addFace(vector<int> verts, vector<int> norms, vector<int> texts);
        void setMaterial(string material);
        void setName(string name);
        void setTextureIndex(GLuint index);
        void setVAOIndex(GLuint vaoIndex);
        GLuint getVAOIndex();
        vector<Face*> getFaces();
        string getMaterial();
        GLuint getTextureIndex();

};

#endif // GROUP_H
