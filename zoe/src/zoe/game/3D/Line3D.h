//
// Created by florian on 19.03.20.
//

#pragma once

#include "Object3D.h"

namespace Zoe{

    class DLL_PUBLIC Line3D: public Object3D {
    public:
        Line3D(const vec3& start, const vec3& end);
        ~Line3D();

        void draw(Camera& camera) override;
        void setPosition(const vec3& startPosition, const vec3& endPosition);
        void setColor(const vec4& start, const vec4& end);
    private:
        vec3 start;
        vec3 end;
        vec4 colorStart;
        vec4 colorEnd;

        std::shared_ptr<VertexBuffer> vertexBuffer;

        Model lineModel;
    };

}

