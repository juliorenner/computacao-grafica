#include "ObjReader.h"

ObjReader::ObjReader(string filename) {
    this->filename = filename;
//    string filepath = "./" + string(filename);
    this->file.open(filename);

    if(this->file.fail()) {
        printf("It was not possible to open the file\n");
    }
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
            
            string v1, v2, v3;
            sline >> v1 >> v2 >> v3; // v/t/n, por exemplo
            stringstream stoken1(v1);
            
            string v, n, t;
            
            getline(stoken1, v, '/');
            getline(stoken1, n, '/');
            getline(stoken1, t, '/');
            
            vert.push_back(stoi(v));
            norm.push_back(stoi(n));
            text.push_back(stoi(t));
            
            stringstream stoken2(v2);
            
            getline(stoken2, v, '/');
            getline(stoken2, n, '/');
            getline(stoken2, t, '/');
            
            vert.push_back(stoi(v));
            norm.push_back(stoi(n));
            text.push_back(stoi(t));
            
            stringstream stoken3(v3);
            
            getline(stoken3, v, '/');
            getline(stoken3, n, '/');
            getline(stoken3, t, '/');
            
            vert.push_back(stoi(v));
            norm.push_back(stoi(n));
            text.push_back(stoi(t));
            
            
            mesh->addFace(vert, norm, text);
        }
        
        
    }

    return mesh;
}
