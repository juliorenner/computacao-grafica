#include "Material.h"

Material::Material(string file) {
    this->file = file;
}

string Material::getFile() {
    return this->file;
}
