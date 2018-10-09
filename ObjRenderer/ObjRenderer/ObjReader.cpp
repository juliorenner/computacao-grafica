#include "ObjReader.h"

ObjReader::ObjReader(string filename) {
    this->filename = filename;
    this->file.open(filename);

    if(this->file.fail()) {
        printf("It was not possible to open the file\n");
    }
    
    this->mesh = new Mesh();
    mesh->setObjFile(filename);
};

Mesh* ObjReader::readFile() {
    while(!this->file.eof()) {
        string line;
        getline(this->file, line);

        stringstream sline;
        sline << line;
        string temp;
        sline >> temp;

        if (temp == "v") {
            float x, y, z;
            sline >> x >> y >> z;
            mesh->addVertex(x, y, z);
        } else if (temp == "vn") {
            float x, y, z;
            sline >> x >> y >> z;

            mesh->addNormal(x, y, z);
        } else if (temp == "vt") {
            float x, y;
            sline >> x >> y;

            mesh->addMapping(x, y);
        } else if (temp == "f") {
            vector<int> vert;
            vector<int> norm;
            vector<int> text;
            string token, v, n, t;
            
            while (sline.rdbuf()->in_avail()) {
                sline >> token;
                stringstream stoken(token);
                
                getline(stoken, v, '/');
                getline(stoken, n, '/');
                getline(stoken, t, '/');
                
                if (vert.size() == 3) {
                    mesh->addFace(vert, norm, text);
                    vert[1] = stoi(v)-1;
                    norm[1] = stoi(n)-1;
                    text[1] = stoi(t)-1;
                } else {
                    vert.push_back(stoi(v)-1);
                    norm.push_back(stoi(n)-1);
                    text.push_back(stoi(t)-1);
                }
            }
            
            mesh->addFace(vert, norm, text);
        } else if (temp == "g") {
            string groupName;
            sline >> groupName;
            if (mesh->getGroups().size() == 1) {
                mesh->getGroups()[0]->setName(groupName);
            } else {
                mesh->newGroup(groupName);
            }
        } else if (temp == "mtllib") {
            string materialFile;
            sline >> materialFile;
            mesh->setMaterialFile(materialFile);
        } else if (temp == "usemtl") {
            string materialId;
            sline >> materialId;
            mesh->setGroupMaterialId(materialId);
        }
    }
    
    return mesh;
}
