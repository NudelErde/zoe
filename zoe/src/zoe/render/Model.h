//
// Created by florian on 19.03.20.
//

#pragma once

#include "../core/Core.h"
#include "../core/File.h"
#include "../math/mat.h"
#include "api/VertexArray.h"

namespace Zoe{

    class DLL_PUBLIC Model {
    public:
        Model();
        explicit Model(const File& file);
        Model(void* vertices, unsigned int* indices, unsigned int verticesSize, unsigned int indicesCount, const std::shared_ptr<VertexBufferLayout>& layout);
        Model(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer, const std::shared_ptr<VertexBufferLayout>& layout);
        explicit Model(std::shared_ptr<VertexArray> vertexArray);
        ~Model();

        Model(const Model& model);
        Model(Model&& model) noexcept;
        Model& operator=(const Model&);
        Model& operator=(Model&&) noexcept;

        inline const mat4x4& getModelMatrix() const {return modelMatrix;}
        inline void setModelMatrix(const mat4x4& matrix){modelMatrix = matrix;}
        inline const std::shared_ptr<VertexArray>& getVertexArray() const {return vertexArray;}

    private:

        mat4x4 modelMatrix;

        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
    };

}



