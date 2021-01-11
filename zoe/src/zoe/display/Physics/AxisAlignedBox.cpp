//
// Created by Florian on 20.12.2020.
//

#include "AxisAlignedBox.h"
#include "cmath"

namespace Zoe{

AxisAlignedBox::AxisAlignedBox(const vec3& lower, const vec3& higher) {
    AxisAlignedBox::lower = vec3(std::min(lower.x, higher.x), std::min(lower.y, higher.y), std::min(lower.z, higher.z));
    AxisAlignedBox::higher = vec3(std::max(lower.x, higher.x), std::max(lower.y, higher.y), std::max(lower.z, higher.z));
}

AxisAlignedBox::AxisAlignedBox(float x1, float y1, float z1, float x2, float y2, float z2) {
    AxisAlignedBox::lower = vec3(std::min(x1, x2), std::min(y1, y2), std::min(z1, z2));
    AxisAlignedBox::higher = vec3(std::max(x1, x2), std::max(y1, y2), std::max(z1, z2));
}

}