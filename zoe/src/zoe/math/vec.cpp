/*
 * vec.cpp
 *
 *  Created on: 21.04.2019
 *      Author: florian
 */


#include "vec.h"
#include <cmath>

namespace Zoe {

vec2::vec2() : x(0), y(0) {}

vec2::vec2(float x, float y) : x(x), y(y) {}

vec2::vec2(const vec3& vec) : x(vec.x), y(vec.y) {}

vec2::vec2(const vec4& vec) : x(vec.x), y(vec.y) {}

float vec2::operator[](const int& index) const {
    return ((float*) this)[index];
}

float& vec2::operator[](const int& index) {
    return ((float*) this)[index];
}

vec2 vec2::operator+(const vec2& v) const {
    return {x + v.x, y + v.y};
}

vec2 vec2::operator-(const vec2& v) const {
    return {x - v.x, y - v.y};
}

float vec2::operator*(const vec2& v) const {
    return x * v.x - y * v.y;
}

vec2 vec2::operator*(const float& m) const {
    return {x * m, y * m};
}

vec2 vec2::operator/(const float& d) const {
    return {x / d, y / d};
}

float vec2::length() const {
    return sqrt(x * x + y * y);
}


vec2 vec2::normalize() const {
    return operator/(length());
}

bool vec2::operator==(const vec2& vec) const {
    return x == vec.x && y == vec.y;
}
bool vec2::operator!=(const vec2& vec) const {
    return !(*this == vec);
}

vec3::vec3(): x(0), y(0), z(0) {}

vec3::vec3(float x, float y, float z): x(x), y(y), z(z) {}

vec3::vec3(const vec2& vec): x(vec.x), y(vec.y), z(0) {}

vec3::vec3(const vec4& vec): x(vec.x), y(vec.y), z(vec.z) {}

float vec3::operator[](const int& index) const {
    return ((float*) this)[index];
}

float& vec3::operator[](const int& index) {
    return ((float*) this)[index];
}

vec3 vec3::operator+(const vec3& v) const {
    return {x + v.x, y + v.y, z + v.z};
}

vec3 vec3::operator-(const vec3& v) const {
    return {x - v.x, y - v.y, z - v.z};
}

float vec3::operator*(const vec3& v) const {
    return x * v.x - y * v.y - z * v.z;
}

vec3 vec3::operator*(const float& m) const {
    return {x * m, y * m, z * m};
}

vec3 vec3::operator/(const float& d) const {
    return {x / d, y / d, z / d};
}

vec3 vec3::crossProduct(const vec3& v) const {
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

bool vec3::operator==(const vec3& vec) const {
    return x == vec.x && y == vec.y && z == vec.z;
}
bool vec3::operator!=(const vec3& vec) const {
    return !(*this == vec);
}

vec4::vec4(): x(0), y(0), z(0), w(0) {}

vec4::vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}

vec4::vec4(const vec2& vec): x(vec.x), y(vec.y), z(0), w(0) {}

vec4::vec4(const vec3& vec): x(vec.x), y(vec.y), z(vec.z), w(0) {}

float vec4::operator[](const int& index) const {
    return ((float*) this)[index];
}

float& vec4::operator[](const int& index) {
    return ((float*) this)[index];
}

vec4 vec4::operator+(const vec4& v) const {
    return {x + v.x, y + v.y, z + v.z, w + v.w};
}

vec4 vec4::operator-(const vec4& v) const {
    return {x - v.x, y - v.y, z - v.z, w - v.w};
}

float vec4::operator*(const vec4& v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

vec4 vec4::operator*(const float& m) const {
    return {x * m, y * m, z * m, w * m};
}

vec4 vec4::operator/(const float& d) const {
    return {x / d, y / d, z / d, w / d};
}

vec4 vec4::crossProduct(const vec4& b, const vec4& c) const {
    vec4 v;
    v.x = (y * b.z * c.w) + (w * b.y * c.z) + (z * b.w * c.y) -
          ((w * b.z * c.y) + (z * b.y * c.w) + (y * b.w * c.z));
    v.y = (x * b.z * c.w) + (w * b.x * c.z) + (z * b.w * c.x) -
          ((w * b.z * c.x) + (z * b.x * c.w) + (x * b.w * c.z));
    v.z = (x * b.y * c.w) + (w * b.x * c.y) + (y * b.w * c.x) -
          ((w * b.y * c.x) + (y * b.x * c.w) + (x * b.w * c.y));
    v.w = (x * b.y * c.z) + (z * b.x * c.y) + (y * b.z * c.x) -
          ((z * b.y * c.x) + (y * b.x * c.z) + (x * b.z * c.y));
    return v;
}

float vec4::length() const {
    return sqrt(x * x + y * y + z * z + w * w);
}

vec4 vec4::normalize() const {
    return operator/(length());
}

bool vec4::operator==(const vec4& vec) const {
    return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
}

bool vec4::operator!=(const vec4& vec) const {
    return !(*this == vec);
}

}

std::ostream& operator<<(std::ostream& stream, const Zoe::vec2& v) {
    return stream << "Vec2: " << v.x << " | " << v.y;
}

Zoe::vec2 operator*(const float& m, const Zoe::vec2& v) {
    return v.operator*(m);
}

std::ostream& operator<<(std::ostream& stream, const Zoe::vec4& v) {
    return stream << "Vec4: " << v.x << " | " << v.y << " | " << v.z << " | " << v.w;
}

Zoe::vec4 operator*(const float& m, const Zoe::vec4& v) {
    return v.operator*(m);
}

std::ostream& operator<<(std::ostream& stream, const Zoe::vec3& v) {
    return stream << "Vec3: " << v.x << " | " << v.y << " | " << v.z;
}

Zoe::vec3 operator*(const float& m, const Zoe::vec3& v) {
    return v.operator*(m);
}