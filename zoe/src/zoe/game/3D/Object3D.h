//
// Created by florian on 13.03.20.
//

#pragma once

#include "../../Core.h"
#include "../../render/Material.h"
#include "../../render/Camera.h"

namespace Zoe {

    class DLL_PUBLIC Object3D {
    public:

        virtual ~Object3D();

        virtual void draw(Camera &camera) = 0;
        virtual void tick();

    protected:
        Object3D(Material material);

        Object3D();

        inline void setMaterial(const Material &material) { this->material = material; }

        Material material;
    private:
    };

}



