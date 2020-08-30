//
// Created by Florian on 29.08.2020.
//

#pragma once

#include "../core/Core.h"
#include "Component.h"

namespace Zoe {

struct ChaiScriptInstance;

class DLL_PUBLIC ChaiScriptComponent : public BaseComponent {
public:
    ChaiScriptComponent();
    ChaiScriptComponent(ChaiScriptComponent&&) noexcept;
    ChaiScriptComponent& operator=(ChaiScriptComponent&&) noexcept;
    ChaiScriptComponent(const ChaiScriptComponent&) = delete;
    ChaiScriptComponent& operator=(const ChaiScriptComponent&) = delete;
    ~ChaiScriptComponent();
protected:
    void onDraw(const Camera& camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;
private:
    std::unique_ptr<ChaiScriptInstance> script;
};

}