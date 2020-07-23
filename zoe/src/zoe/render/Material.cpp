//
// Created by florian on 14.03.20.
//

#include "Material.h"

#include <utility>
#include <string>
#include "GraphicsContext.h"
#include "../core/Application.h"

namespace Zoe {

template<typename T>
static T fromString(std::string string){
    std::stringstream sstream(string);
    T value;
    sstream >> value;
    return value;
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

Material::Material(const File &materialSource) : shader(
        Application::getContext().getShader(materialSource)) {
    loadTags();
}

Material::Material(std::shared_ptr<Shader> shader) : shader(std::move(shader)) {
    loadTags();
}

void Material::setModelMatrix(const mat4x4 &modelMatrix) {
    this->modelMatrix = modelMatrix;
}

void Material::setViewMatrix(const mat4x4 &viewMatrix) {
    this->viewMatrix = viewMatrix;
}

void Material::setProjectionMatrix(const mat4x4 &projectionMatrix) {
    this->projectionMatrix = projectionMatrix;
}

void Material::setUniform(const std::string &name, float x) {
    shader->setUniform1f(name, x);
}

void Material::setUniform(const std::string &name, vec2 vec) {
    shader->setUniform2f(name, vec.x, vec.y);
}

void Material::setUniform(const std::string &name, vec3 vec) {
    shader->setUniform3f(name, vec.x, vec.y, vec.z);
}

void Material::setUniform(const std::string &name, vec4 vec) {
    shader->setUniform4f(name, vec.x, vec.y, vec.z, vec.w);
}

void Material::setUniform(const std::string &name, const mat3x3 &mat) {
    shader->setUniform3m(name, mat);
}

void Material::setUniform(const std::string &name, const mat4x4 &mat) {
    shader->setUniform4m(name, mat);
}

void Material::bind() {
    shader->bind();
    std::string name = uniformMatrixMap["model"];
    uniformSetter[name](this, name);
    if (name != uniformMatrixMap["view"]) {
        name = uniformMatrixMap["view"];
        uniformSetter[name](this, name);
    }
    if (name != uniformMatrixMap["projection"]) {
        name = uniformMatrixMap["projection"];
        uniformSetter[name](this, name);
    }

    if (!uniformMatrixMap["modelMatrix_it"].empty()) {
        name = uniformMatrixMap["modelMatrix_it"];
        uniformSetter[name](this, name);
    }
}

void Material::loadTags() {
    std::map<std::string, std::string> tags = shader->getTags();
    if (!tags["modelMatrix"].empty()) {
        uniformSetter[tags["modelMatrix"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->modelMatrix);
        };
        uniformMatrixMap["model"] = tags["modelMatrix"];
    } else if (!tags["modelViewMatrix"].empty()) {
        uniformSetter[tags["modelViewMatrix"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->modelMatrix * me->viewMatrix);
        };
        uniformMatrixMap["model"] = tags["modelViewMatrix"];
        uniformMatrixMap["view"] = tags["modelViewMatrix"];
    } else if (!tags["modelViewProjectionMatrix"].empty()) {
        uniformSetter[tags["modelViewProjectionMatrix"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->modelMatrix * me->viewMatrix * me->projectionMatrix);
        };
        uniformMatrixMap["model"] = tags["modelViewProjectionMatrix"];
        uniformMatrixMap["view"] = tags["modelViewProjectionMatrix"];
        uniformMatrixMap["projection"] = tags["modelViewProjectionMatrix"];
    }

    if (!tags["viewMatrix"].empty()) {
        uniformSetter[tags["viewMatrix"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->viewMatrix);
        };
        uniformMatrixMap["view"] = tags["viewMatrix"];
    } else if (!tags["viewProjectionMatrix"].empty()) {
        uniformSetter[tags["viewProjectionMatrix"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->viewMatrix * me->projectionMatrix);
        };
        uniformMatrixMap["view"] = tags["viewProjectionMatrix"];
        uniformMatrixMap["projection"] = tags["viewProjectionMatrix"];
    }

    if (!tags["projectionMatrix"].empty()) {
        uniformSetter[tags["projectionMatrix"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->projectionMatrix);
        };
        uniformMatrixMap["projection"] = tags["projectionMatrix"];
    }

    if (!tags["modelMatrix_it"].empty()) {
        uniformMatrixMap["modelMatrix_it"] = tags["modelMatrix_it"];
        uniformSetter[tags["modelMatrix_it"]] = [](Material *me, const std::string &name) {
            me->setUniform(name, me->modelMatrix.inverse().transpose());
            //throw std::runtime_error("modelMatrix^-T is not implemented!");
        };
    }
}

static vec3 read3Float(std::string string){
    trim(string);
    vec3 value{};
    size_t index;
    if((index = string.find(' ')) != std::string::npos){
        value.x = fromString<float>(string.substr(0, index));
        std::string value23 = string.substr(index+1);
        if((index = value23.find(' ')) != std::string::npos){
            value.y = fromString<float>(value23.substr(0, index));
            value.z = fromString<float>(value23.substr(index+1));
        }else{
            value.y = fromString<float>(value23);
            value.z = value.x;
        }
    }else{
        value.x = fromString<float>(string);
        value.y = value.x;
        value.z = value.x;
    }
    return value;
}

static vec3 readRGB_Spectral_XYZ_Color(std::string valueString){
    bool xyzSpace = false;
    if(valueString.rfind("spectral ") == 0){
        throw std::runtime_error("spectral curves / .rfl files are not supported");
    }else if(valueString.rfind("xyz ") == 0){
        xyzSpace = true;
        valueString = valueString.substr(4);
    }
    vec3 value = read3Float(valueString);
    //TODO: multiply with matrix to convert from xyz space to rgb space if xyzSpace is true
    return value;
}

//TODO: implement this shit: http://paulbourke.net/dataformats/mtl/
//template shader oder so wo dinge mit <name> oder so stehen die dann ersetzt werden?
//wär schon cool
//dieses spectral dingens wird gekonnt raus gelassen
//xyz wird zu fucking rgb gemacht => matrix dann ez
std::map<std::string,Material> Material::loadMaterials(const File &mtlFile) {
    struct MaterialData{
        std::string name;
        vec3 ambientReflectivity{};
        vec3 diffuseReflectivity{};
        vec3 specularReflectivity{};
        vec3 transmissionFilter{};
        int illuminationModel{};
        float dissolve{};
        bool dissolveDependentOnSurfaceOrientation{};
        float specularExponent{};
        float sharpness{};
        float opticalDensity{};
    };
    std::map<std::string, MaterialData> materials;
    std::string materialName;

    std::unique_ptr<std::istream> istream = mtlFile.createIStream(false);
    std::string line;

    while(std::getline(*istream, line)){
        if(line.rfind("newmtl ", 0) == 0){
            std::string name = line.substr(7, std::string::npos);
            materials[name] = MaterialData();
            materials[name].name = name;
            materialName = name;
        }else if(line.rfind("Ka ", 0) == 0){
            std::string valueString = line.substr(3, std::string::npos);
            materials[materialName].ambientReflectivity = readRGB_Spectral_XYZ_Color(valueString);
        }else if(line.rfind("Kd ", 0) == 0){
            std::string valueString = line.substr(3, std::string::npos);
            materials[materialName].diffuseReflectivity = readRGB_Spectral_XYZ_Color(valueString);
        }else if(line.rfind("Ks ", 0) == 0){
            std::string valueString = line.substr(3, std::string::npos);
            materials[materialName].specularReflectivity = readRGB_Spectral_XYZ_Color(valueString);
        }else if(line.rfind("Tf ", 0) == 0){
            std::string valueString = line.substr(3, std::string::npos);
            materials[materialName].transmissionFilter = readRGB_Spectral_XYZ_Color(valueString);
        }else if(line.rfind("illum ", 0) == 0){
            std::string valueString = line.substr(6, std::string::npos);
            materials[materialName].illuminationModel = fromString<int>(valueString);
        }else if(line.rfind("d ", 0) == 0){
            std::string valueString = line.substr(2, std::string::npos);
            if(valueString.find(" -halo ") != std::string::npos){
                materials[materialName].dissolveDependentOnSurfaceOrientation = true;
                valueString = line.substr(8, std::string::npos);
            }else{
                materials[materialName].dissolveDependentOnSurfaceOrientation = false;
            }
            materials[materialName].dissolve = fromString<float>(valueString);
        }else if(line.rfind("Ns ", 0) == 0){
            std::string valueString = line.substr(2, std::string::npos);
            materials[materialName].specularExponent = fromString<float>(valueString);
        }else if(line.rfind("sharpness ", 0) == 0){
            std::string valueString = line.substr(10, std::string::npos);
            materials[materialName].sharpness = fromString<float>(valueString);
        }else if(line.rfind("Ni", 0) == 0){
            std::string valueString = line.substr(2, std::string::npos);
            materials[materialName].opticalDensity = fromString<float>(valueString);
        }
    }
    return std::map<std::string,Material>();
}

Material::Material() = default;
}