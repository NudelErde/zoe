//
// Created by Florian on 05.08.2020.
//

#include "Material.h"
#include "api/Shader.h"

namespace Zoe{

Material::Material(const std::shared_ptr<Shader>& shader, const std::vector<std::shared_ptr<Texture>> & textures,
                   const std::function<void(Material *, const mat4x4 &, const mat4x4 &, const mat4x4 &)> & bindingFunction) {
    this->shader = shader;
    this->textures = textures;
    this->bindingFunction = bindingFunction;
}

void Material::bind(const mat4x4 &model, const mat4x4 &view, const mat4x4 &projection) {
    shader->bind();
    bindingFunction(this, model, view, projection);
}

static std::shared_ptr<std::map<std::string, MaterialLibrary>> loadedMaterialLibraries;

MaterialLibrary MaterialLibrary::parseMaterialLibrary(const File &file, bool forceReload) {
    if(!forceReload && loadedMaterialLibraries->count(file.getPath())){
        return loadedMaterialLibraries->at(file.getPath());
    }
    //Create new library
    MaterialLibrary lib;
    //TODO: implement .mtl parsing
    return lib;
}

const Material &MaterialLibrary::get(const std::string & name) {
    return materialMap->at(name);
}

bool MaterialLibrary::hasLibrary(const std::string & name) {
    return materialMap->count(name);
}

MaterialLibrary::MaterialLibrary(MaterialLibrary && other) noexcept {
    materialMap = other.materialMap;
    other.materialMap = nullptr;
}

MaterialLibrary::MaterialLibrary(const MaterialLibrary & other) {
    materialMap = new std::map<std::string, Material>(*other.materialMap);
}

MaterialLibrary &MaterialLibrary::operator=(MaterialLibrary && other) noexcept {
    delete materialMap;
    materialMap = other.materialMap;
    other.materialMap = nullptr;
    return *this;
}

MaterialLibrary &MaterialLibrary::operator=(const MaterialLibrary & other) {
    if(this == &other)
        return *this;
    delete materialMap;
    materialMap = new std::map<std::string, Material>(*other.materialMap);
    return *this;
}

MaterialLibrary::~MaterialLibrary() {
    delete materialMap;
}

MaterialLibrary::MaterialLibrary() {
    materialMap = new std::map<std::string, Material>();
}
}