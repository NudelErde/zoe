//
// Created by Florian on 06.08.2020.
//

#include "Model.h"
#include "../core/Application.h"
#include "api/VertexBufferLayout.h"
#include "api/VertexArray.h"
#include <memory>
#include <utility>

namespace Zoe {

Model::Model(Material  material, const std::shared_ptr<VertexBuffer> & vbo, const std::shared_ptr<IndexBuffer> & ibo): material(std::move(material)) {
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

WavefrontFile WavefrontFile::parseWavefrontFile(const File & file, bool forceReload) {
    if(!forceReload && loadedWavefrontFiles->count(file.getPath())){
        return loadedWavefrontFiles->at(file.getPath());
    }
    WavefrontFile wff;
    //TODO: parse WavefrontFile
    return wff;
}

const Model &WavefrontFile::get(const std::string & name) {
    return modelMap->at(name);
}

bool WavefrontFile::hasModel(const std::string & name) {
    return modelMap->count(name);
}

WavefrontFile::WavefrontFile(WavefrontFile && other) noexcept {
    modelMap = other.modelMap;;
    other.modelMap = nullptr;
}

WavefrontFile::WavefrontFile(const WavefrontFile & other) {
    modelMap = new std::map<std::string, Model>(*other.modelMap);
}

WavefrontFile &WavefrontFile::operator=(WavefrontFile && other) noexcept {
    delete modelMap;
    modelMap = other.modelMap;
    other.modelMap = nullptr;
    return *this;
}

WavefrontFile &WavefrontFile::operator=(const WavefrontFile & other) {
    if(this == &other)
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