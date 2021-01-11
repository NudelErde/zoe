//
// Created by Florian on 20.12.2020.
//

#pragma once

#include "../../math/vec.h"

namespace Zoe {

class AxisAlignedBox {
public:
    AxisAlignedBox() = default;
    AxisAlignedBox(const vec3& lower, const vec3& higher);
    AxisAlignedBox(float x1, float y1, float z1, float x2, float y2, float z2);

    [[nodiscard]] inline const vec3& getLowerCorner() const { return lower; }
    [[nodiscard]] inline const vec3& getHigherCorner() const { return higher; }

    [[nodiscard]] inline vec3 getDimension() const { return higher - lower; }

    [[nodiscard]] inline bool checkCollision(const AxisAlignedBox& other) const {
        return !(lower.x > other.higher.x || other.lower.x > higher.x
                 || lower.y > other.higher.y || other.lower.y > higher.y
                 || lower.z > other.higher.z || other.lower.z > higher.z);
    }

    [[nodiscard]] inline vec3 getCenter() const {
        return (lower + higher) / 2;
    }

private:
    vec3 lower;
    vec3 higher;
};

}
