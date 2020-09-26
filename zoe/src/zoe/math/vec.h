/*
 * vec.h
 *
 *  Created on: 21.04.2019
 *      Author: florian
 */

#pragma once

#include <ostream>
#include "../core/Core.h"

namespace Zoe {

class vec2;
class vec3;
class vec4;

/**
 * A 2 dimensional vector.
 * @see https://en.wikipedia.org/wiki/Vector_(mathematics_and_physics)
 */
class vec2 {
public:
    float x, y;
public:
    /**
     * Creates a new null vector.
     */
    vec2();

    /**
     * Creates a new vector with the specified values.
     * @param x the specified x value
     * @param y the specified y value
     */
    vec2(float x, float y);

    /**
     * Creates a new vector with the values in the specified vector.
     * @param vec the specified vector
     */
    vec2(const vec3& vec);

    /**
     * Creates a new vector with the values in the specified vector.
     * @param vec the specified vector
     */
    vec2(const vec4& vec);

    /**
     * Adds this vector element by element to the specified vector and returns the result.
     * @param v the specified vector
     * @returns the result
     */
    vec2 operator+(const vec2& v) const;

    /**
     * Subtracts this vector element by element from the specified vector and returns the result.
     * @param v the specified vector
     * @returns the result
     */
    vec2 operator-(const vec2& v) const;

    /**
     * Calculates the scalar product of the specified and this vector.
     * @param mat the specified vector
     * @returns the result
     */
    float operator*(const vec2& v) const;

    /**
     * Scales this vector by the specified factor.
     * @param m the specified factor
     * @returns the result
     */
    vec2 operator*(const float& m) const;

    /**
     * Scales this vector by the multiplicative inverse of the specified factor.
     * @param d the specified factor
     * @returns the result
     */
    vec2 operator/(const float& d) const;

    /**
     * Access an element of this vector by its index.
     * @param index the specified index
     * @returns the value
     */
    float operator[](const int& index) const;

    /**
     * Access an element of this vector by its index.
     * @param index the specified index
     * @returns a reference to the value
     */
    float& operator[](const int& index);

    /**
     * Returns a normalized version of this vector. Its length is guaranteed to be 1.
     * @returns the normalized vector
     */
    vec2 normalize() const;

    /**
     * Returns the length, also known as magnitude, of this vector.
     * @returns the length
     */
    float length() const;

    /**
     * Checks if this vector equals the specified vector.
     * @param vec the specified vector
     * @returns `true` if every element is equal
     */
    bool operator==(const vec2& vec) const;

    /**
	 * Checks if this vector does not equal the specified vector.
	 * @param vec the specified vector
	 * @returns `true` if any element is different
	 */
    bool operator!=(const vec2& vec) const;
};

/**
 * Prints the specified vector to the specified stream.
 * @param stream the specified stream.
 * @param v the specified vector
 * @returns the stream
 */
std::ostream& operator<<(std::ostream& stream, const vec2& v);

 /**
  * Scales the specified vector by the specified factor.
  * @param m the specified factor
  * @param v the specified vector
  * @returns the result
  */
vec2 operator*(const float& m, const vec2& v);

/**
 * A 3 dimensional vector.
 * @see https://en.wikipedia.org/wiki/Vector_(mathematics_and_physics)
 */
class vec3 {
public:
    float x, y, z;
public:

    /**
     * Creates a new null vector.
     */
    vec3();

    /**
     * Creates a new vector with the specified values.
     * @param x the specified x value
     * @param y the specified y value
     * @param z the specified z value
     */
    vec3(float x, float y, float z);

    /**
     * Creates a new vector with the values in the specified vector.
     * @param vec the specified vector
     */
    vec3(const vec2& vec);

    /**
     * Creates a new vector with the values in the specified vector.
     * @param vec the specified vector
     */
    vec3(const vec4& vec);

