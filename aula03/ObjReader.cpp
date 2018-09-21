#include "ObjReader.h"

ObjReader::ObjReader(string filename) {
    this->filename = filename;
    string basePath("/Users/juliorenner/Documents/git/computacao-grafica/aula03/");
    string filepath = basePath + string(filename);
    this->file.open(filepath);
};

Mesh* ObjReader::readFile() {
    Mesh* mesh = new Mesh();
    
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
        } 
        else if (temp == "vn") {
            float x, y, z;
            sline >> x >> y >> z;

            mesh->addNormal(x, y, z);
        } 
        else if (temp == "vt") {
            float x, y;
            sline >> x >> y;

            mesh->addMapping(x, y);
        } else if (temp == "f") {
            string token;
            sline >> token; // v/t/n, por exemplo
            stringstream stoken;
            string aux;
            getline(stoken, aux, '/');
            // mesh->addFace()
        }
        
        
    }

    return mesh;
}