/*
 * mat.cpp
 *
 *  Created on: 30.04.2019
 *      Author: florian
 */

#include "mat.h"
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

namespace Zoe {

mat2x2::mat2x2() = default;

mat2x2::mat2x2(const mat3x3 &mat) {
    data[0][0] = mat[0][0];
    data[0][1] = mat[0][1];
    data[1][0] = mat[1][0];
    data[1][1] = mat[1][1];
}

mat2x2::mat2x2(const mat4x4 &mat) {
    data[0][0] = mat[0][0];
    data[0][1] = mat[0][1];
    data[1][0] = mat[1][0];
    data[1][1] = mat[1][1];
}


mat2x2 mat2x2::operator+(const mat2x2 &mat) const {
    mat2x2 res;
    res[0][0] = (*this)[0][0] + mat[0][0];
    res[1][0] = (*this)[1][0] + mat[1][0];
    res[0][1] = (*this)[0][1] + mat[0][1];
    res[1][1] = (*this)[1][1] + mat[1][1];
    return res;
}

mat2x2 mat2x2::operator-(const mat2x2 &mat) const {
    mat2x2 res;
    res[0][0] = (*this)[0][0] - mat[0][0];
    res[1][0] = (*this)[1][0] - mat[1][0];
    res[0][1] = (*this)[0][1] - mat[0][1];
    res[1][1] = (*this)[1][1] - mat[1][1];
    return res;
}

vec2 mat2x2::operator*(const vec2 &vec) const {
    return {(*this)[0][0] * vec[0] + (*this)[0][1] * vec[1],
            (*this)[1][0] * vec[0] + (*this)[1][1] * vec[1]};
}

mat2x2 mat2x2::operator*(const mat2x2 &mat) const {
    mat2x2 res;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                res[i][j] += (*this)[i][k] * mat[k][j];
            }
        }
    }
    return res;
}

vec2 mat2x2::operator[](const int &index) const {
    return data[index];
}

vec2 &mat2x2::operator[](const int &index) {
    return data[index];
}

bool mat2x2::operator==(const mat2x2 &rhs) const {
    for (int i = 0; i < 2; ++i) {
        if (data[i] != rhs.data[i])
            return false;
    }
    return true;
}

bool mat2x2::operator!=(const mat2x2 &rhs) const {
    return !(rhs == *this);
}

mat2x2 mat2x2::transpose() const {
    mat2x2 result;
    result[0][0] = data[0][0];
    result[1][1] = data[1][1];
    result[0][1] = data[1][0];
    result[1][0] = data[0][1];
    return result;
}

mat2x2 mat2x2::inverse() const {
    mat2x2 result;
    double det = determinant();
    if (det == 0) {
        throw std::runtime_error("Inverse matrix is undefined for matrices with determinant 0");
    }

    result[0][0] = data[1][1] / det;
    result[1][1] = data[0][0] / det;
    result[0][1] = -data[0][1] / det;
    result[1][0] = -data[1][0] / det;

    return result;
}

double mat2x2::determinant() const {
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

const mat2x2 &mat2x2::identity() {
    static bool setupInstance = false;
    static mat2x2 instance;
    if (!setupInstance) {
        instance[0][0] = 1;
        instance[1][1] = 1;
    }
    return instance;
}

mat3x3::mat3x3() = default;

mat3x3::mat3x3(const mat2x2 &mat) {
    data[0][0] = mat[0][0];
    data[1][0] = mat[1][0];
    data[0][1] = mat[0][1];
    data[1][1] = mat[1][1];
    data[2][2] = 1;
}

mat3x3::mat3x3(const mat4x4 &mat) {
    data[0] = mat[0].xyz();
    data[1] = mat[1].xyz();
    data[2] = mat[2].xyz();
}


mat3x3 mat3x3::operator+(const mat3x3 &mat) const {
    mat3x3 res;
    res[0][0] = data[0][0] + mat[0][0];
    res[1][0] = data[1][0] + mat[1][0];
    res[2][0] = data[2][0] + mat[2][0];
    res[0][1] = data[0][1] + mat[0][1];
    res[1][1] = data[1][1] + mat[1][1];
    res[2][1] = data[2][1] + mat[2][1];
    res[0][2] = data[0][2] + mat[0][2];
    res[1][2] = data[1][2] + mat[1][2];
    res[2][2] = data[2][2] + mat[2][2];
    return res;
}

mat3x3 mat3x3::operator-(const mat3x3 &mat) const {
    mat3x3 res;
    res[0][0] = data[0][0] - mat[0][0];
    res[1][0] = data[1][0] - mat[1][0];
    res[2][0] = data[2][0] - mat[2][0];
    res[0][1] = data[0][1] - mat[0][1];
    res[1][1] = data[1][1] - mat[1][1];
    res[2][1] = data[2][1] - mat[2][1];
    res[0][2] = data[0][2] - mat[0][2];
    res[1][2] = data[1][2] - mat[1][2];
    res[2][2] = data[2][2] - mat[2][2];
    return res;
}

vec3 mat3x3::operator*(const vec3 &vec) const {
    return {(*this)[0][0] * vec[0] + (*this)[0][1] * vec[1] + (*this)[0][2] * vec[2],
            (*this)[1][0] * vec[0] + (*this)[1][1] * vec[1] + (*this)[1][2] * vec[2],
            (*this)[2][0] * vec[0] + (*this)[2][1] * vec[1] + (*this)[2][2] * vec[2]};
}

mat3x3 mat3x3::operator*(const mat3x3 &rhs) const {
    mat3x3 res;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                res[i][j] += data[i][k] * rhs[k][j];
            }
        }
    }
    return res;
}