    /**
     * Access the x and y member as a vec2.
     * @return a reference to xy as a vec2
     */
    inline vec2& xy() { return reinterpret_cast<vec2&>(x); }

    /**
     * Access the y and z member as a vec2.
     * @return a reference to yz as a vec2
     */
    inline vec2& yz() { return reinterpret_cast<vec2&>(y); }

    /**
     * Access the x and y member as a vec2.
     * @return a copy of x and y as a vec2
     */
    inline const vec2& xy() const { return reinterpret_cast<const vec2&>(x); }

    /**
     * Access the y and z member as a vec2.
     * @return a copy of y and z as a vec2
     */
    inline const vec2& yz() const { return reinterpret_cast<const vec2&>(y); }

    /**
     * Adds this vector element by element to the specified vector and returns the result.
     * @param v the specified vector
     * @returns the result
     */
    vec3 operator+(const vec3& v) const;

    /**
     * Subtracts this vector element by element from the specified vector and returns the result.
     * @param v the specified vector
     * @returns the result
     */
    vec3 operator-(const vec3& v) const;

    /**
     * Calculates the scalar product of the specified and this vector.
     * @param mat the specified vector
     * @returns the result
     */
    float operator*(const vec3& v) const;

    /**
     * Scales this vector by the specified factor.
     * @param m the specified factor
     * @returns the result
     */
    vec3 operator*(const float& m) const;

    /**
     * Scales this vector by the multiplicative inverse of the specified factor.
     * @param d the specified factor
     * @returns the result
     */
    vec3 operator/(const float& d) const;

    /**
     * Calculates the cross product of the specified an this vector.
     * @param v the specified vector
     * @returns the result
     */
    vec3 crossProduct(const vec3& v) const;

    /**
     * Access an element of this vector by its index.
     * @param index the specified index
     * @returns the value
     */
    float operator[](const int& index) const;

    /**
     * Access an element of this vector by its index.
     * @param index the specified index
     * @returns a reference to the value
     */
    float& operator[](const int& index);

    /**
     * Returns a normalized version of this vector. Its length is guaranteed to be 1.
     * @returns the normalized vector
     */
    vec3 normalize() const;

    /**
     * Returns the length, also known as magnitude, of this vector.
     * @returns the length
     */
    float length() const;

    /**
     * Checks if this vector equals the specified vector.
     * @param vec the specified vector
     * @returns `true` if every element is equal
     */
    bool operator==(const vec3& vec) const;

    /**
	 * Checks if this vector does not equal the specified vector.
	 * @param vec the specified vector
	 * @returns `true` if any element is different
	 */
    bool operator!=(const vec3& vec) const;
};

/**
 * Prints the specified vector to the specified stream.
 * @param stream the specified stream.
 * @param v the specified vector
 * @returns the stream
 */
std::ostream& operator<<(std::ostream& stream, const vec3& v);
vec3 operator*(const float& m, const vec3& v);

/**
 * A 4 dimensional vector.
 * @see https://en.wikipedia.org/wiki/Vector_(mathematics_and_physics)
 */
class vec4 {
public:
    float x, y, z, w;
public:

    /**
     * Creates a new null vector.
     */
    vec4();

    /**
     * Creates a new vector with the specified values.
     * @param x the specified x value
     * @param y the specified y value
     * @param z the specified z value
     * @param w the specified w value
     */
    vec4(float x, float y, float z, float w);

    /**
     * Creates a new vector with the values in the specified vector.
     * @param vec the specified vector
     */
    vec4(const vec2& vec);

    /**
     * Creates a new vector with the values in the specified vector.
     * @param vec the specified vector
     */
    vec4(const vec3& vec);

    /**
     * Access the x and y member as a vec2.
     * @return a reference to xy as a vec2
     */
    inline vec2& xy() { return reinterpret_cast<vec2&>(x); }

    /**
     * Access the y and z member as a vec2.
     * @return a reference to yz as a vec2
     */
    inline vec2& yz() { return reinterpret_cast<vec2&>(y); }

