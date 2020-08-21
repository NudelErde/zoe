//
// Created by Florian on 05.08.2020.
//

#include "Material.h"
#include "api/Shader.h"
#include "../core/String.h"
#include "../core/Application.h"

namespace Zoe {

Material::Material(const std::shared_ptr<Shader> &shader, const std::vector<std::shared_ptr<Texture>> &textures,
                   const std::function<void(Material *, const Camera&, const mat4x4&)> &bindingFunction) {
    this->shader = shader;
    this->textures = textures;
    this->bindingFunction = bindingFunction;
}

void Material::bind(const Camera& camera, const mat4x4& model) {
    if(shader){
        shader->bind();
        bindingFunction(this, camera, model);
    }
}

Material::Material() = default;

static std::shared_ptr<std::map<std::string, MaterialLibrary>> loadedMaterialLibraries = std::make_shared<std::map<std::string, MaterialLibrary>>();

static std::pair<std::string, std::string> getStringTo(const std::string &str, char c) {
    std::stringstream ss;
    std::pair<std::string, std::string> pair;
    bool hasSwitched = false;
    for (const char *ptr = str.c_str(); *ptr; ++ptr) {
        if (!hasSwitched && *ptr == c) {
            hasSwitched = true;
            pair.first = ss.str();
            ss.str("");
        } else {
            ss << *ptr;
        }
    }
    pair.second = ss.str();
    return pair;
}

static vec3 readVec3(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    std::stringstream ss;
    vec3 result{};
    if (begin == end) {
        return vec3({0, 0, 0});
    }
    ss.str(*(begin++));
    ss >> result.x;
    if (begin == end) {
        result.y = result.x;
        result.z = result.x;
    }
    ss.str(*(begin++));
    ss >> result.y;
    if (begin == end) {
        result.z = result.x;
    }
    ss.str(*(begin++));
    ss >> result.z;
    return result;
}

//TODO: insert real xyz to rgb matrix
static vec3 readRGB_XYZ(const std::string &str) {
    static mat3x3 convertXYZToRGBMatrix = []() {
        mat3x3 mat;
        mat[0][0] = 1;
        mat[0][1] = 0;
        mat[0][1] = 0;
        mat[1][0] = 0;
        mat[1][1] = 1;
        mat[1][1] = 0;
        mat[2][0] = 0;
        mat[2][1] = 0;
        mat[2][1] = 1;
        return mat;
    }();
    //TODO: throw exception if spectral curve is used or implement spectral curves
    std::vector<std::string> vec = split(str, ' ');
    std::string first = vec[0];
    toLower(first);
    if (first == "xyz") {
        return convertXYZToRGBMatrix * readVec3(++vec.begin(), vec.end());
    } else {
        return readVec3(vec.begin(), vec.end());
    }
}

MaterialLibrary MaterialLibrary::parseMaterialLibrary(const File &file, bool forceReload) {
    static std::shared_ptr<Shader> phongShader = Application::getContext().getShader(
            File("virtual/zoe/materials/TemplatePhong.glsl"));
    if (!forceReload && loadedMaterialLibraries->count(file.getPath())) {
        return loadedMaterialLibraries->at(file.getPath());
    }
    //Create new library
    MaterialLibrary lib;

    struct MaterialData {
        bool init = false;
        std::string name;
        vec3 ambientReflectivity{};
        vec3 diffuseReflectivity{};
        vec3 specularReflectivity{};
        vec3 emissivityCoefficient{};
        vec3 transmissionFilter{};
        int illuminationModel{};
        float dissolve{};
        bool dissolveDependentOnSurfaceOrientation{};
        float specularExponent{};
        float sharpness{};
        float opticalDensity{};
    };
    std::vector<MaterialData> materials;
    MaterialData currentMaterial;
    std::unique_ptr<std::istream> stream = file.createIStream(false);
    std::istream &is = *stream;
    std::string line;
    int lineNumber = 0;
    while (getline(is, line)) {
        ++lineNumber;
        trim(line);
        if (startsWith(line, "#")) {
            continue;
        }
        auto[cmd, args] = getStringTo(line, ' ');
        std::vector<std::string> argsList = split(args, ' ');
        trim(cmd);
        toLower(cmd);
        if(cmd.empty())
            continue;

        if (cmd == "newmtl") {
            if (currentMaterial.init) {
                materials.push_back(currentMaterial);
            }
            currentMaterial = MaterialData();
            currentMaterial.init = true;
            currentMaterial.name = args;
        } else if (!currentMaterial.init) {
            //no currentMaterial
            throw std::runtime_error(".mtl file is wrong formatted in line: " + std::to_string(lineNumber));
        } else if (cmd == "ka") {
            currentMaterial.ambientReflectivity = readRGB_XYZ(args);
        } else if (cmd == "kd") {
            currentMaterial.diffuseReflectivity = readRGB_XYZ(args);
        } else if (cmd == "ks") {
            currentMaterial.specularReflectivity = readRGB_XYZ(args);
        } else if (cmd == "ke") {
            currentMaterial.emissivityCoefficient = readRGB_XYZ(args);
        } else if (cmd == "tf") {
            currentMaterial.transmissionFilter = readRGB_XYZ(args);
        } else if (cmd == "illum") {
            currentMaterial.illuminationModel = fromString<int>(args);
        } else if (cmd == "d") {
            if (argsList[0] == "-halo") {
                currentMaterial.dissolveDependentOnSurfaceOrientation = true;
                currentMaterial.dissolve = fromString<float>(argsList[1]);
            } else {
                currentMaterial.dissolveDependentOnSurfaceOrientation = false;
                currentMaterial.dissolve = fromString<float>(argsList[0]);
            }
        } else if (cmd == "tr") {
            //(inverted: Tr = 1 - d)
            currentMaterial.dissolveDependentOnSurfaceOrientation = false;
            currentMaterial.dissolve = 1.0f - fromString<float>(argsList[0]);
        } else if (cmd == "ns") {
            currentMaterial.specularExponent = fromString<float>(args);
        } else if (cmd == "sharpness") {
            currentMaterial.sharpness = fromString<float>(args);
        } else if (cmd == "ni") {
            currentMaterial.opticalDensity = fromString<float>(args);
        } else {
            warning("Unknown or unsupported attribute \"", cmd, " ", args, "\" in file ", file.getPath());
        }
    }
    if (currentMaterial.init) {
        materials.push_back(currentMaterial);
    }

    for (MaterialData &data: materials) {
        lib.materialMap->operator[](data.name) = Material(phongShader, std::vector<std::shared_ptr<Texture>>(),
                     [data](Material *me, const Camera& camera, const mat4x4& model) {
                         Shader &shader = *(me->shader);
                         shader.bind();
                         const std::map<std::string, std::string> &tags = shader.getTags();
                         bool modelUniformIsSet = false;
                         bool viewUniformIsSet = false;
                         bool projectUniformIsSet = false;
                         //some logic is simplified, could be solved smart and shit but no
                         if (tags.count("modelMatrix") != 0) {
                             modelUniformIsSet = true;
                             shader.setUniform4m(tags.at("modelMatrix"), model);
                         }
                         if (!modelUniformIsSet && tags.count("modelViewMatrix")) {
                             modelUniformIsSet = true;
                             viewUniformIsSet = true;
                             shader.setUniform4m(tags.at("modelViewMatrix"), model * camera.getViewMatrix());
                         }
                         if (!modelUniformIsSet && !viewUniformIsSet && tags.count("modelViewProjectionMatrix")) {
                             viewUniformIsSet = true;
                             projectUniformIsSet = true;
                             shader.setUniform4m(tags.at("modelViewProjectionMatrix"), model * camera.getViewMatrix() * camera.getProjectionMatrix());
                         }
                         if (!viewUniformIsSet && tags.count("viewMatrix")) {
                             viewUniformIsSet = true;
                             shader.setUniform4m(tags.at("viewMatrix"), camera.getViewMatrix());
                         }
                         if (!viewUniformIsSet && tags.count("viewProjectionMatrix")) {
                             shader.setUniform4m(tags.at("viewProjectionMatrix"), camera.getViewMatrix() * camera.getProjectionMatrix());
                         }
                         if (!projectUniformIsSet && tags.count("projectionMatrix")) {
                             shader.setUniform4m(tags.at("projectionMatrix"), camera.getProjectionMatrix());
                         }
                         if (tags.count("modelMatrix_it")) {
                             shader.setUniform4m(tags.at("modelMatrix_it"), model.transpose().inverse());
                         }
                         shader.setUniform3f("ambientReflectivity_mtl", data.ambientReflectivity.x,
                                             data.ambientReflectivity.y, data.ambientReflectivity.z);
                         shader.setUniform3f("diffuseReflectivity_mtl", data.diffuseReflectivity.x,
                                             data.diffuseReflectivity.y, data.diffuseReflectivity.z);
                         shader.setUniform3f("specularReflectivity_mtl", data.specularReflectivity.x,
                                             data.specularReflectivity.y, data.specularReflectivity.z);
                         shader.setUniform1f("specularExponent_mtl", data.specularExponent);
                         vec3 cameraPos = camera.getPosition();
                         shader.setUniform3f("cameraPosition", cameraPos.x, cameraPos.y, cameraPos.z);

                         //TODO: source is enough documentation
                         shader.setUniform3f("lightPosition", 10, 10, 10);
                         shader.setUniform3f("lightColor", 0.5f, 1, 0);
                     });
    }
    loadedMaterialLibraries->operator[](file.getPath()) = lib;
    return lib;
}

const Material &MaterialLibrary::get(const std::string &name) const {
    return materialMap->at(name);
}

bool MaterialLibrary::hasLibrary(const std::string &name) const {
    return materialMap->count(name);
}

MaterialLibrary::MaterialLibrary(MaterialLibrary &&other) noexcept {
    materialMap = other.materialMap;
    other.materialMap = nullptr;
}

MaterialLibrary::MaterialLibrary(const MaterialLibrary &other) {
    materialMap = new std::map<std::string, Material>(*other.materialMap);
}

MaterialLibrary &MaterialLibrary::operator=(MaterialLibrary &&other) noexcept {
    delete materialMap;
    materialMap = other.materialMap;
    other.materialMap = nullptr;
    return *this;
}

MaterialLibrary &MaterialLibrary::operator=(const MaterialLibrary &other) {
    if (this == &other)
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