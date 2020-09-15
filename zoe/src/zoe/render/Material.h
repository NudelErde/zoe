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

class DLL_PUBLIC MaterialLibrary {
public:
    //parse .mtl file and load attached images
    static MaterialLibrary parseMaterialLibrary(const File &file, bool forceReload = false);

    [[nodiscard]] const Material &get(const std::string &) const;

    [[nodiscard]] bool hasLibrary(const std::string &) const;

    MaterialLibrary(MaterialLibrary &&) noexcept;

    MaterialLibrary(const MaterialLibrary &);

    MaterialLibrary &operator=(MaterialLibrary &&) noexcept;

    MaterialLibrary &operator=(const MaterialLibrary &);

    ~MaterialLibrary();

    MaterialLibrary();

private:
    //need ptr because vs is buggy and doesn't like if map is in static storage
    std::map<std::string, Material> *materialMap;
};

class DLL_PUBLIC Material {
public:
    Material();

    Material(const std::shared_ptr<Shader> &, const std::map<std::string, std::shared_ptr<Texture>> &,
             const std::function<void(Material *, const Camera &, const mat4x4 &)> &);

    void bind(const Camera &camera, const mat4x4 &model);

    [[nodiscard]] inline const std::shared_ptr<Shader> &getShader() const {
        return shader;
    }

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