    /**
     * Access the z and w member as a vec2.
     * @return a reference to zw as a vec2
     */
    inline vec2& zw() { return reinterpret_cast<vec2&>(z); }

    /**
     * Access the x, y and z member as a vec3.
     * @return a reference to xyz as a vec3
     */
    inline vec3& xyz() { return reinterpret_cast<vec3&>(x); }

    /**
     * Access the y, z and w member as a vec3.
     * @return a reference to yzw as a vec3
     */
    inline vec3& yzw() { return reinterpret_cast<vec3&>(y); }

    /**
     * Access the x and y member as a vec2.
     * @return a copy of x and y as a vec2
     */
    inline const vec2& xy() const { return reinterpret_cast<const vec2&>(x); }

    /**
     * Access the y and z member as a vec2.
     * @return a copy of y and z as a vec2
     */
    inline const vec2& yz() const { return reinterpret_cast<const vec2&>(y); }

    /**
     * Access the z and w member as a vec2.
     * @return a copy of z and w as a vec2
     */
    inline const vec2& zw() const { return reinterpret_cast<const vec2&>(z); }

    /**
     * Access the x, y and z member as a vec3.
     * @return a copy of x, y and z as a vec3
     */
    inline const vec3& xyz() const { return reinterpret_cast<const vec3&>(x); }

    /**
     * Access the y, z and w member as a vec3.
     * @return a copy of y, z and w as a vec3
     */
    inline const vec3& yzw() const { return reinterpret_cast<const vec3&>(y); }

    /**
     * Adds this vector element by element to the specified vector and returns the result.
     * @param v the specified vector
     * @returns the result
     */
    vec4 operator+(const vec4& v) const;

    /**
     * Subtracts this vector element by element from the specified vector and returns the result.
     * @param v the specified vector
     * @returns the result
     */
    vec4 operator-(const vec4& v) const;

    /**
     * Calculates the scalar product of the specified and this vector.
     * @param mat the specified vector
     * @returns the result
     */
    float operator*(const vec4& v) const;

    /**
     * Scales this vector by the specified factor.
     * @param m the specified factor
     * @returns the result
     */
    vec4 operator*(const float& m) const;

    /**
     * Scales this vector by the multiplicative inverse of the specified factor.
     * @param d the specified factor
     * @returns the result
     */
    vec4 operator/(const float& d) const;

    /**
     * Calculates the cross product of the two specified and this vector.
     * @param v1 the first specified vector
     * @param v2 the second specified vector
     * @returns the result
     */
    vec4 crossProduct(const vec4& v1, const vec4& v2) const;

    /**
     * Access an element of this vector by its index.
     * @param index the specified index
     * @returns the value
     */
    float operator[](const int& index) const;

    /**
     * Access an element of this vector by its index.
     * @param index the specified index
     * @returns a reference to the value
     */
    float& operator[](const int& index);

    /**
     * Returns a normalized version of this vector. Its length is guaranteed to be 1.
     * @returns the normalized vector
     */
    vec4 normalize() const;

    /**
     * Returns the length, also known as magnitude, of this vector.
     * @returns the length
     */
    float length() const;

    /**
     * Checks if this vector equals the specified vector.
     * @param vec the specified vector
     * @returns `true` if every element is equal
     */
    bool operator==(const vec4& vec) const;

    /**
	 * Checks if this vector does not equal the specified vector.
	 * @param vec the specified vector
	 * @returns `true` if any element is different
	 */
    bool operator!=(const vec4& vec) const;
};

/**
 * Prints the specified vector to the specified stream.
 * @param stream the specified stream.
 * @param v the specified vector
 * @returns the stream
 */
std::ostream& operator<<(std::ostream& stream, const vec4& v);

/**
 * Scales the specified vector by the specified factor.
 * @param m the specified factor
 * @param v the specified vector
 * @returns the result
 */
vec4 operator*(const float& m, const vec4& v);

}
