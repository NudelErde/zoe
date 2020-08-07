//
// Created by Florian on 06.08.2020.
//

#pragma once

#include <memory>

#include "../core/Core.h"
#include "Material.h"

namespace Zoe{

class VertexBuffer;
class IndexBuffer;
class VertexArray;

class Model;

class DLL_PUBLIC WavefrontFile{
public:

    //parse .obj file and used .mtl files
    static WavefrontFile parseWavefrontFile(const File&, bool forceReload = false);

    const Model& get(const std::string&);
    bool hasModel(const std::string&);

    WavefrontFile(WavefrontFile&&) noexcept;
    WavefrontFile(const WavefrontFile&);

    WavefrontFile& operator=(WavefrontFile&&) noexcept;
    WavefrontFile& operator=(const WavefrontFile&);

    ~WavefrontFile();

private:
    WavefrontFile();

    //need ptr because vs is buggy and doesn't like if map is in static storage
    std::map<std::string, Model>* modelMap;
};

class DLL_PUBLIC Model {
public:
    Model(Material , const std::shared_ptr<VertexBuffer>&, const std::shared_ptr<IndexBuffer>&);

private:
    Material material;
    struct{
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexArray> vertexArray;
    } mesh;
};

}