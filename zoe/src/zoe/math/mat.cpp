/*
 * mat.cpp
 *
 *  Created on: 30.04.2019
 *      Author: florian
 */

#include "mat.h"
#include <cmath>

namespace Zoe {

mat2x2::mat2x2() = default;
mat2x2::~mat2x2() = default;
mat2x2 mat2x2::operator +(const mat2x2& mat) const {
	mat2x2 res;
	res[0][0] = (*this)[0][0] + mat[0][0];
	res[1][0] = (*this)[1][0] + mat[1][0];
	res[0][1] = (*this)[0][1] + mat[0][1];
	res[1][1] = (*this)[1][1] + mat[1][1];
	return res;
}

mat2x2 mat2x2::operator -(const mat2x2& mat) const {
	mat2x2 res;
	res[0][0] = (*this)[0][0] - mat[0][0];
	res[1][0] = (*this)[1][0] - mat[1][0];
	res[0][1] = (*this)[0][1] - mat[0][1];
	res[1][1] = (*this)[1][1] - mat[1][1];
	return res;
}

vec2 mat2x2::operator *(const vec2& vec) const {
	return {(*this)[0][0]*vec[0]+(*this)[0][1]*vec[1],
		(*this)[1][0]*vec[0]+(*this)[1][1]*vec[1]};
}

mat2x2 mat2x2::operator *(const mat2x2& mat) const {
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

vec2 mat2x2::operator [](const int& index) const {
	return data[index];
}

vec2& mat2x2::operator [](const int& index) {
	return data[index];
}

mat2x2 operator*(const float a, const mat2x2& mat){
    return operator*(mat, a);
}

mat2x2 operator*(const mat2x2& mat, const float a){
    mat2x2 result;
    for(int i = 0; i < 2; ++i){
        for(int j = 0; j < 2; ++j){
            result[i][j] = mat[i][j] * a;
        }
    }
    return result;
}

mat3x3::mat3x3() = default;
mat3x3::mat3x3(const mat2x2& mat) {
	data[0][0] = mat[0][0];
	data[1][0] = mat[1][0];
	data[0][1] = mat[0][1];
	data[1][1] = mat[1][1];
	data[2][0] = 0;
	data[2][1] = 0;
	data[0][2] = 0;
	data[1][2] = 0;
	data[2][2] = 1;
}
mat3x3::~mat3x3() = default;

mat3x3 mat3x3::operator +(const mat3x3& mat) const {
	mat3x3 res;
	res[0][0] = (*this)[0][0] + mat[0][0];
	res[1][0] = (*this)[1][0] + mat[1][0];
	res[2][0] = (*this)[2][0] + mat[2][0];
	res[0][1] = (*this)[0][1] + mat[0][1];
	res[1][1] = (*this)[1][1] + mat[1][1];
	res[2][1] = (*this)[2][1] + mat[2][1];
	res[0][2] = (*this)[0][2] + mat[0][2];
	res[1][2] = (*this)[1][2] + mat[1][2];
	res[2][2] = (*this)[2][2] + mat[2][2];
	return res;
}

mat3x3 mat3x3::operator -(const mat3x3& mat) const {
	mat3x3 res;
	res[0][0] = (*this)[0][0] - mat[0][0];
	res[1][0] = (*this)[1][0] - mat[1][0];
	res[2][0] = (*this)[2][0] - mat[2][0];
	res[0][1] = (*this)[0][1] - mat[0][1];
	res[1][1] = (*this)[1][1] - mat[1][1];
	res[2][1] = (*this)[2][1] - mat[2][1];
	res[0][2] = (*this)[0][2] - mat[0][2];
	res[1][2] = (*this)[1][2] - mat[1][2];
	res[2][2] = (*this)[2][2] - mat[2][2];
	return res;
}

vec3 mat3x3::operator *(const vec3& vec) const {
	return {(*this)[0][0]*vec[0]+(*this)[0][1]*vec[1]+(*this)[0][2]*vec[2],
		(*this)[1][0]*vec[0]+(*this)[1][1]*vec[1]+(*this)[1][2]*vec[2],
		(*this)[2][0]*vec[0]+(*this)[2][1]*vec[1]+(*this)[2][2]*vec[2]};
}

mat3x3 mat3x3::operator *(const mat3x3& mat) const {
	mat3x3 res;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				res[i][j] += (*this)[i][k] * mat[k][j];
			}
		}
	}
	return res;
}

vec3 mat3x3::operator [](const int& index) const {
	return data[index];
}

vec3& mat3x3::operator [](const int& index) {
	return data[index];
}

