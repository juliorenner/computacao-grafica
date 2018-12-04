#ifndef FACE_H
#define FACE_H

#include <vector>

using namespace std;

class Face{
    private:
        vector<int> verts;
        vector<int> norms;
        vector<int> texts;

    public:
        void addVertexInfo(int vertexIndex, int textureIndex, int normalIndex);
        void setInfo(vector<int> verts, vector<int> norms, vector<int> texts);
        void setVerts(vector<int> verts);
        void setNorms(vector<int> norms);
        void setTexts(vector<int> texts);
        vector<int> getVerts();
        vector<int> getNorms();
        vector<int> getTexts();
};  

#endif // FACE_H
