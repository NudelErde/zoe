//
// Created by florian on 19.03.20.
//

#include "Model.h"

#include <utility>
#include <cstring>
#include "../core/Application.h"

namespace Zoe {

static vec4 defaultPosition = {0, 0, 0, 1};
static vec3 defaultNormal = {1, 0, 0};
static vec2 defaultTextureCoordinate = {0, 0};

class Vertex {
public:
    Vertex(vec4 position, vec2 texture, vec3 normal) : position(position), normal(normal),
                                                       textureCoordinate(texture) {}

    vec4 position;
    vec3 normal;
    vec2 textureCoordinate;

    bool operator==(const Vertex &rhs) const {
        return position == rhs.position &&
               normal == rhs.normal &&
               textureCoordinate == rhs.textureCoordinate;
    }

    bool operator!=(const Vertex &rhs) const {
        return !(rhs == *this);
    }
};

static Vertex
createVertex(const std::string &vertexString, const std::vector<vec4> &positions, const std::vector<vec3> &normals,
             const std::vector<vec2> &textureCoords) {
    int posIndex;
    int normIndex;
    int texIndex;
    size_t firstSlash;
    size_t secondSlash;

    firstSlash = vertexString.find('/');
    if (firstSlash == std::string::npos) {
        posIndex = std::stoi(vertexString);
        return Vertex(positions[(posIndex < 0) ? positions.size() + posIndex : posIndex - 1],
                      defaultTextureCoordinate,
                      defaultNormal);
    } else {
        posIndex = std::stoi(vertexString.substr(0, firstSlash));
        secondSlash = vertexString.find('/', firstSlash + 1);
        if (secondSlash == std::string::npos) {
            texIndex = std::stoi(vertexString.substr(firstSlash + 1, vertexString.length() - (firstSlash + 1)));
            return Vertex(
                    positions[(posIndex < 0) ? positions.size() + posIndex : posIndex - 1],
                    textureCoords[(texIndex < 0) ? textureCoords.size() + texIndex : texIndex - 1],
                    defaultNormal);
        } else if (firstSlash + 1 == secondSlash) {
            normIndex = std::stoi(vertexString.substr(firstSlash + 2, vertexString.length() - (firstSlash + 2)));
            return Vertex(
                    positions[(posIndex < 0) ? positions.size() + posIndex : posIndex - 1],
                    defaultTextureCoordinate,
                    normals[(normIndex < 0) ? normals.size() + normIndex : normIndex - 1]);
        } else {
            texIndex = std::stoi(vertexString.substr(firstSlash + 1, secondSlash - (firstSlash + 1)));
            normIndex = std::stoi(vertexString.substr(secondSlash + 1, vertexString.length() - (secondSlash + 1)));
            return Vertex(
                    positions[(posIndex < 0) ? positions.size() + posIndex : posIndex - 1],
                    textureCoords[(texIndex < 0) ? textureCoords.size() + texIndex : texIndex - 1],
                    normals[(normIndex < 0) ? normals.size() + normIndex : normIndex - 1]);
        }
    }
}

Model::Model() {
    vertexArray = nullptr;
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    modelMatrix = scale3D(1, 1, 1);
}

Model::Model(const File &file) {
    modelMatrix = scale3D(1, 1, 1);
    std::unique_ptr<std::istream> stream = file.createIStream();
    std::string line;
    float data[4];
    std::vector<vec4> positions;
    std::vector<vec3> normals;
    std::vector<vec2> textureCoords;
    std::stringstream sstream;
    int index;
    int element;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    while (getline(*stream, line)) {
        if (line[0] == '#') {
            continue;
        } else if (line[0] == 'v' && line[1] == ' ') {
            //create new vertex position
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 1;
            sstream = std::stringstream();
            for (element = 0, index = 2; index < line.length(); ++index) {
                if (line[index] == ' ') {
                    sstream >> data[element];
                    sstream = std::stringstream();
                    ++element;
                } else {
                    sstream << line[index];
                }
            }
            if (sstream.str().length() > 0) {
                sstream >> data[element];
                sstream = std::stringstream();
            }
            positions.push_back({data[0], data[1], data[2], data[3]});
        } else if (line[0] == 'v' && line[1] == 't') {
            //create new vertex texture coordinate
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 1;
            sstream = std::stringstream();
            for (element = 0, index = 3; index < line.length(); ++index) {
                if (line[index] == ' ') {
                    sstream >> data[element];
                    sstream = std::stringstream();
                    ++element;
                } else {
                    sstream << line[index];
                }
            }
            if (sstream.str().length() > 0) {
                sstream >> data[element];
                sstream = std::stringstream();
            }
            textureCoords.push_back({data[0], data[1]});
        } else if (line[0] == 'v' && line[1] == 'n') {
            //create new vertex normal
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 1;
            sstream = std::stringstream();
            for (element = 0, index = 3; index < line.length(); ++index) {
                if (line[index] == ' ') {
                    sstream >> data[element];
                    sstream = std::stringstream();
                    ++element;
                } else {
                    sstream << line[index];
                }
            }
            if (sstream.str().length() > 0) {
                sstream >> data[element];
                sstream = std::stringstream();
            }
            normals.push_back({data[0], data[1], data[2]});
        } else if (line[0] == 'f' && line[1] == ' ') {
            std::vector<std::string> elements;
            sstream = std::stringstream();
            element = 0;
            for (index = 2; index < line.length(); ++index) {
                if (line[index] == ' ') {
                    elements.push_back(sstream.str());
                    sstream.str(std::string()); //clear
                    ++element;
                } else {
                    sstream << line[index];
                }
            }
            if (sstream.str().length() > 0) {
                elements.push_back(sstream.str());
                sstream.str(std::string()); //clear
            }

            element = 0;
            unsigned int firstVertexIndex = 0;
            unsigned int lastVertexIndex = 0;
            for (const std::string &string: elements) {
                Vertex vertex = createVertex(string, positions, normals, textureCoords);
                size_t index = vertices.size();
                for (unsigned int i = 0; i < vertices.size(); ++i) {
                    if (vertices[i] == vertex) {
                        index = i;
                        break;
                    }
                }

                if (index == vertices.size()) {
                    vertices.push_back(vertex);
                }

                if (element == 0) {
                    firstVertexIndex = index;
                    ++element;
                } else if (element == 1) {
                    lastVertexIndex = index;
                    ++element;
                } else if (element == 2) {
                    indices.push_back(firstVertexIndex);
                    indices.push_back(lastVertexIndex);
                    indices.push_back(index);
                    lastVertexIndex = index;
                }
            }
        }
    }

    //create arrays
    const size_t verticesArrayLength = vertices.size() * 9;
    float *verticesArray = new float[verticesArrayLength];
    for (index = 0; index < vertices.size(); ++index) {
        verticesArray[index * 9 + 0] = vertices[index].position.x;
        verticesArray[index * 9 + 1] = vertices[index].position.y;
        verticesArray[index * 9 + 2] = vertices[index].position.z;
        verticesArray[index * 9 + 3] = vertices[index].position.w;
        verticesArray[index * 9 + 4] = vertices[index].normal.x;
        verticesArray[index * 9 + 5] = vertices[index].normal.y;
        verticesArray[index * 9 + 6] = vertices[index].normal.z;
        verticesArray[index * 9 + 7] = vertices[index].textureCoordinate.x;
        verticesArray[index * 9 + 8] = vertices[index].textureCoordinate.y;
    }
    unsigned int *indicesArray = new unsigned int[indices.size()];
    std::copy(indices.begin(), indices.end(), indicesArray);

    vertexBuffer = Application::getContext().getVertexBuffer(false);
    indexBuffer = Application::getContext().getIndexBuffer(false);
    vertexBuffer->setData(verticesArray, vertices.size() * 9 * sizeof(float));
    indexBuffer->setData(indicesArray, indices.size());

    indexBuffer->getCount();
    delete[] verticesArray;
    delete[] indicesArray;
    this->vertexArray = Application::getContext().getVertexArray();
    std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
    layout->push_float(4);
    layout->push_float(3);
    layout->push_float(2);
    vertexArray->set(vertexBuffer, indexBuffer, layout);
    modelMatrix = translate3D(0, 0, 0);
}

Model::Model(void *vertices, unsigned int *indices, unsigned int verticesSize, unsigned int indicesCount,
             const std::shared_ptr<VertexBufferLayout> &layout) {
    vertexBuffer = Application::getContext().getVertexBuffer(false);
    indexBuffer = Application::getContext().getIndexBuffer(false);
    vertexBuffer->setData(vertices, verticesSize);
    indexBuffer->setData(indices, indicesCount);
    this->vertexArray = Application::getContext().getVertexArray();
    vertexArray->set(vertexBuffer, indexBuffer, layout);
    modelMatrix = translate3D(0, 0, 0);
}

Model::Model(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer,
             const std::shared_ptr<VertexBufferLayout> &layout) {
    this->vertexBuffer = vertexBuffer;
    this->indexBuffer = indexBuffer;
    this->vertexArray = Application::getContext().getVertexArray();
    this->vertexArray->set(vertexBuffer, indexBuffer, layout);
    modelMatrix = translate3D(0, 0, 0);
}

Model::Model(std::shared_ptr<VertexArray> vertexArray) {
    this->vertexArray = std::move(vertexArray);
    modelMatrix = translate3D(0, 0, 0);
}

Model &Model::operator=(Model && other) noexcept {
    this->modelMatrix = other.modelMatrix;
    this->vertexArray = std::move(other.vertexArray);
    this->indexBuffer = std::move(other.indexBuffer);
    this->vertexBuffer = std::move(other.vertexBuffer);
    return *this;
}

Model::Model(Model &&other) noexcept {
    this->modelMatrix = other.modelMatrix;
    this->vertexArray = std::move(other.vertexArray);
    this->indexBuffer = std::move(other.indexBuffer);
    this->vertexBuffer = std::move(other.vertexBuffer);
}

Model &Model::operator=(const Model & other) {
    if(&other == this){
        return *this;
    }
    this->modelMatrix = other.modelMatrix;
    this->vertexBuffer = Application::getContext().getVertexBuffer(other.vertexBuffer->isDynamic());
    this->indexBuffer = Application::getContext().getIndexBuffer(other.indexBuffer->isDynamic());
    {
        unsigned int size = other.vertexBuffer->getSize();
        char *buffer = static_cast<char *>(other.vertexBuffer->getData(0, size));
        vertexBuffer->setData(buffer, size);
        delete[] buffer;
    }
    {
        unsigned int count = other.indexBuffer->getCount();
        unsigned int* buffer = other.indexBuffer->getData(0,count);
        indexBuffer->setData(buffer, count);
        delete[] buffer;
    }
    vertexArray = Application::getContext().getVertexArray();
    vertexArray->set(vertexBuffer, indexBuffer, other.vertexArray->getVertexBufferLayout());
    return *this;
}

Model::Model(const Model &other) {
    this->modelMatrix = other.modelMatrix;
    this->vertexBuffer = Application::getContext().getVertexBuffer(other.vertexBuffer->isDynamic());
    this->indexBuffer = Application::getContext().getIndexBuffer(other.indexBuffer->isDynamic());
    {
        unsigned int size = other.vertexBuffer->getSize();
        char *buffer = static_cast<char *>(other.vertexBuffer->getData(0, size));
        vertexBuffer->setData(buffer, size);
        delete[] buffer;
    }
    {
        unsigned int count = other.indexBuffer->getCount();
        unsigned int* buffer = other.indexBuffer->getData(0,count);
        indexBuffer->setData(buffer, count);
        delete[] buffer;
    }
    vertexArray = Application::getContext().getVertexArray();
    vertexArray->set(vertexBuffer, indexBuffer, other.vertexArray->getVertexBufferLayout());
}

Model::~Model() = default;
}