vec3 mat3x3::operator[](const int &index) const {
    return data[index];
}

vec3 &mat3x3::operator[](const int &index) {
    return data[index];
}

bool mat3x3::operator==(const mat3x3 &rhs) const {
    for (int i = 0; i < 3; ++i) {
        if (data[i] != rhs.data[i])
            return false;
    }
    return true;
}

bool mat3x3::operator!=(const mat3x3 &rhs) const {
    return !(rhs == *this);
}

mat3x3 mat3x3::inverse() const {
    mat3x3 result;

    result[0][0] = data[1][1] * data[2][2] - data[1][2] * data[2][1];
    result[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
    result[0][2] = data[0][1] * data[1][2] - data[0][2] * data[1][1];
    result[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
    result[1][1] = data[0][0] * data[2][2] - data[0][2] * data[2][0];
    result[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
    result[2][0] = data[1][0] * data[2][1] - data[1][1] * data[2][0];
    result[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
    result[2][2] = data[0][0] * data[1][1] - data[0][1] * data[1][0];

    double det = result[0][0] * data[0][0] + result[0][1] * data[1][0] + result[0][2] * data[2][0];
    if (det == 0) {
        throw std::runtime_error("Inverse matrix is undefined for matrices with determinant 0");
    }

    det = 1 / det;

    return result * det;
}

mat3x3 mat3x3::transpose() const {
    mat3x3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = data[j][i];
        }
    }
    return result;
}

double mat3x3::determinant() const {
    return data[0][0] * data[1][1] * data[2][2] + data[0][1] * data[1][2] * data[2][0] +
           data[0][2] * data[1][0] * data[2][1]
           - (data[0][2] * data[1][1] * data[2][0] + data[1][0] * data[0][1] * data[2][2] +
              data[0][0] * data[2][1] * data[1][2]);
}

const mat3x3 &mat3x3::identity() {
    static bool setupInstance = false;
    static mat3x3 instance;
    if (!setupInstance) {
        instance[0][0] = 1;
        instance[1][1] = 1;
        instance[2][2] = 1;
    }
    return instance;
}

mat3x3 rotate2D(double angle) {
    double s = std::sin(angle);
    double c = std::cos(angle);
    mat3x3 res;
    res[0][0] = c;
    res[1][0] = -s;
    res[2][0] = 0;
    res[0][1] = s;
    res[1][1] = c;
    res[2][1] = 0;
    res[0][2] = 0;
    res[1][2] = 0;
    res[2][2] = 1;
    return res;
}

mat3x3 translate2D(double x, double y) {
    mat3x3 res;
    res[0][0] = 1;
    res[0][1] = 0;
    res[0][2] = x;
    res[1][0] = 0;
    res[1][1] = 1;
    res[1][2] = y;
    res[2][0] = 0;
    res[2][1] = 0;
    res[2][2] = 1;
    return res;
}

mat3x3 scale2D(double scaleX, double scaleY) {
    mat3x3 res;
    res[0][0] = scaleX;
    res[1][0] = 0;
    res[2][0] = 0;
    res[0][1] = 0;
    res[1][1] = scaleY;
    res[2][1] = 0;
    res[0][2] = 0;
    res[1][2] = 0;
    res[2][2] = 1;
    return res;
}

mat4x4::mat4x4() = default;

mat4x4::mat4x4(const mat2x2 &mat) {
    data[0][0] = mat[0][0];
    data[0][1] = mat[0][1];
    data[1][0] = mat[1][0];
    data[1][1] = mat[1][1];
    data[2][2] = 1;
    data[3][3] = 1;
}

mat4x4::mat4x4(const mat3x3 &mat) {
    data[0][0] = mat[0][0];
    data[0][1] = mat[0][1];
    data[0][2] = mat[0][2];
    data[1][0] = mat[1][0];
    data[1][1] = mat[1][1];
    data[1][2] = mat[1][2];
    data[2][0] = mat[2][0];
    data[2][1] = mat[2][1];
    data[2][2] = mat[2][2];
    data[3][3] = 1;
}

mat4x4 mat4x4::operator+(const mat4x4 &mat) const {
    mat4x4 res;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[i][j] = data[i][j] + mat[i][j];
        }
    }
    return res;
}

