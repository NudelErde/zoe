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

        Object3D(Material material, Model model);

        virtual ~Object3D();

        virtual void draw(Camera &camera);

        virtual void tick();

        inline Material &getMaterial() { return material; }

        inline Model &getModel() { return model; }

        inline const Material &getMaterial() const { return material; }

        inline const Model &getModel() const { return model; }

    protected:

        Object3D();

        inline void setMaterial(const Material &material) { this->material = material; }

        inline void setModel(const Model &model) { this->model = model; }

        Material material;
        Model model;
    private:
    };

}



