//
// Created by Florian on 20.08.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../Component.h"
#include "../../render/Model.h"

namespace Zoe {

class DLL_PUBLIC ModelComponent: public BaseComponent{
public:
    ModelComponent();

protected:
    void onDraw(const Camera& camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;

public:
    inline const Model &getModel() const {
        return model;
    }

    inline void setModel(const Model &model) {
        ModelComponent::model = model;
    }

private:
    Model model;
};

}
