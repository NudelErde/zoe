//
// Created by Florian on 05.08.2020.
//

#include "Material.h"
#include "api/Shader.h"
#include "../core/String.h"
#include "../core/Application.h"

namespace Zoe {

Material::Material(const std::shared_ptr<Shader> &shader,
                   const std::map<std::string, std::shared_ptr<Texture>> &textures,
                   const std::function<void(Material *, const Camera &, const mat4x4 &)> &bindingFunction) {
    this->shader = shader;
    this->textures = textures;
    this->bindingFunction = bindingFunction;
}

void Material::bind(const Camera &camera, const mat4x4 &model) {
    if (shader) {
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
    vec3 result{};
    if (begin == end) {
        return vec3({0, 0, 0});
    }
    result.x = fromString<float>(*(begin++));
    if (begin == end) {
        result.y = result.x;
        result.z = result.x;
        return result;
    }
    result.y = fromString<float>(*(begin++));
    if (begin == end) {
        result.z = result.x;
        return result;
    }
    result.z = fromString<float>(*(begin++));
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
    static std::shared_ptr<Shader> phongShaders[8]{};
    for(unsigned int i = 0; i < 8; ++i){
        std::set<std::string> options;
        if(i & 0b001u){
            options.insert("ambientMap");
        }
        if(i & 0b010u){
            options.insert("diffuseMap");
        }
        if(i & 0b100u){
            options.insert("specularMap");
        }
        phongShaders[i] = Application::getContext().getShader(
                File("virtual/zoe/materials/TemplatePhong.glsl"), options);
    }
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
        float dissolve = 1;
        bool dissolveDependentOnSurfaceOrientation{};
        float specularExponent = 32;
        float sharpness{};
        float opticalDensity{};
        std::string mapAmbientReflectivityName;
        std::string mapDiffuseReflectivityName;
        std::string mapSpecularReflectivityName;
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
        if (cmd.empty())
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
            throw std::runtime_error(file.getAbsolutePath()+" file is wrong formatted in line: " + std::to_string(lineNumber));
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
        } else if (cmd == "map_ka") {
            currentMaterial.mapAmbientReflectivityName = file.getParent().getAbsolutePath()+"/"+args;
        } else if (cmd == "map_kd") {
            currentMaterial.mapDiffuseReflectivityName = file.getParent().getAbsolutePath()+"/"+args;
        } else if (cmd == "map_ks") {
            currentMaterial.mapSpecularReflectivityName = file.getParent().getAbsolutePath()+"/"+args;
        } else {
            warning("Unknown or unsupported attribute \"", cmd, " ", args, "\" in file ", file.getPath());
        }
    }
    if (currentMaterial.init) {
        materials.push_back(currentMaterial);
    }

    for (MaterialData &data: materials) {
        unsigned int shaderOptionsIndex = 0;
        std::map<std::string, std::shared_ptr<Texture>> textureMap;
        if(Path ambientMap(currentMaterial.mapAmbientReflectivityName); ambientMap.isFile()){
            textureMap["map_ka"] = Application::getContext().getTexture(ambientMap);
            shaderOptionsIndex |= 0b001u;
        } else {
            //TODO: warning
        }
        if(Path diffuseMap(currentMaterial.mapDiffuseReflectivityName); diffuseMap.isFile()){
            textureMap["map_kd"] = Application::getContext().getTexture(diffuseMap);
            shaderOptionsIndex |= 0b010u;
        } else {
            //TODO: warning
        }
        if(Path specularMap(currentMaterial.mapSpecularReflectivityName); specularMap.isFile()){
            textureMap["map_ks"] = Application::getContext().getTexture(specularMap);
            shaderOptionsIndex |= 0b100u;
        } else {
            //TODO: warning
        }

        lib.materialMap->operator[](data.name) =
                Material(phongShaders[shaderOptionsIndex], textureMap,
                         [data, shaderOptionsIndex](Material *me, const Camera &camera, const mat4x4 &model) {
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
                                 shader.setUniform4m(tags.at("modelViewProjectionMatrix"),
                                                     model * camera.getViewMatrix() * camera.getProjectionMatrix());
                             }
                             if (!viewUniformIsSet && tags.count("viewMatrix")) {
                                 viewUniformIsSet = true;
                                 shader.setUniform4m(tags.at("viewMatrix"), camera.getViewMatrix());
                             }
                             if (!viewUniformIsSet && tags.count("viewProjectionMatrix")) {
                                 shader.setUniform4m(tags.at("viewProjectionMatrix"),
                                                     camera.getViewMatrix() * camera.getProjectionMatrix());
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
                             vec3 cameraPos = camera.getWorldPosition();
                             shader.setUniform3f("cameraPosition", cameraPos.x, cameraPos.y, cameraPos.z);

                             //TODO: source is enough documentation
                             shader.setUniform3f("lightPosition", cameraPos.x, cameraPos.y + 1, cameraPos.z);
                             shader.setUniform3f("lightColor", 1, 1, 1);

                             //TODO: changeable intensity
                             shader.setUniform1f("ambientIntensity", 0.2f);
                             shader.setUniform1f("diffuseIntensity", 1.0f);
                             shader.setUniform1f("specularIntensity", 1.0f);
                             for(const auto& entry: me->textures){
                                 if(entry.first == "map_ka"){
                                     shader.setTexture("ambientMap", *entry.second);
                                 } else if(entry.first == "map_kd"){
                                     shader.setTexture("diffuseMap", *entry.second);
                                 } else if(entry.first == "map_ks"){
                                     shader.setTexture("specularMap", *entry.second);
                                 }
                             }
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