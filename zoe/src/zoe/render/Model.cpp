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
#include "Material.h"

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

Model::Model() = default;

static std::shared_ptr<std::map<std::string, WavefrontFile>> loadedWavefrontFiles = std::make_shared<std::map<std::string, WavefrontFile>>();

WavefrontFile WavefrontFile::parseWavefrontFile(const File &file, bool forceReload) {
    if (!forceReload && loadedWavefrontFiles->count(file.getAbsolutePath())) {
        return loadedWavefrontFiles->at(file.getAbsolutePath());
    }
    WavefrontFile wff;
    std::unique_ptr<std::istream> stream = file.createIStream(false);
    std::istream &is = *stream;
    std::string line;
    int lineNumber = 0;

    std::vector<MaterialLibrary> materialLibraries;
    struct Vertex {
        vec4 pos;
        vec3 norm;
        vec2 tex;

        bool operator==(const Vertex &rhs) const {
            return this->pos == rhs.pos && this->norm == rhs.norm && this->tex == rhs.tex;
        }
    };
    struct Face {
        std::array<unsigned int, 3> positionIndex;
        std::array<unsigned int, 3> normalsIndex;
        std::array<unsigned int, 3> texPositionsIndex;
    };
    struct Object {
        std::string name;
        std::vector<Face> faces;
        Material material;
    };
    std::vector<vec4> positions;
    std::vector<vec3> normals;
    std::vector<vec2> texPositions;
    std::vector<Object> objects;
    Object currentObject{"default"};
    while (getline(is, line)) {
        ++lineNumber;
        trim(line);
        if (line.empty() || line[0] == '#')
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
            Path path(file.getParent().getPath() + "/" + splitLine[1]);
            if (!path.isFile()) {
                warning("Material library ", path.getPath(), " could not be found");
            } else {
                materialLibraries.push_back(MaterialLibrary::parseMaterialLibrary(path, false));
            }
        } else if (splitLine[0] == "o") {
            if (!currentObject.faces.empty()) {
                objects.push_back(currentObject);
            }
            currentObject = {splitLine[1]};
        } else if (splitLine[0] == "usemtl") {
            for (const auto &materialLibrary: materialLibraries) {
                if (materialLibrary.hasLibrary(splitLine[1])) {
                    currentObject.material = materialLibrary.get(splitLine[1]);
                    break;
                }
            }
        } else if (splitLine[0] == "f") {
            Face face{};
            for (int i = 0; i < 3; ++i) {
                std::vector<std::string> values = split(splitLine[i + 1], '/');
                face.positionIndex[i] = fromString<int>(values[0]);
                if (face.positionIndex[i] > 0) {
                    --face.positionIndex[i];
                } else if (face.positionIndex[i] < 0) {
                    face.positionIndex[i] = (unsigned int) (positions.size() - face.positionIndex[i]);
                }
                face.texPositionsIndex[i] = (values.size() < 2) ? 0 : fromString<int>(values[1]);
                if (face.texPositionsIndex[i] > 0) {
                    --face.texPositionsIndex[i];
                } else if (face.texPositionsIndex[i] < 0) {
                    face.texPositionsIndex[i] = (unsigned int) (texPositions.size() - face.texPositionsIndex[i]);
                }
                face.normalsIndex[i] = (values.size() < 3 ? 0 : fromString<int>(values[2]));
                if (face.normalsIndex[i] > 0) {
                    --face.normalsIndex[i];
                } else if (face.normalsIndex[i] < 0) {
                    face.normalsIndex[i] = (unsigned int) (normals.size() - face.normalsIndex[i]);
                }
            }
            currentObject.faces.push_back(face);
        } else if (splitLine[0] == "g") {
            info("Wavefront groups are not yet supported");
        } else if (splitLine[0] == "s") {
            if (splitLine[1] != "off") {
                info("Wavefront smooth shading is not yet supported");
            }
        } else {
            warning("Unknown or unsupported attribute \"", line, "\" in file ", file.getPath(), ":", lineNumber);
        }
    }
    //TODO: objects and other std::vectors => Model
    for (const auto &object: objects) {
        std::vector<unsigned int> indexData;
        std::vector<Vertex> vertexVector;
        for (const auto &face: object.faces) {
            for (int i = 0; i < 3; ++i) {
                Vertex v{};
                v.pos = positions[face.positionIndex[i]];
                v.norm = normals[face.normalsIndex[i]];
                v.tex = texPositions[face.texPositionsIndex[i]];
                unsigned int index = (unsigned int)vertexVector.size();
                for (unsigned int j = 0; j < vertexVector.size(); ++j) {
                    if (vertexVector[j] == v) {
                        index = j;
                        break;
                    }
                }
                if (index == vertexVector.size()) {
                    vertexVector.push_back(v);
                }
                indexData.push_back(index);
            }
        }

        auto *vertexData = new float[vertexVector.size() * 9];

        for (unsigned int index = 0; index < vertexVector.size(); ++index) {
            const auto &vertex = vertexVector[index];
            vertexData[index * 9 + 0] = vertex.pos.x;
            vertexData[index * 9 + 1] = vertex.pos.y;
            vertexData[index * 9 + 2] = vertex.pos.z;
            vertexData[index * 9 + 3] = vertex.pos.w;
            vertexData[index * 9 + 4] = vertex.norm.x;
            vertexData[index * 9 + 5] = vertex.norm.y;
            vertexData[index * 9 + 6] = vertex.norm.z;
            vertexData[index * 9 + 7] = vertex.tex.x;
            vertexData[index * 9 + 8] = vertex.tex.y;
        }
        std::shared_ptr<VertexBuffer> vertexBuffer = Application::getContext().getVertexBuffer(false);
        vertexBuffer->setData(vertexData, (unsigned int)(vertexVector.size() * 9 * sizeof(float)));
        delete[] vertexData;

        std::shared_ptr<IndexBuffer> indexBuffer = Application::getContext().getIndexBuffer(false);
        indexBuffer->setData(indexData.data(), (unsigned int)indexData.size());

        wff.modelMap->operator[](object.name) = Model(object.material, vertexBuffer, indexBuffer);
    }
    loadedWavefrontFiles->operator[](file.getAbsolutePath()) = wff;
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