mat4x4 mat4x4::operator-(const mat4x4 &mat) const {
    mat4x4 res;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[i][j] = data[i][j] - mat[i][j];
        }
    }
    return res;
}

vec4 mat4x4::operator*(const vec4 &vec) const {
    return {data[0][0] * vec[0] + data[0][1] * vec[1] + data[0][2] * vec[2] + data[0][3] * vec[3],
            data[1][0] * vec[0] + data[1][1] * vec[1] + data[1][2] * vec[2] + data[1][3] * vec[3],
            data[2][0] * vec[0] + data[2][1] * vec[1] + data[2][2] * vec[2] + data[2][3] * vec[3],
            data[3][0] * vec[0] + data[3][1] * vec[1] + data[3][2] * vec[2] + data[3][3] * vec[3]};
}

mat4x4 mat4x4::operator*(const mat4x4 &mat) const {
    mat4x4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                res[i][j] += (*this)[i][k] * mat[k][j];
            }
        }
    }
    return res;
}

vec4 mat4x4::operator[](const int &index) const {
    return data[index];
}

vec4 &mat4x4::operator[](const int &index) {
    return data[index];
}

bool mat4x4::operator==(const mat4x4 &rhs) const {
    for (int i = 0; i < 4; ++i) {
        if (data[i] != rhs.data[i])
            return false;
    }
    return true;
}

bool mat4x4::operator!=(const mat4x4 &rhs) const {
    return !(rhs == *this);
}

