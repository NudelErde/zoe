/*
 * mat.h
 *
 *  Created on: 30.04.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"

#include "vec.h"

namespace Zoe{

class DLL_PUBLIC mat2x2{
public:
	mat2x2();
	~mat2x2();
	mat2x2 operator+(const mat2x2& mat) const;
	mat2x2 operator-(const mat2x2& mat) const;
	vec2 operator*(const vec2& vec) const;
	mat2x2 operator*(const mat2x2& mat) const;

	vec2 operator[](const int& index )const;
	vec2& operator[](const int& index );

    friend mat2x2 operator*(const mat2x2& mat, float a);
    friend mat2x2 operator*(float a, const mat2x2& mat);
private:
	vec2 data[2];
};
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream,const mat2x2& v);

DLL_PUBLIC mat2x2 operator*(const mat2x2& mat, float a);
DLL_PUBLIC mat2x2 operator*(float a, const mat2x2& mat);

class DLL_PUBLIC mat3x3{
public:
	mat3x3();
	mat3x3(const mat2x2& mat);
	~mat3x3();
	mat3x3 operator+(const mat3x3& mat) const;
	mat3x3 operator-(const mat3x3& mat) const;
	vec3 operator*(const vec3& vec) const;
	mat3x3 operator*(const mat3x3& mat) const;

	vec3 operator[](const int& index )const;
	vec3& operator[](const int& index );

    friend mat3x3 operator*(const mat3x3& mat, float a);
    friend mat3x3 operator*(float a, const mat3x3& mat);
private:
	vec3 data[3];
};
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream,const mat3x3& v);

DLL_PUBLIC mat3x3 rotate2D(float angle);
DLL_PUBLIC mat3x3 translate2D(float x,float y);
DLL_PUBLIC mat3x3 scale2D(float scaleX,float scaleY);

DLL_PUBLIC mat3x3 operator*(const mat3x3& mat, float a);
DLL_PUBLIC mat3x3 operator*(float a, const mat3x3& mat);

class DLL_PUBLIC mat4x4{
public:
	mat4x4();
	mat4x4(const mat3x3& mat);
	~mat4x4();
	mat4x4 operator+(const mat4x4& mat) const;
	mat4x4 operator-(const mat4x4& mat) const;
	vec4 operator*(const vec4& vec) const;
	mat4x4 operator*(const mat4x4& mat) const;

	vec4 operator[](const int& index )const;
	vec4& operator[](const int& index );

    friend mat4x4 operator*(const mat4x4& mat, float a);
    friend mat4x4 operator*(float a, const mat4x4& mat);
private:
	vec4 data[4];
};
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream,const mat4x4& v);

DLL_PUBLIC mat4x4 rotateXY3D(float angle);
DLL_PUBLIC mat4x4 rotateYZ3D(float angle);
DLL_PUBLIC mat4x4 rotateXZ3D(float angle);

DLL_PUBLIC mat4x4 translate3D(float x,float y,float z);
DLL_PUBLIC mat4x4 scale3D(float scaleX,float scaleY,float scaleZ);

DLL_PUBLIC mat4x4 orthographic(float left, float top, float right, float bottom, float near, float far);
DLL_PUBLIC mat4x4 calcViewMatrix(vec3 position, vec3 rotation);
DLL_PUBLIC mat4x4 perspective(float near,float far,float fov,float aspectRatio);

DLL_PUBLIC mat4x4 inverse(const mat4x4& mat);
DLL_PUBLIC mat4x4 transpose(const mat4x4& mat);

DLL_PUBLIC mat4x4 operator*(const mat4x4& mat, float a);
DLL_PUBLIC mat4x4 operator*(float a, const mat4x4& mat);

}
