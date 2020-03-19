//
// Created by florian on 19.03.20.
//

#include "Model.h"

#include <utility>
#include "../Application.h"

namespace Zoe {


    Model::Model() {
        vertexArray = nullptr;
    }

    Model::Model(const File &file) {
        std::shared_ptr<std::istream> stream = file.getInputStream();
        std::string line;
        float data[4];
        std::vector<vec4> positions;
        std::vector<vec3> normals;
        std::vector<vec2> textureCoords;
        std::stringstream sstream;
        int index;
        int element;
        float value;
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
                for(element = 0, index = 2; index < line.length(); ++index){
                    if(line[index] == ' '){
                        sstream >> data[element];
                        sstream = std::stringstream();
                        ++element;
                    }else{
                        sstream << line[index];
                    }
                }
                positions.push_back({data[0],data[1],data[2],data[3]});
            } else if (line[0] == 'v' && line[1] == 't') {
                //create new vertex texture coordinate
                data[0] = 0;
                data[1] = 0;
                data[2] = 0;
                data[3] = 1;
                sstream = std::stringstream();
                for(element = 0, index = 3; index < line.length(); ++index){
                    if(line[index] == ' '){
                        sstream >> data[element];
                        sstream = std::stringstream();
                        ++element;
                    }else{
                        sstream << line[index];
                    }
                }
                textureCoords.push_back({data[0],data[1]});
            } else if (line[0] == 'v' && line[1] == 'n') {
                //create new vertex normal
                data[0] = 0;
                data[1] = 0;
                data[2] = 0;
                data[3] = 1;
                sstream = std::stringstream();
                for(element = 0, index = 3; index < line.length(); ++index){
                    if(line[index] == ' '){
                        sstream >> data[element];
                        sstream = std::stringstream();
                        ++element;
                    }else{
                        sstream << line[index];
                    }
                }
                normals.push_back({data[0],data[1],data[2]});
            } else if (line[0] == 'f' && line[1] == ' ') {
                //create new face
                throw std::runtime_error("not implemented! could not create face");
            }
        }

        modelMatrix = translate3D(0,0,0);
    }

    Model::Model(void *vertices, unsigned int *indices, unsigned int verticesSize, unsigned int indicesCount,
                 const std::shared_ptr<VertexBufferLayout>& layout) {
        std::shared_ptr<VertexBuffer> vertexBuffer = Application::getContext().getVertexBuffer(false);
        std::shared_ptr<IndexBuffer> indexBuffer = Application::getContext().getIndexBuffer(false);
        vertexBuffer->setData(vertices, verticesSize);
        indexBuffer->setData(indices, indicesCount);
        this->vertexArray = Application::getContext().getVertexArray();
        vertexArray->set(*vertexBuffer, *indexBuffer, *layout);
        modelMatrix = translate3D(0,0,0);
    }

    Model::Model(const std::shared_ptr<VertexBuffer> &vertexBuffer, const std::shared_ptr<IndexBuffer> &indexBuffer,
                 const std::shared_ptr<VertexBufferLayout> &layout) {
        this->vertexArray = Application::getContext().getVertexArray();
        this->vertexArray->set(*vertexBuffer, *indexBuffer, *layout);
        modelMatrix = translate3D(0,0,0);
    }

    Model::Model(std::shared_ptr<VertexArray> vertexArray) {
        this->vertexArray = std::move(vertexArray);
        modelMatrix = translate3D(0,0,0);
    }

    Model::~Model() = default;
}