mat4x4 mat4x4::inverse() const {
    mat4x4 result;

    result[0][0] = +(data[1][1] * data[2][2] * data[3][3] + data[1][2] * data[2][3] * data[3][1] +
                     data[1][3] * data[2][1] * data[3][2]
                     - (data[1][3] * data[2][2] * data[3][1] + data[3][2] * data[2][3] * data[1][1] +
                        data[1][2] * data[2][1] * data[3][3]));
    result[1][0] = -(data[1][0] * data[2][2] * data[3][3] + data[1][2] * data[2][3] * data[3][0] +
                     data[1][3] * data[3][2] * data[2][0]
                     - (data[1][3] * data[2][2] * data[3][0] + data[1][2] * data[2][0] * data[3][3] +
                        data[1][0] * data[3][2] * data[2][3]));
    result[2][0] = +(data[1][0] * data[2][1] * data[3][3] + data[1][3] * data[2][0] * data[3][1] +
                     data[1][1] * data[2][3] * data[3][0]
                     - (data[1][3] * data[3][0] * data[2][1] + data[1][1] * data[2][0] * data[3][3] +
                        data[2][3] * data[3][1] * data[1][0]));
    result[3][0] = -(data[1][0] * data[2][1] * data[3][2] + data[1][1] * data[2][2] * data[3][0] +
                     data[1][2] * data[2][0] * data[3][1]
                     - (data[1][2] * data[2][1] * data[3][0] + data[1][1] * data[2][0] * data[3][2] +
                        data[3][1] * data[2][2] * data[1][0]));

    result[0][1] = -(data[0][1] * data[2][2] * data[3][3] + data[0][2] * data[2][3] * data[3][1] +
                     data[0][3] * data[2][1] * data[3][2]
                     - (data[0][3] * data[2][2] * data[3][1] + data[3][2] * data[2][3] * data[0][1] +
                        data[0][2] * data[2][1] * data[3][3]));
    result[1][1] = +(data[0][0] * data[2][2] * data[3][3] + data[0][2] * data[2][3] * data[3][0] +
                     data[0][3] * data[3][2] * data[2][0]
                     - (data[0][3] * data[2][2] * data[3][0] + data[0][2] * data[2][0] * data[3][3] +
                        data[0][0] * data[3][2] * data[2][3]));
    result[2][1] = -(data[0][0] * data[2][1] * data[3][3] + data[0][3] * data[2][0] * data[3][1] +
                     data[0][1] * data[2][3] * data[3][0]
                     - (data[0][3] * data[3][0] * data[2][1] + data[0][1] * data[2][0] * data[3][3] +
                        data[2][3] * data[3][1] * data[0][0]));
    result[3][1] = +(data[0][0] * data[2][1] * data[3][2] + data[0][1] * data[2][2] * data[3][0] +
                     data[0][2] * data[2][0] * data[3][1]
                     - (data[0][2] * data[2][1] * data[3][0] + data[0][1] * data[2][0] * data[3][2] +
                        data[3][1] * data[2][2] * data[0][0]));

    result[0][2] = +(data[0][1] * data[1][2] * data[3][3] + data[0][2] * data[1][3] * data[3][1] +
                     data[0][3] * data[1][1] * data[3][2]
                     - (data[0][3] * data[1][2] * data[3][1] + data[3][2] * data[1][3] * data[0][1] +
                        data[0][2] * data[1][1] * data[3][3]));
    result[1][2] = -(data[0][0] * data[1][2] * data[3][3] + data[0][2] * data[1][3] * data[3][0] +
                     data[0][3] * data[3][2] * data[1][0]
                     - (data[0][3] * data[1][2] * data[3][0] + data[0][2] * data[1][0] * data[3][3] +
                        data[0][0] * data[3][2] * data[1][3]));
    result[2][2] = +(data[0][0] * data[1][1] * data[3][3] + data[0][3] * data[1][0] * data[3][1] +
                     data[0][1] * data[1][3] * data[3][0]
                     - (data[0][3] * data[3][0] * data[1][1] + data[0][1] * data[1][0] * data[3][3] +
                        data[1][3] * data[3][1] * data[0][0]));
    result[3][2] = -(data[0][0] * data[1][1] * data[3][2] + data[0][1] * data[1][2] * data[3][0] +
                     data[0][2] * data[1][0] * data[3][1]
                     - (data[0][2] * data[1][1] * data[3][0] + data[0][1] * data[1][0] * data[3][2] +
                        data[3][1] * data[1][2] * data[0][0]));

    result[0][3] = -(data[0][1] * data[1][2] * data[2][3] + data[0][2] * data[1][3] * data[2][1] +
                     data[0][3] * data[1][1] * data[2][2]
                     - (data[0][3] * data[1][2] * data[2][1] + data[2][2] * data[1][3] * data[0][1] +
                        data[0][2] * data[1][1] * data[2][3]));
    result[1][3] = +(data[0][0] * data[1][2] * data[2][3] + data[0][2] * data[1][3] * data[2][0] +
                     data[0][3] * data[2][2] * data[1][0]
                     - (data[0][3] * data[1][2] * data[2][0] + data[0][2] * data[1][0] * data[2][3] +
                        data[0][0] * data[2][2] * data[1][3]));
    result[2][3] = -(data[0][0] * data[1][1] * data[2][3] + data[0][3] * data[1][0] * data[2][1] +
                     data[0][1] * data[1][3] * data[2][0]
                     - (data[0][3] * data[2][0] * data[1][1] + data[0][1] * data[1][0] * data[2][3] +
                        data[1][3] * data[2][1] * data[0][0]));
    result[3][3] = +(data[0][0] * data[1][1] * data[2][2] + data[0][1] * data[1][2] * data[2][0] +
                     data[0][2] * data[1][0] * data[2][1]
                     - (data[0][2] * data[1][1] * data[2][0] + data[0][1] * data[1][0] * data[2][2] +
                        data[2][1] * data[1][2] * data[0][0]));

    double det = result[0][0] * data[0][0] + result[0][1] * data[1][0] + result[0][2] * data[2][0] +
                result[0][3] * data[3][0];
    if (det == 0) {
        throw std::runtime_error("Inverse matrix is undefined for matrices with determinant 0");
    }

    det = 1 / det;

    return result * det;
}

