#include "Mesh.h"

Mesh::Mesh() {
    Group* defaultGroup = new Group("default", "");
    this->groups.push_back(defaultGroup);
    this->mainShader = new Shader("vs.glsl", "fs.glsl");
    this->activeGroup = 0;
}

void Mesh::setObjFile(string objFile) {
    this->objFile = objFile;
}

Shader* Mesh::getShader() {
    return this->mainShader;
}

void Mesh::addMapping(float x, float y) {
    glm::vec2 mapping = glm::vec2(x, y);
    this->mappings.push_back(mapping);
}

void Mesh::addNormal(float x, float y, float z) {
    glm::vec3 normal = glm::vec3(x, y, z);
    this->normals.push_back(normal);

    // cout << "new normals size: " << this->normals.size() << "\n";
}

void Mesh::addVertex(float x, float y, float z) {
    glm::vec3 vertex = glm::vec3(x, y, z);
    this->vertex.push_back(vertex);
}

int Mesh::newGroup(string name) {
    Group* group = new Group(name, "");
    this->groups.push_back(group);

    int groupIndex = this->groups.size() - 1;

    this->setActiveGroup(groupIndex);

    return groupIndex;
}

void Mesh::setGroupMaterialId(string material) {
    this->groups[this->activeGroup]->setMaterial(material);
}

void Mesh::setMaterialFile(string materialFile) {
    this->materialFile = materialFile;
}

string Mesh::getMaterialFile() {
    string materialFile;
    if (this->materialFile.size() > 0) {
        materialFile = this->materialFile;
    } else {
        materialFile = this->objFile.substr(0, this->objFile.find(".")) + ".mtl";
    }
    
    return this->materialFile;
}

void Mesh::setActiveGroup(int index) {
    this->activeGroup = index;
}

void Mesh::addFace(vector<int> verts, vector<int> norms, vector<int> texts) {
    Group* group = this->groups[this->activeGroup];
    group->addFace(verts, norms, texts);
}

void Mesh::addFace(Face* face) {
    Group* group = this->groups[this->activeGroup];
    group->addFace(face);
}

vector<Group*> Mesh::getGroups() {
    return this->groups;
}

vector<glm::vec3> Mesh::getVertex() {
    return this->vertex;
}

vector<glm::vec2> Mesh::getMappings() {
    return this->mappings;
}

vector<glm::vec3> Mesh::getNormals() {
    return this->normals;
}