mat3x3 rotate2D(float angle) {
	float s = std::sin(angle);
	float c = std::cos(angle);
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
mat3x3 translate2D(float x, float y) {
	mat3x3 res;
	res[0][0] = 1;
	res[1][0] = 0;
	res[2][0] = 0;
	res[0][1] = 0;
	res[1][1] = 1;
	res[2][1] = 0;
	res[0][2] = x;
	res[1][2] = y;
	res[2][2] = 1;
	return res;
}
mat3x3 scale2D(float scaleX, float scaleY) {
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

std::ostream& operator<<(std::ostream& stream, const mat2x2& v) {
	return stream << v[0][0] << " | " << v[0][1] << "\n" << v[1][0] << " | "
			<< v[1][1];
}

std::ostream& operator<<(std::ostream& stream, const mat3x3& v) {
	return stream << v[0][0] << " | " << v[0][1] << " | " << v[0][2] << "\n"
			<< v[1][0] << " | " << v[1][1] << " | " << v[1][2] << "\n"
			<< v[2][0] << " | " << v[2][1] << " | " << v[2][2];
}

mat3x3 operator*(const float a, const mat3x3& mat){
    return operator*(mat, a);
}

mat3x3 operator*(const mat3x3& mat, const float a){
    mat3x3 result;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            result[i][j] = mat[i][j] * a;
        }
    }
    return result;
}

mat4x4::mat4x4() = default;

mat4x4::mat4x4(const mat3x3& mat) {
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

mat4x4::~mat4x4() = default;

mat4x4 mat4x4::operator +(const mat4x4& mat) const {
	mat4x4 res;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            res[i][j] = data[i][j] + mat[i][j];
        }
    }
	return res;
}

mat4x4 mat4x4::operator -(const mat4x4& mat) const {
	mat4x4 res;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            res[i][j] = data[i][j] - mat[i][j];
        }
    }
	return res;
}

vec4 mat4x4::operator *(const vec4& vec) const {
	return {data[0][0]*vec[0]+data[0][1]*vec[1]+data[0][2]*vec[2]+data[0][3]*vec[3],
		data[1][0]*vec[0]+data[1][1]*vec[1]+data[1][2]*vec[2]+data[1][3]*vec[3],
		data[2][0]*vec[0]+data[2][1]*vec[1]+data[2][2]*vec[2]+data[2][3]*vec[3],
		data[3][0]*vec[0]+data[3][1]*vec[1]+data[3][2]*vec[2]+data[3][3]*vec[3]};
}

