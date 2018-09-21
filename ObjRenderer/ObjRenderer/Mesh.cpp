#include "Mesh.h"

Mesh::Mesh() {
    Group* defaultGroup = new Group("default", "");
    this->groups.push_back(defaultGroup);
    this->mainShader = new Shader("vs.glsl", "fs.glsl");
    this->activeGroup = 0;
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

int Mesh::newGroup(string name, string material) {
    Group* group = new Group(name, material);
    this->groups.push_back(group);

    int groupIndex = this->groups.size() - 1;

    this->setActiveGroup(groupIndex);

    return groupIndex;
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

void Mesh::prepareGroupsVAO() {
    vector<float> vs;

    for (int i=0; i < this->groups.size(); i++) {
        vector<Face*> faces = this->groups[i]->getFaces();
        for (int j=0; j < faces.size(); j++){
            vector<int> vertsIndex = faces[j]->getVerts();
            vector<int> normsIndex = faces[j]->getNorms();
            
            if (vertsIndex.size() != normsIndex.size()) {
                throw "Vertex and Normal length differ.";
            }

            for (int k=0; k < vertsIndex.size(); k++) {
                glm::vec3 v = this->vertex[vertsIndex[k]-1];
                glm::vec3 n = this->normals[normsIndex[k]-1];

                vs.push_back(v.x);
                vs.push_back(v.y);
                vs.push_back(v.z);
//                vs.push_back(n.x);
//                vs.push_back(n.y);
//                vs.push_back(n.z);
            }
        }

        GLuint VBO, VAO = 0;

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        // glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(float), &vs[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

//        // normal attribute
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//        glEnableVertexAttribArray(1);


        this->groups[i]->setVAOIndex(VAO);
    }
}

void Mesh::draw() {
    for (int i=0; i < this->groups.size(); i++) {
        GLuint VAO = this->groups[i]->getVAOIndex();
        
        mainShader->use();
        // create transformations
        glm::mat4 projection, model, view;
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(mainShader->ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(mainShader->ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        projection = glm::perspective(glm::radians(45.0f), (float)1000 / (float)1000, 0.1f, 100.0f);
        mainShader->setMat4("projection", projection);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
