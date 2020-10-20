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

/**
 * A WavefrontFile contains models and loads it used .mtl files
 */
class DLL_PUBLIC WavefrontFile {
public:

    //parse .obj file and used .mtl files
    /**
     * Loads the WavefrontFile referenced by the specified file. It is only parsed on the first load or when forceReload is true.
     * @param file the .obj file
     * @param forceReload forces a reload
     * @return the WavefrontFile
     */
    static WavefrontFile parseWavefrontFile(const File& file, bool forceReload = false);

    /**
     * Get a Model by the specified name.
     * @param name the specified name
     * @return the Model with the name
     */
    [[nodiscard]] const Model& get(const std::string& name) const;

    /**
     * Checks if a Model with the specified name exists in this WavefrontFile.
     * @param name the specified name
     * @return `true` if the Material exists
     */
    [[nodiscard]] bool hasModel(const std::string& name) const;

    /**
     * Defines a move constructor.
     */
    WavefrontFile(WavefrontFile&&) noexcept;

    /**
     * Defines a copy constructor.
     */
    WavefrontFile(const WavefrontFile&);

    /**
     * Defines a move assign.
     * @return a reference to this
     */
    WavefrontFile& operator=(WavefrontFile&&) noexcept;

    /**
     * Defines a copy assign.
     * @return a reference to this
     */
    WavefrontFile& operator=(const WavefrontFile&);

    /**
     * Destructs the wavefront file
     */
    ~WavefrontFile();

    /**
     * Constructs an empty WavefrontFile.
     */
    WavefrontFile();

private:

    //need ptr because vs is buggy and doesn't like if map is in static storage
    std::map<std::string, Model>* modelMap;
};

/**
 * A Model contains a mesh, a material and the model matrix.
 */
class DLL_PUBLIC Model {
public:

    /**
     * Constructs a Model with the specified material, the specified vertex buffer and the specified index buffer.
     * The model matrix is initialized with the identity matrix.
     * @param material the specified material
     * @param vertexBuffer the specified vertex buffer
     * @param indexBuffer the specified index buffer
     */
    Model(Material material, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);

    /**
     * Constructs an empty Model.
     */
    Model();

    /**
     * Returns the used material.
     * @return the used material
     */
    [[nodiscard]] inline const Material& getMaterial() const {
        return material;
    }

    /**
     * Returns the used material.
     * @return the used material
     */
    inline Material& getMaterial() {
        return material;
    }

    /**
     * Sets the used material.
     * @param material the used material
     */
    inline void setMaterial(const Material& material) {
        Model::material = material;
    }

    /**
     * Returns the used model matrix.
     * @return the used model matrix
     */
    [[nodiscard]] inline const mat4x4& getModelMatrix() const {
        return modelMatrix;
    }

    /**
     * Sets the used model matrix.
     * @param modelMatrix the used model matrix
     */
    inline void setModelMatrix(const mat4x4& modelMatrix) {
        Model::modelMatrix = modelMatrix;
    }

    /**
     * Returns the used vertex array.
     * @return the used vertex array
     */
    [[nodiscard]] inline const std::shared_ptr<VertexArray>& getVertexArray() const {
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