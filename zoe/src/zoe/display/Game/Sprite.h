//
// Created by Florian on 10.07.2020.
//

#pragma once

#include "../Component.h"

namespace Zoe{

class Sprite: public BaseComponent{
public:

protected:
    void onDraw(const Camera& camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;
private:

};

typedef Sprite ComponentGroup;

}