mat4x4 mat4x4::operator *(const mat4x4& mat) const {
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

vec4 mat4x4::operator [](const int& index) const {
	return data[index];
}

vec4& mat4x4::operator [](const int& index) {
	return data[index];
}

std::ostream& operator <<(std::ostream& stream, const mat4x4& v) {
	return stream << v[0][0] << " | " << v[0][1] << " | " << v[0][2] << " | " << v[0][3] <<"\n"
			<< v[1][0] << " | " << v[1][1] << " | " << v[1][2] << " | " << v[1][3] <<"\n"
			<< v[2][0] << " | " << v[2][1] << " | " << v[2][2] << " | " << v[2][3] <<"\n"
			<< v[3][0] << " | " << v[3][1] << " | " << v[3][2] << " | " << v[3][3];
}

mat4x4 rotateXY3D(float angle) {
	float c=std::cos(angle);
	float s=std::sin(angle);
	mat4x4 res;
	res[0][0]= c;res[0][1]=-s;res[0][2]= 0;res[0][3]= 0;
	res[1][0]= s;res[1][1]= c;res[1][2]= 0;res[1][3]= 0;
	res[2][0]= 0;res[2][1]= 0;res[2][2]= 1;res[2][3]= 0;
	res[3][0]= 0;res[3][1]= 0;res[3][2]= 0;res[3][3]= 1;
	return res;
}

mat4x4 rotateYZ3D(float angle) {
	float c = std::cos(angle);
	float s = std::sin(angle);
	mat4x4 res;
	res[0][0]=1;res[0][1]=0;res[0][2]= 0;res[0][3]=0;
	res[1][0]=0;res[1][1]=c;res[1][2]=-s;res[1][3]=0;
	res[2][0]=0;res[2][1]=s;res[2][2]= c;res[2][3]=0;
	res[3][0]=0;res[3][1]=0;res[3][2]= 0;res[3][3]=1;
	return res;
}

mat4x4 rotateXZ3D(float angle) {
	float c = std::cos(angle);
	float s = std::sin(angle);
	mat4x4 res;
	res[0][0]=c;res[0][1]=0;res[0][2]= s;res[0][3]=0;
	res[1][0]=0;res[1][1]=1;res[1][2]= 0;res[1][3]=0;
	res[2][0]=-s;res[2][1]=0;res[2][2]= c;res[2][3]=0;
	res[3][0]=0;res[3][1]=0;res[3][2]= 0;res[3][3]=1;
	return res;
}

mat4x4 translate3D(float x, float y, float z) {
	mat4x4 res;
	res[0][0]=1;res[0][1]=0;res[0][2]= 0;res[0][3]=x;
	res[1][0]=0;res[1][1]=1;res[1][2]= 0;res[1][3]=y;
	res[2][0]=0;res[2][1]=0;res[2][2]= 1;res[2][3]=z;
	res[3][0]=0;res[3][1]=0;res[3][2]= 0;res[3][3]=1;
	return res;
}

mat4x4 scale3D(float scaleX, float scaleY, float scaleZ) {
	mat4x4 res;
	res[0][0]=scaleX;res[1][0]=0;res[2][0]= 0;res[3][0]=0;
	res[0][1]=0;res[1][1]=scaleY;res[2][1]= 0;res[3][1]=0;
	res[0][2]=0;res[1][2]=0;res[2][2]= scaleZ;res[3][2]=0;
	res[0][3]=0;res[1][3]=0;res[2][3]= 0;res[3][3]=1;
	return res;
}

mat4x4 orthographic(float left, float top, float right, float bottom, float near, float far){
	mat4x4 res;
	res[0][0]=2/(right-left);res[0][1]=0;res[0][2]= 0;res[0][3]=-(right+left)/(right-left);
	res[1][0]=0;res[1][1]=2/(top-bottom);res[1][2]= 0;res[1][3]=-(top+bottom)/(top-bottom);
	res[2][0]=0;res[2][1]=0;res[2][2]= 2/(far-near);res[2][3]=-(far+near)/(far-near);
	res[3][0]=0;res[3][1]=0;res[3][2]= 0;res[3][3]=1;
	return res;
}

mat4x4 calcViewMatrix(vec3 position, vec3 rotation){
	return rotateXY3D(-rotation.z) * rotateYZ3D(-rotation.x) * rotateXZ3D(-rotation.y)
			*translate3D(-position.x, -position.y, -position.z);
}

mat4x4 perspective(float near,float far,float fov,float aspectRatio){
	fov = fov/180*PI;
	const float n = near;
	const float f = far;
	const float t = std::tan(fov/2);
	const float r = t*aspectRatio;

	mat4x4 proj;
	proj[0][0]=n/r;	proj[0][1]=0;	proj[0][2]= 0;				proj[0][3]=0;
	proj[1][0]=0;	proj[1][1]=n/t;	proj[1][2]= 0;				proj[1][3]=0;
	proj[2][0]=0;	proj[2][1]=0;	proj[2][2]= -(f+n)/(f-n);	proj[2][3]=-2*f*n/(f-n);
	proj[3][0]=0;	proj[3][1]=0;	proj[3][2]= -1;				proj[3][3]=0;

	return proj*scale3D(1, 1, -1);
}

mat4x4 inverse(const mat4x4& mat){
    float det;
    const float* ma = reinterpret_cast<float*>(&(const_cast<mat4x4&>(mat)[0]));
    mat4x4 result;

    result[0][0] = ma[5]  * ma[10] * ma[15] -
             ma[5]  * ma[14] * ma[11] -
             ma[6]  * ma[9]  * ma[15] +
             ma[6]  * ma[13]  * ma[11] +
             ma[7] * ma[9]  * ma[14] -
             ma[7] * ma[13]  * ma[10];

    result[1][0] = -ma[1]  * ma[10] * ma[15] +
             ma[1]  * ma[14] * ma[11] +
             ma[2]  * ma[9]  * ma[15] -
             ma[2]  * ma[13]  * ma[11] -
             ma[3] * ma[9]  * ma[14] +
             ma[3] * ma[13]  * ma[10];

    result[2][0] = ma[1]  * ma[6] * ma[15] -
             ma[1]  * ma[14] * ma[7] -
             ma[2]  * ma[5] * ma[15] +
             ma[2]  * ma[13] * ma[7] +
             ma[3] * ma[5] * ma[14] -
             ma[3] * ma[13] * ma[6];

    result[3][0] = -ma[1]  * ma[6] * ma[11] +
              ma[1]  * ma[10] * ma[7] +
              ma[2]  * ma[5] * ma[11] -
              ma[2]  * ma[9] * ma[7] -
              ma[3] * ma[5] * ma[10] +
              ma[3] * ma[9] * ma[6];

    result[0][1] = -ma[4]  * ma[10] * ma[15] +
             ma[4]  * ma[14] * ma[11] +
             ma[6]  * ma[8] * ma[15] -
             ma[6]  * ma[12] * ma[11] -
             ma[7] * ma[8] * ma[14] +
             ma[7] * ma[12] * ma[10];

    result[1][1] = ma[0]  * ma[10] * ma[15] -
             ma[0]  * ma[14] * ma[11] -
             ma[2]  * ma[8] * ma[15] +
             ma[2]  * ma[12] * ma[11] +
             ma[3] * ma[8] * ma[14] -
             ma[3] * ma[12] * ma[10];

    result[2][1] = -ma[0]  * ma[6] * ma[15] +
             ma[0]  * ma[14] * ma[7] +
             ma[2]  * ma[4] * ma[15] -
             ma[2]  * ma[12] * ma[7] -
             ma[3] * ma[4] * ma[14] +
             ma[3] * ma[12] * ma[6];

    result[3][1] = ma[0]  * ma[6] * ma[11] -
              ma[0]  * ma[10] * ma[7] -
              ma[2]  * ma[4] * ma[11] +
              ma[2]  * ma[8] * ma[7] +
              ma[3] * ma[4] * ma[10] -
              ma[3] * ma[8] * ma[6];

    result[0][2] = ma[4]  * ma[9] * ma[15] -
             ma[4]  * ma[13] * ma[11] -
             ma[5]  * ma[8] * ma[15] +
             ma[5]  * ma[12] * ma[11] +
             ma[7] * ma[8] * ma[13] -
             ma[7] * ma[12] * ma[9];

    result[1][2] = -ma[0]  * ma[9] * ma[15] +
             ma[0]  * ma[13] * ma[11] +
             ma[1]  * ma[8] * ma[15] -
             ma[1]  * ma[12] * ma[11] -
             ma[3] * ma[8] * ma[13] +
             ma[3] * ma[12] * ma[9];

    result[2][2] = ma[0]  * ma[5] * ma[15] -
              ma[0]  * ma[13] * ma[7] -
              ma[1]  * ma[4] * ma[15] +
              ma[1]  * ma[12] * ma[7] +
              ma[3] * ma[4] * ma[13] -
              ma[3] * ma[12] * ma[5];

    result[3][2] = -ma[0]  * ma[5] * ma[11] +
              ma[0]  * ma[9] * ma[7] +
              ma[1]  * ma[4] * ma[11] -
              ma[1]  * ma[8] * ma[7] -
              ma[3] * ma[4] * ma[9] +
              ma[3] * ma[8] * ma[5];

    result[0][3] = -ma[4] * ma[9] * ma[14] +
             ma[4] * ma[13] * ma[10] +
             ma[5] * ma[8] * ma[14] -
             ma[5] * ma[12] * ma[10] -
             ma[6] * ma[8] * ma[13] +
             ma[6] * ma[12] * ma[9];

    result[1][3] = ma[0] * ma[9] * ma[14] -
             ma[0] * ma[13] * ma[10] -
             ma[1] * ma[8] * ma[14] +
             ma[1] * ma[12] * ma[10] +
             ma[2] * ma[8] * ma[13] -
             ma[2] * ma[12] * ma[9];

    result[2][3] = -ma[0] * ma[5] * ma[14] +
              ma[0] * ma[13] * ma[6] +
              ma[1] * ma[4] * ma[14] -
              ma[1] * ma[12] * ma[6] -
              ma[2] * ma[4] * ma[13] +
              ma[2] * ma[12] * ma[5];

    result[3][3] = ma[0] * ma[5] * ma[10] -
              ma[0] * ma[9] * ma[6] -
              ma[1] * ma[4] * ma[10] +
              ma[1] * ma[8] * ma[6] +
              ma[2] * ma[4] * ma[9] -
              ma[2] * ma[8] * ma[5];

    det = ma[0] * result[0][0] + ma[4] * result[0][1] + ma[8] * result[0][2] + ma[12] * result[0][3];

    if (det == 0)
        throw std::runtime_error("Inverse matrix is undefined for matrices with determinant 0");

    det = 1.0f / det;

    result = result * det;

    return result;
}

mat4x4 transpose(const mat4x4& mat){
    mat4x4 res;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            res[i][j] = mat[j][i];
        }
    }
    return res;
}

mat4x4 operator*(const float a, const mat4x4& mat){
    return operator*(mat, a);
}

mat4x4 operator*(const mat4x4& mat, const float a){
    mat4x4 result;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            result[i][j] = mat[i][j] * a;
        }
    }
    return result;
}

}
