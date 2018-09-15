#include "Group.h"

#include <iostream>
using namespace std;

Group::Group(string name, string material) {
    this->name = name;
    this->material = material;
}

void Group::setMaterial(string material) {
    this->material = material;
}

void Group::setName(string name) {
    this->name = name;
}

void Group::addFace(Face* face) {
    this->faces.push_back(face);
}

void Group::addFace(vector<int> verts, vector<int> norms, vector<int> texts) {
    Face* newFace = new Face();
    newFace->setInfo(verts, norms, texts);
    
    this->faces.push_back(newFace);
}

vector<Face*> Group::getFaces() {
    return this->faces;
}

void Group::setVAOIndex(GLuint vaoIndex) {
    this->VAO = vaoIndex;
}