mat4x4 mat4x4::transpose() const {
    mat4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = data[j][i];
        }
    }
    return result;
}

double mat4x4::determinant() const {
    return
            data[0][0] * (
                    data[1][1] * data[2][2] * data[3][3] + data[1][2] * data[2][3] * data[3][1] +
                    data[1][3] * data[2][1] * data[3][2]
                    - (data[1][3] * data[2][2] * data[3][1] + data[3][2] * data[2][3] * data[1][1] +
                       data[1][2] * data[2][1] * data[3][3])
            )
            - data[0][1] * (
                    data[1][0] * data[2][2] * data[3][3] + data[1][2] * data[2][3] * data[3][0] +
                    data[1][3] * data[3][2] * data[2][0]
                    - (data[1][3] * data[2][2] * data[3][0] + data[1][2] * data[2][0] * data[3][3] +
                       data[1][0] * data[3][2] * data[2][3])
            )
            + data[0][2] * (
                    data[1][0] * data[2][1] * data[3][3] + data[1][3] * data[2][0] * data[3][1] +
                    data[1][1] * data[2][3] * data[3][0]
                    - (data[1][3] * data[3][0] * data[2][1] + data[1][1] * data[2][0] * data[3][3] +
                       data[2][3] * data[3][1] * data[1][0])
            )
            - data[0][3] * (
                    data[1][0] * data[2][1] * data[3][2] + data[1][1] * data[2][2] * data[3][0] +
                    data[1][2] * data[2][0] * data[3][1]
                    - (data[1][2] * data[2][1] * data[3][0] + data[1][1] * data[2][0] * data[3][2] +
                       data[3][1] * data[2][2] * data[1][0])
            );
}

const mat4x4 &mat4x4::identity() {
    static bool setupInstance = false;
    static mat4x4 instance;
    if (!setupInstance) {
        instance[0][0] = 1;
        instance[1][1] = 1;
        instance[2][2] = 1;
        instance[3][3] = 1;
    }
    return instance;
}

mat4x4 rotateXY3D(double angle) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    mat4x4 res;
    res[0][0] = c;
    res[0][1] = -s;
    res[0][2] = 0;
    res[0][3] = 0;
    res[1][0] = s;
    res[1][1] = c;
    res[1][2] = 0;
    res[1][3] = 0;
    res[2][0] = 0;
    res[2][1] = 0;
    res[2][2] = 1;
    res[2][3] = 0;
    res[3][0] = 0;
    res[3][1] = 0;
    res[3][2] = 0;
    res[3][3] = 1;
    return res;
}

mat4x4 rotateYZ3D(double angle) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    mat4x4 res;
    res[0][0] = 1;
    res[0][1] = 0;
    res[0][2] = 0;
    res[0][3] = 0;
    res[1][0] = 0;
    res[1][1] = c;
    res[1][2] = -s;
    res[1][3] = 0;
    res[2][0] = 0;
    res[2][1] = s;
    res[2][2] = c;
    res[2][3] = 0;
    res[3][0] = 0;
    res[3][1] = 0;
    res[3][2] = 0;
    res[3][3] = 1;
    return res;
}

mat4x4 rotateXZ3D(double angle) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    mat4x4 res;
    res[0][0] = c;
    res[0][1] = 0;
    res[0][2] = s;
    res[0][3] = 0;
    res[1][0] = 0;
    res[1][1] = 1;
    res[1][2] = 0;
    res[1][3] = 0;
    res[2][0] = -s;
    res[2][1] = 0;
    res[2][2] = c;
    res[2][3] = 0;
    res[3][0] = 0;
    res[3][1] = 0;
    res[3][2] = 0;
    res[3][3] = 1;
    return res;
}

mat4x4 translate3D(double x, double y, double z) {
    mat4x4 res;
    res[0][0] = 1;
    res[0][1] = 0;
    res[0][2] = 0;
    res[0][3] = x;
    res[1][0] = 0;
    res[1][1] = 1;
    res[1][2] = 0;
    res[1][3] = y;
    res[2][0] = 0;
    res[2][1] = 0;
    res[2][2] = 1;
    res[2][3] = z;
    res[3][0] = 0;
    res[3][1] = 0;
    res[3][2] = 0;
    res[3][3] = 1;
    return res;
}

