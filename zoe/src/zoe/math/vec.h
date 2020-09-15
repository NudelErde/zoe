/*
 * vec.h
 *
 *  Created on: 21.04.2019
 *      Author: florian
 */

#pragma once

#include <ostream>
#include "../core/Core.h"

namespace Zoe{

class DLL_PUBLIC vec2{
public:
	float x,y;
public:
	vec2 operator +(const vec2& v)const;
	vec2 operator -(const vec2& v)const;
	float operator *(const vec2& v)const;
	vec2 operator *(const float& m)const;
	vec2 operator /(const float& d)const;
	float operator[](const int& index)const;
	float& operator[](const int& index);
	vec2 normalize()const;
	float length()const;

	bool operator==(const vec2& vec) const;
	bool operator!=(const vec2& vec) const;
};
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream,const vec2& v);
DLL_PUBLIC vec2 operator*(const float& m,const vec2& v);

class DLL_PUBLIC vec3{
public:
	union{
		struct{
			float x,y,z;
		};
		struct{
			vec2 xy;
			float _z;
		};
		struct{
			float _x;
			vec2 yz;
		};
	};
public:
	vec3 operator +(const vec3& v)const;
	vec3 operator -(const vec3& v)const;
	float operator *(const vec3& v)const;
	vec3 operator *(const float& m)const;
	vec3 operator /(const float& d)const;
	vec3 crossProduct(const vec3& v)const;
	float operator[](const int& index)const;
	float& operator[](const int& index);
	vec3 normalize()const;
	float length()const;

	bool operator==(const vec3& vec) const;
	bool operator!=(const vec3& vec) const;
};
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream,const vec3& v);
DLL_PUBLIC vec3 operator*(const float& m,const vec3& v);

class DLL_PUBLIC vec4{
public:
	union{
		struct{
			float x,y,z,w;
		};
		struct{
			vec2 xy,zw;
		};
		struct{
			vec3 xyz;
			float _w;
		};
		struct{
			float _x;
			vec3 yzw;
		};
	};
public:
	vec4 operator +(const vec4& v)const;
	vec4 operator -(const vec4& v)const;
	float operator *(const vec4& v)const;
	vec4 operator *(const float& m)const;
	vec4 operator /(const float& d)const;
	vec4 crossProduct(const vec4& v1,const vec4& v2)const;
	float operator[](const int& index)const;
	float& operator[](const int& index);
	vec4 normalize()const;
	float length()const;

	bool operator==(const vec4& vec) const;
	bool operator!=(const vec4& vec) const;
};
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream,const vec4& v);
DLL_PUBLIC vec4 operator*(const float& m,const vec4& v);

}
