//
// Created by Florian on 06.08.2020.
//

#include "Model.h"
#include "../core/Application.h"
#include "api/VertexBufferLayout.h"
#include "api/VertexArray.h"
#include <memory>
#include <utility>
#include "../core/String.h"

namespace Zoe {

Model::Model(Material material, const std::shared_ptr<VertexBuffer> &vbo, const std::shared_ptr<IndexBuffer> &ibo)
        : material(std::move(material)) {
    mesh.vertexBuffer = vbo;
    mesh.indexBuffer = ibo;
    mesh.vertexArray = Application::getContext().getVertexArray();
    std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
    layout->push_float(4);
    layout->push_float(3);
    layout->push_float(2);
    mesh.vertexArray->set(vbo, ibo, layout);
}

static std::shared_ptr<std::map<std::string, WavefrontFile>> loadedWavefrontFiles;

WavefrontFile WavefrontFile::parseWavefrontFile(const File &file, bool forceReload) {
    if (!forceReload && loadedWavefrontFiles->count(file.getPath())) {
        return loadedWavefrontFiles->at(file.getPath());
    }
    WavefrontFile wff;
    //TODO: parse WavefrontFile
    std::unique_ptr<std::istream> stream = file.createIStream(false);
    std::istream &is = *stream;
    std::string line;
    int lineNumber = 0;
    std::vector<MaterialLibrary> materialLibraries;
    struct Face{
        unsigned int positionIndex;
        unsigned int normalsIndex;
        unsigned int texPositionsIndex;
    };
    struct Object{
        std::vector<Face> faces;
        Material material;
    };
    std::vector<vec4> positions;
    std::vector<vec3> normals;
    std::vector<vec2> texPositions;
    std::vector<Object> objects;
    while (getline(is, line)) {
        ++lineNumber;
        trim(line);
        if(line.empty() || line[0] == '#')
            continue;

        std::vector<std::string> splitLine = split(line, ' ');
        trim(splitLine[0]);
        if (splitLine[0] == "v") {
            positions.push_back(vec4({fromString<float>(splitLine[1]),
                                      fromString<float>(splitLine[2]),
                                      fromString<float>(splitLine[3]),
                                      splitLine.size() > 4 ? fromString<float>(splitLine[4]) : 1.0f}));
        } else if (splitLine[0] == "vn") {
            normals.push_back(vec3({fromString<float>(splitLine[1]),
                                    fromString<float>(splitLine[2]),
                                    fromString<float>(splitLine[3])}).normalize());
        } else if (splitLine[0] == "vt") {
            positions.push_back(vec4({fromString<float>(splitLine[1]),
                                      fromString<float>(splitLine[2])}));
        } else if (splitLine[0] == "mtllib") {
            //file.getDirectory();
        } else {
            warning("Unknown or unsupported attribute in ",file.getPath(),":",lineNumber,"\"",line,"\"");
        }//TODO: den rest
    }
    //TODO: objects and other std::vectors => Model

    return wff;
}

const Model &WavefrontFile::get(const std::string &name) {
    return modelMap->at(name);
}

bool WavefrontFile::hasModel(const std::string &name) {
    return modelMap->count(name);
}

WavefrontFile::WavefrontFile(WavefrontFile &&other) noexcept {
    modelMap = other.modelMap;;
    other.modelMap = nullptr;
}

WavefrontFile::WavefrontFile(const WavefrontFile &other) {
    modelMap = new std::map<std::string, Model>(*other.modelMap);
}

WavefrontFile &WavefrontFile::operator=(WavefrontFile &&other) noexcept {
    delete modelMap;
    modelMap = other.modelMap;
    other.modelMap = nullptr;
    return *this;
}

WavefrontFile &WavefrontFile::operator=(const WavefrontFile &other) {
    if (this == &other)
        return *this;
    delete modelMap;
    modelMap = new std::map<std::string, Model>(*other.modelMap);
    return *this;
}

WavefrontFile::~WavefrontFile() {
    delete modelMap;
}

WavefrontFile::WavefrontFile() {
    modelMap = new std::map<std::string, Model>();
}
}