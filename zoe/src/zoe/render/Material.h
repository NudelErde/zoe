//
// Created by Florian on 05.08.2020.
//

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <map>

#include "../core/Core.h"
#include "../math/mat.h"
#include "../core/File.h"
#include "../display/Camera.h"

namespace Zoe {

class Shader;

class Texture;

class Material;

/**
 * A MaterialLibrary is loaded from a .mtl file. It contains multiple Materials.
 */
class DLL_PUBLIC MaterialLibrary {
public:
    /**
     * Loads the material library referenced by the specified file. It is only parsed on the first load or when forceReload is true.
     * @param file the .mtl file
     * @param forceReload forces a reload
     * @return the MaterialLibrary
     */
    static MaterialLibrary parseMaterialLibrary(const File &file, bool forceReload = false);

    /**
     * Get a Material by the specified name.
     * @param name the specified name
     * @return the Material with the name
     */
    [[nodiscard]] const Material &get(const std::string & name) const;

    /**
     * Checks if a Material with the specified name exists in this material library.
     * @param name the specified name
     * @return `true` if the Material exists
     */
    [[nodiscard]] bool hasLibrary(const std::string & name) const;

    /**
     * Defines a move constructor.
     */
    MaterialLibrary(MaterialLibrary &&) noexcept;

    /**
     * Defines a copy constructor.
     */
    MaterialLibrary(const MaterialLibrary &);

    /**
     * Defines a move assign.
     * @return a reference to this
     */
    MaterialLibrary &operator=(MaterialLibrary &&) noexcept;

    /**
     * Defines a copy assign.
     * @return a reference to this
     */
    MaterialLibrary &operator=(const MaterialLibrary &);

    /**
     * Destructs the material library
     */
    ~MaterialLibrary();

    /**
     * Constructs an empty MaterialLibrary.
     */
    MaterialLibrary();

private:
    //need ptr because vs is buggy and doesn't like if map is in static storage
    std::map<std::string, Material> *materialMap;
};

/**
 * A material is used to define how the mesh in the model is rendered.
 */
class DLL_PUBLIC Material {
public:
    /**
     * Create an empty Material
     */
    Material();

    /**
     * Create a Material from a shader, textures and an on load function
     */
    Material(const std::shared_ptr<Shader> &, const std::map<std::string, std::shared_ptr<Texture>> &,
             const std::function<void(Material *, const Camera &, const mat4x4 &)> &);

    /**
     * Bind this material with the specified camera and model.
     * @param camera the specified camera
     * @param model the specified model
     */
    void bind(const Camera &camera, const mat4x4 &model);

    /**
     * Return the used shader.
     * @return the shader
     */
    [[nodiscard]] inline const std::shared_ptr<Shader> &getShader() const {
        return shader;
    }

    /**
     * Return the stored textures.
     * @return the textures
     */
    [[nodiscard]] inline const std::map<std::string, std::shared_ptr<Texture>> &getTextures() const {
        return textures;
    }

private:
    friend class MaterialLibrary;

    std::shared_ptr<Shader> shader;
    std::map<std::string, std::shared_ptr<Texture>> textures;
    std::function<void(Material *, const Camera &, const mat4x4 &modelMatrix)> bindingFunction;
};

}
