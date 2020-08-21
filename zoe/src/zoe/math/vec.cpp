/*
 * vec.cpp
 *
 *  Created on: 21.04.2019
 *      Author: florian
 */


#include "vec.h"
#include <math.h>

namespace Zoe {

    float vec2::operator[](const int &index) const {
        return ((float *) this)[index];
    }

    float &vec2::operator[](const int &index) {
        return ((float *) this)[index];
    }

    vec2 vec2::operator+(const vec2 &v) const {
        return {x + v.x, y + v.y};
    }

    vec2 vec2::operator-(const vec2 &v) const {
        return {x - v.x, y - v.y};
    }

    float vec2::operator*(const vec2 &v) const {
        return x * v.x - y * v.y;
    }

    vec2 vec2::operator*(const float &m) const {
        return {x * m, y * m};
    }

    vec2 vec2::operator/(const float &d) const {
        return {x / d, y / d};
    }

    float vec2::length() const {
        return sqrt(x * x + y * y);
    }

    vec2 vec2::normalize() const {
        return operator/(length());
    }


    std::ostream &operator<<(std::ostream &stream, const vec2 &v) {
        return stream << "Vec2: " << v.x << " | " << v.y;
    }

    vec2 operator*(const float &m, const vec2 &v) {
        return v.operator*(m);
    }

    bool vec2::operator==(const vec2 &vec) const {
        return x == vec.x && y == vec.y;
    }

    bool vec2::operator!=(const vec2 &vec) const {
        return !(*this == vec);
    }

    float vec3::operator[](const int &index) const {
        return ((float *) this)[index];
    }

    float &vec3::operator[](const int &index) {
        return ((float *) this)[index];
    }

    vec3 vec3::operator+(const vec3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    vec3 vec3::operator-(const vec3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    float vec3::operator*(const vec3 &v) const {
        return x * v.x - y * v.y - z * v.z;
    }

    vec3 vec3::operator*(const float &m) const {
        return {x * m, y * m, z * m};
    }

    vec3 vec3::operator/(const float &d) const {
        return {x / d, y / d, z / d};
    }

    vec3 vec3::crossProduct(const vec3 &v) const {
        return {y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x};
    }

    float vec3::length() const {
        return sqrt(x * x + y * y + z * z);
    }

    vec3 vec3::normalize() const {
        return operator/(length());
    }


    std::ostream &operator<<(std::ostream &stream, const vec3 &v) {
        return stream << "Vec3: " << v.x << " | " << v.y << " | " << v.z;
    }

    vec3 operator*(const float &m, const vec3 &v) {
        return v.operator*(m);
    }

    bool vec3::operator==(const vec3 &vec) const {
        return x == vec.x && y == vec.y && z == vec.z;
    }

    bool vec3::operator!=(const vec3 &vec) const {
        return !(*this == vec);
    }

    float vec4::operator[](const int &index) const {
        return ((float *) this)[index];
    }

    float &vec4::operator[](const int &index) {
        return ((float *) this)[index];
    }

    vec4 vec4::operator+(const vec4 &v) const {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    vec4 vec4::operator-(const vec4 &v) const {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    float vec4::operator*(const vec4 &v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    vec4 vec4::operator*(const float &m) const {
        return {x * m, y * m, z * m, w * m};
    }

    vec4 vec4::operator/(const float &d) const {
        return {x / d, y / d, z / d, w / d};
    }

    vec4 vec4::crossProduct(const vec4 &b, const vec4 &c) const {
        vec4 v;
        v.x = (this->y * b.z * c.w) + (this->w * b.y * c.z) + (this->z * b.w * c.y) -
              ((this->w * b.z * c.y) + (this->z * b.y * c.w) + (this->y * b.w * c.z));
        v.y = (this->x * b.z * c.w) + (this->w * b.x * c.z) + (this->z * b.w * c.x) -
              ((this->w * b.z * c.x) + (this->z * b.x * c.w) + (this->x * b.w * c.z));
        v.z = (this->x * b.y * c.w) + (this->w * b.x * c.y) + (this->y * b.w * c.x) -
              ((this->w * b.y * c.x) + (this->y * b.x * c.w) + (this->x * b.w * c.y));
        v.w = (this->x * b.y * c.z) + (this->z * b.x * c.y) + (this->y * b.z * c.x) -
              ((this->z * b.y * c.x) + (this->y * b.x * c.z) + (this->x * b.z * c.y));
        return v;
    }

    float vec4::length() const {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    vec4 vec4::normalize() const {
        return operator/(length());
    }

    std::ostream &operator<<(std::ostream &stream, const vec4 &v) {
        return stream << "Vec4: " << v.x << " | " << v.y << " | " << v.z << " | " << v.w;
    }

    vec4 operator*(const float &m, const vec4 &v) {
        return v.operator*(m);
    }

    bool vec4::operator==(const vec4 &vec) const {
        return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
    }

    bool vec4::operator!=(const vec4 &vec) const {
        return !(*this == vec);
    }

}
