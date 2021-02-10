//
// Created by Florian on 20.12.2020.
//

#pragma once

#include "../../math/vec.h"

namespace Zoe {

/**
 * AxisAlignedBox represents a box in 3D space that is parallel to all coordinate axes.
 */
class AxisAlignedBox {
public:
    /**
     * Creates an AxisAlignedBox with the lower and higher corners {0|0|0}.
     */
    AxisAlignedBox() = default;
    /**
     * Creates an AxisAlignedBox with the specified lower and higher corners.
     * @param lower the lower corner
     * @param higher the higher corner
     */
    AxisAlignedBox(const vec3& lower, const vec3& higher);
    /**
     * Creates an AxisAligned Box with the specified lower and higher corners.
     * @param x1 the lower x value
     * @param y1 the lower y value
     * @param z1 the lower z value
     * @param x2 the higher x value
     * @param y2 the higher y value
     * @param z2 the higher z value
     */
    AxisAlignedBox(double x1, double y1, double z1, double x2, double y2, double z2);

    /**
     * Returns the lower corner.
     * @return the lower corner
     */
    [[nodiscard]] inline const vec3& getLowerCorner() const { return lower; }

    /**
     * Returns the higher corner.
     * @return the higher corner
     */
    [[nodiscard]] inline const vec3& getHigherCorner() const { return higher; }

    /**
     * Returns the size of the AxisAlignedBox.
     * @return the size of the box
     */
    [[nodiscard]] inline vec3 getDimension() const { return higher - lower; }

    /**
     * Checks collision with another AxisAlignedBox.
     * @param other the other AxisAlignedBox
     * @return `true` if they collide
     */
    [[nodiscard]] inline bool checkCollision(const AxisAlignedBox& other) const {
        return !(lower.x > other.higher.x || other.lower.x > higher.x
                 || lower.y > other.higher.y || other.lower.y > higher.y
                 || lower.z > other.higher.z || other.lower.z > higher.z);
    }

    /**
     * Returns the center of the AxisAlignedBox.
     * @return the center
     */
    [[nodiscard]] inline vec3 getCenter() const {
        return (lower + higher) / 2;
    }

private:
    vec3 lower;
    vec3 higher;
};

}