mat4x4 scale3D(double scaleX, double scaleY, double scaleZ) {
    mat4x4 res;
    res[0][0] = scaleX;
    res[1][0] = 0;
    res[2][0] = 0;
    res[3][0] = 0;
    res[0][1] = 0;
    res[1][1] = scaleY;
    res[2][1] = 0;
    res[3][1] = 0;
    res[0][2] = 0;
    res[1][2] = 0;
    res[2][2] = scaleZ;
    res[3][2] = 0;
    res[0][3] = 0;
    res[1][3] = 0;
    res[2][3] = 0;
    res[3][3] = 1;
    return res;
}

mat4x4 orthographic(double left, double top, double right, double bottom, double near, double far) {
    mat4x4 res;
    res[0][0] = 2 / (right - left);
    res[0][1] = 0;
    res[0][2] = 0;
    res[0][3] = -(right + left) / (right - left);
    res[1][0] = 0;
    res[1][1] = 2 / (top - bottom);
    res[1][2] = 0;
    res[1][3] = -(top + bottom) / (top - bottom);
    res[2][0] = 0;
    res[2][1] = 0;
    res[2][2] = 2 / (far - near);
    res[2][3] = -(far + near) / (far - near);
    res[3][0] = 0;
    res[3][1] = 0;
    res[3][2] = 0;
    res[3][3] = 1;
    return res;
}

mat4x4 calcViewMatrix(vec3 position, vec3 rotation) {
    return rotateXY3D(-rotation.z) * rotateYZ3D(-rotation.x) * rotateXZ3D(-rotation.y)
           * translate3D(-position.x, -position.y, -position.z);
}

mat4x4 perspective(double near, double far, double fov, double aspectRatio) {
    fov = fov / 180 * PI;
    const double n = near;
    const double f = far;
    const double t = std::tan(fov / 2);
    const double r = t * aspectRatio;

    mat4x4 proj;
    proj[0][0] = n / r;
    proj[0][1] = 0;
    proj[0][2] = 0;
    proj[0][3] = 0;
    proj[1][0] = 0;
    proj[1][1] = n / t;
    proj[1][2] = 0;
    proj[1][3] = 0;
    proj[2][0] = 0;
    proj[2][1] = 0;
    proj[2][2] = -(f + n) / (f - n);
    proj[2][3] = -2 * f * n / (f - n);
    proj[3][0] = 0;
    proj[3][1] = 0;
    proj[3][2] = -1;
    proj[3][3] = 0;

    return proj * scale3D(1, 1, -1);
}

}

Zoe::mat4x4 operator*(const double a, const Zoe::mat4x4 &mat) {
    return operator*(mat, a);
}

Zoe::mat4x4 operator*(const Zoe::mat4x4 &mat, const double a) {
    Zoe::mat4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = mat[i][j] * a;
        }
    }
    return result;
}

Zoe::mat2x2 operator*(const double a, const Zoe::mat2x2 &mat) {
    return operator*(mat, a);
}

Zoe::mat2x2 operator*(const Zoe::mat2x2 &mat, const double a) {
    Zoe::mat2x2 result;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = mat[i][j] * a;
        }
    }
    return result;
}

Zoe::mat3x3 operator*(const double a, const Zoe::mat3x3 &mat) {
    return operator*(mat, a);
}

Zoe::mat3x3 operator*(const Zoe::mat3x3 &mat, const double a) {
    Zoe::mat3x3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = mat[i][j] * a;
        }
    }
    return result;
}

std::ostream &operator<<(std::ostream &stream, const Zoe::mat2x2 &v) {
    return stream << v[0][0] << " | " << v[0][1] << "\n" << v[1][0] << " | "
                  << v[1][1];
}

std::ostream &operator<<(std::ostream &stream, const Zoe::mat3x3 &v) {
    return stream << v[0][0] << " | " << v[0][1] << " | " << v[0][2] << "\n"
                  << v[1][0] << " | " << v[1][1] << " | " << v[1][2] << "\n"
                  << v[2][0] << " | " << v[2][1] << " | " << v[2][2];
}

std::ostream &operator<<(std::ostream &stream, const Zoe::mat4x4 &v) {
    return stream << v[0][0] << " | " << v[0][1] << " | " << v[0][2] << " | " << v[0][3] << "\n"
                  << v[1][0] << " | " << v[1][1] << " | " << v[1][2] << " | " << v[1][3] << "\n"
                  << v[2][0] << " | " << v[2][1] << " | " << v[2][2] << " | " << v[2][3] << "\n"
                  << v[3][0] << " | " << v[3][1] << " | " << v[3][2] << " | " << v[3][3];
}