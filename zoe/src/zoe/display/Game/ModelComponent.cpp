//
// Created by Florian on 20.08.2020.
//

#include "ModelComponent.h"

namespace Zoe {

ModelComponent::ModelComponent() = default;

void ModelComponent::onDraw(const Camera &camera) {}

void ModelComponent::onUpdate(double time) {}

void ModelComponent::onInputEvent(Event &event) {}

void ModelComponent::fill(const XMLNode &node) {
    std::string name = "default";
    if (node.attributes.count("name")) {
        name = node.attributes.at("name");
    }
    if (node.attributes.count("src")) {
        Path path(node.attributes.at("src"));
        if (path.isFile()) {
            const auto &wavefrontFile = WavefrontFile::parseWavefrontFile(path, false);
            if (wavefrontFile.hasModel(name)) {
                model = wavefrontFile.get(name);
            } else {
                warning("File ", path.getAbsolutePath(), " does not contain a model with name ", name);
            }
        } else {
            warning("Could not load WavefrontFile, because ", path.getAbsolutePath(), " is not a File");
        }
    }
}

void ModelComponent::postFill() {}

}