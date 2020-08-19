//
// Created by Florian on 06.08.2020.
//

#pragma once

#include <memory>

#include "../core/Core.h"
#include "Material.h"

namespace Zoe {

class VertexBuffer;

class IndexBuffer;

class VertexArray;

class Model;

class DLL_PUBLIC WavefrontFile {
public:

    //parse .obj file and used .mtl files
    static WavefrontFile parseWavefrontFile(const File &, bool forceReload = false);

    const Model &get(const std::string &) const;

    bool hasModel(const std::string &) const;

    WavefrontFile(WavefrontFile &&) noexcept;

    WavefrontFile(const WavefrontFile &);

    WavefrontFile &operator=(WavefrontFile &&) noexcept;

    WavefrontFile &operator=(const WavefrontFile &);

    ~WavefrontFile();

    WavefrontFile();

private:

    //need ptr because vs is buggy and doesn't like if map is in static storage
    std::map<std::string, Model> *modelMap;
};

class DLL_PUBLIC Model {
public:
    Model(Material, const std::shared_ptr<VertexBuffer> &, const std::shared_ptr<IndexBuffer> &);

    Model();

    inline const Material &getMaterial() const {
        return material;
    }

    inline Material &getMaterial() {
        return material;
    }

    inline void setMaterial(const Material &material) {
        Model::material = material;
    }

    inline const mat4x4 &getModelMatrix() const {
        return modelMatrix;
    }

    inline void setModelMatrix(const mat4x4 &modelMatrix) {
        Model::modelMatrix = modelMatrix;
    }

    inline const std::shared_ptr<VertexArray>& getVertexArray() {
        return mesh.vertexArray;
    }

private:
    Material material;
    struct {
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexArray> vertexArray;
    } mesh;
    mat4x4 modelMatrix;
};

}