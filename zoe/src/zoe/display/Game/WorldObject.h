//
// Created by Florian on 11.07.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Model.h"

namespace Zoe{

class DLL_PUBLIC WorldObject: public BaseComponent{
public:
    WorldObject();

protected:
    void onDraw(const Camera& camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;

public:
    /*inline const Model &getModel() const {return model;}

    inline void setModel(const Model &model) {WorldObject::model = model;}*/

private:
    /*Model model;*/

    //std::weak_ptr<TextureComponent> texture;
    //std::weak_ptr<TextureComponent> normalTexture;
    //std::weak_ptr<MaterialComponent> material;

    //TODO: default material
};

}