/*
 * mat.h
 *
 *  Created on: 30.04.2019
 *      Author: florian
 */

#pragma once

#include "../core/Core.h"

#include "vec.h"

namespace Zoe {

class mat2x2;

class mat3x3;

class mat4x4;

/**
 * A 2 by 2 matrix. The vales can be access by the [] operator. It returns a vector which has [] operator. The first [] is the y coordinate of the cell.
 * This pattern can be used to assign the matrix:\n
 * m[0][0] = 1; m[0][1] = 0;\n
 * m[1][0] = 0; m[1][1] = 1;
 */
class DLL_PUBLIC mat2x2 {
public:

    /**
     * Creates an empty 2 by 2 matrix.
     */
    mat2x2();

    /**
     * Creates a new 2 by 2 matrix. Copies the top left corner of the specified matrix.
     * @param mat the specified matrix
     */
    explicit mat2x2(const mat3x3& mat);

    /**
     * Creates a new 2 by 2 matrix. Copies the top left corner of the specified matrix.
     * @param mat the specified matrix
     */
    explicit mat2x2(const mat4x4& mat);

    /**
     * Adds this matrix element by element to the specified matrix and returns the result.
     * @param mat the specified matrix
     * @returns the result
     */
    mat2x2 operator+(const mat2x2& mat) const;

    /**
     * Subtracts this matrix element by element to the specified matrix and returns the result.
     * @param mat the specified matrix
     * @returns the result
     */
    mat2x2 operator-(const mat2x2& mat) const;

    /**
     * Multiplies this matrix and the specified vector using matrix vector multiplication.
     * @param vec the specified vector
     * @returns the result
     */
    vec2 operator*(const vec2& vec) const;

    /**
     * Multiplies this matrix and the specified matrix using matrix multiplication.
     * @param mat the specified matrix
     * @returns the result
     */
    mat2x2 operator*(const mat2x2& mat) const;

    /**
     * Access the row of this matrix. The index must be 0 or 1. The returned vector also has an operator[] to access the element of this matrix.
     * @param index the index of the row
     * @returns the row as a vector
     */
    vec2 operator[](const int& index) const;

    /**
     * Access the row of this matrix. The index must be 0 or 1. The returned vector also has an operator[] to access the element of this matrix.
     * @param index the index of the row
     * @returns a reference to the row as a vector
     */
    vec2& operator[](const int& index);

    /**
     * Checks if this matrix equals the specified matrix.
     * @param rhs the specified matrix
     * @returns `true` if every element is equal
     */
    bool operator==(const mat2x2& rhs) const;

    /**
	 * Checks if this matrix does not equal the specified matrix.
	 * @param rhs the specified matrix
	 * @returns `true` if any element is different
	 */
    bool operator!=(const mat2x2& rhs) const;

    /**
     * Calculates the inverse of this matrix. Throws an exception if the determinant is 0.
     * @returns the inverse matrix
     */
    mat2x2 inverse() const;

    /**
     * Calculates the transpose of this matrix.
     * @return the transposed matrix
     */
    mat2x2 transpose() const;

    /**
     * Calculates the determinant of this matrix.
     * @return the determinant
     */
    float determinant() const;

    /**
     * Returns the 2 by 2 identity matrix.
     * The 2 by 2 identity matrix is:\n
     * ```
     * 1 0
     * 0 1
     * ```
     * @returns the 2 by 2 identity matrix
     */
    static const mat2x2& identity();
private:
    vec2 data[2]{};
};
/**
 * Prints the specified 2 by 2 matrix to the specified stream.
 * @param stream the specified stream.
 * @param v the specified matrix
 * @returns the stream
 */
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream, const mat2x2& v);

/**
 * Multiplies the specified matrix and the specified float element by element.
 * @param mat the specified matrix
 * @param a the specified float
 * @returns the result
 */
DLL_PUBLIC mat2x2 operator*(const mat2x2& mat, float a);
/**
 * Multiplies the specified matrix and the specified float element by element.
 * @param a the specified float
 * @param mat the specified matrix
 * @returns the result
 */
DLL_PUBLIC mat2x2 operator*(float a, const mat2x2& mat);

/**
 * A 3 by 3 matrix. The vales can be access by the [] operator. It returns a vector which has [] operator. The first [] is the y coordinate of the cell.
 * This pattern can be used to assign the matrix:\n
 * m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;\n
 * m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;\n
 * m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
 */
class DLL_PUBLIC mat3x3 {
public:

    /**
     * Creates an empty 3 by 3 matrix.
     */
    mat3x3();

    /**
     * Creates a new 3 by 3 matrix. Copies the specified matrix in the top left corner. The rest is filled like the 3 by 3 identity matrix is.
     * @param mat the specified matrix
     */
    explicit mat3x3(const mat2x2& mat);

    /**
     * Creates a new 3 by 3 matrix. Copies the top left corner of the specified matrix.
     * @param mat the specified matrix
     */
    explicit mat3x3(const mat4x4& mat);

    /**
     * Adds this matrix element by element to the specified matrix and returns the result.
     * @param mat the specified matrix
     * @returns the result
     */
    mat3x3 operator+(const mat3x3& mat) const;

    /**
     * Subtracts this matrix element by element to the specified matrix and returns the result.
     * @param mat the specified matrix
     * @returns the result
     */
    mat3x3 operator-(const mat3x3& mat) const;

    /**
     * Multiplies this matrix and the specified vector using matrix vector multiplication.
     * @param vec the specified vector
     * @returns the result
     */
    vec3 operator*(const vec3& vec) const;

    /**
     * Multiplies this matrix and the specified matrix using matrix multiplication.
     * @param mat the specified matrix
     * @returns the result
     */
    mat3x3 operator*(const mat3x3& rhs) const;

    /**
     * Access the row of this matrix. The index must be 0 or 1. The returned vector also has an operator[] to access the element of this matrix.
     * @param index the index of the row
     * @returns the row as a vector
     */
    vec3 operator[](const int& index) const;

    /**
     * Access the row of this matrix. The index must be 0 or 1. The returned vector also has an operator[] to access the element of this matrix.
     * @param index the index of the row
     * @returns a reference to the row as a vector
     */
    vec3& operator[](const int& index);

    /**
     * Checks if this matrix equals the specified matrix.
     * @param rhs the specified matrix
     * @returns `true` if every element is equal
     */
    bool operator==(const mat3x3& rhs) const;

    /**
	 * Checks if this matrix does not equal the specified matrix.
	 * @param rhs the specified matrix
	 * @returns `true` if any element is different
	 */
    bool operator!=(const mat3x3& rhs) const;

    /**
     * Calculates the inverse of this matrix. Throws an exception if the determinant is 0.
     * @returns the inverse matrix
     */
    mat3x3 inverse() const;

    /**
    * Calculates the transpose of this matrix.
    * @return the transposed matrix
    */
    mat3x3 transpose() const;

    /**
     * Calculates the determinant of this matrix.
     * @return the determinant
     */
    float determinant() const;

    /**
     * Returns the 3 by 3 identity matrix.
     * The 3 by 3 identity matrix is:\n
     * ```
     * 1 0 0
     * 0 1 0
     * 0 0 1
     * ```
     * @returns the 3 by 3 identity matrix
     */
    static const mat3x3& identity();
private:
    vec3 data[3]{};
};

/**
 * Prints the specified 3 by 3 matrix to the specified stream.
 * @param stream the specified stream.
 * @param v the specified matrix
 * @returns the stream
 */
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream, const mat3x3& v);

/**
 * Returns a rotation matrix for 2D space by the specified angle in radians.
 * @param angle the specified angle
 * @returns the rotation matrix
 */
DLL_PUBLIC mat3x3 rotate2D(float angle);

/**
 * Returns a translation matrix for 2D space by the specified offset.
 * @param x the x offset
 * @param y the y offset
 * @return the translation matrix
 */
DLL_PUBLIC mat3x3 translate2D(float x, float y);

/**
 * Returns a scaling matrix for 2D space by the specified values in the x and y direction.
 * @param scaleX the x scale factor
 * @param scaleY the y scale factor
 * @returns the scaling matrix
 */
DLL_PUBLIC mat3x3 scale2D(float scaleX, float scaleY);

/**
 * Multiplies the specified matrix and the specified float element by element.
 * @param mat the specified matrix
 * @param a the specified float
 * @returns the result
 */
DLL_PUBLIC mat3x3 operator*(const mat3x3& mat, float a);

/**
 * Multiplies the specified matrix and the specified float element by element.
 * @param a the specified float
 * @param mat the specified matrix
 * @returns the result
 */
DLL_PUBLIC mat3x3 operator*(float a, const mat3x3& mat);

/**
 * A43 by 4 matrix. The vales can be access by the [] operator. It returns a vector which has [] operator. The first [] is the y coordinate of the cell.
 * This pattern can be used to assign the matrix:\n
 * m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;\n
 * m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;\n
 * m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;\n
 * m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
 */
class DLL_PUBLIC mat4x4 {
public:
    /**
     * Creates an empty 4 by 4 matrix.
     */
    mat4x4();

    /**
     * Creates a new 4 by 4 matrix. Copies the specified matrix in the top left corner. The rest is filled like the 4 by 4 identity matrix is.
     * @param mat the specified matrix
     */
    explicit mat4x4(const mat2x2& mat);

    /**
     * Creates a new 4 by 4 matrix. Copies the specified matrix in the top left corner. The rest is filled like the 4 by 4 identity matrix is.
     * @param mat the specified matrix
     */
    explicit mat4x4(const mat3x3& mat);

    /**
     * Adds this matrix element by element to the specified matrix and returns the result.
     * @param mat the specified matrix
     * @returns the result
     */
    mat4x4 operator+(const mat4x4& mat) const;

    /**
     * Subtracts this matrix element by element to the specified matrix and returns the result.
     * @param mat the specified matrix
     * @returns the result
     */
    mat4x4 operator-(const mat4x4& mat) const;

    /**
     * Multiplies this matrix and the specified vector using matrix vector multiplication.
     * @param vec the specified vector
     * @returns the result
     */
    vec4 operator*(const vec4& vec) const;

    /**
     * Multiplies this matrix and the specified matrix using matrix multiplication.
     * @param mat the specified matrix
     * @returns the result
     */
    mat4x4 operator*(const mat4x4& mat) const;

    /**
     * Access the row of this matrix. The index must be 0 or 1. The returned vector also has an operator[] to access the element of this matrix.
     * @param index the index of the row
     * @returns the row as a vector
     */
    vec4 operator[](const int& index) const;

    /**
     * Access the row of this matrix. The index must be 0 or 1. The returned vector also has an operator[] to access the element of this matrix.
     * @param index the index of the row
     * @returns a reference to the row as a vector
     */
    vec4& operator[](const int& index);

    /**
     * Checks if this matrix equals the specified matrix.
     * @param rhs the specified matrix
     * @returns `true` if every element is equal
     */
    bool operator==(const mat4x4& rhs) const;

    /**
	 * Checks if this matrix does not equal the specified matrix.
	 * @param rhs the specified matrix
	 * @returns `true` if any element is different
	 */
    bool operator!=(const mat4x4& rhs) const;

    /**
     * Calculates the inverse of this matrix. Throws an exception if the determinant is 0.
     * @returns the inverse matrix
     */
    mat4x4 inverse() const;

    /**
    * Calculates the transpose of this matrix.
    * @return the transposed matrix
    */
    mat4x4 transpose() const;

    /**
     * Calculates the determinant of this matrix.
     * @return the determinant
     */
    float determinant() const;

    /**
     * Returns the 4 by 4 identity matrix.
     * The 4 by 4 identity matrix is:\n
     * ```
     * 1 0 0 0
     * 0 1 0 0
     * 0 0 1 0
     * 0 0 0 1
     * ```
     * @returns the 4 by 4 identity matrix
     */
    static const mat4x4& identity();
private:
    vec4 data[4]{};
};

/**
 * Prints the specified 4 by 4 matrix to the specified stream.
 * @param stream the specified stream.
 * @param v the specified matrix
 * @returns the stream
 */
DLL_PUBLIC std::ostream& operator<<(std::ostream& stream, const mat4x4& v);

/**
 * Returns a rotation around the Z-axis matrix for 3D space by the specified angle in radians.
 * @param angle the specified angle
 * @returns the rotation matrix
 */
DLL_PUBLIC mat4x4 rotateXY3D(float angle);

/**
 * Returns a rotation around the X-axis matrix for 3D space by the specified angle in radians.
 * @param angle the specified angle
 * @returns the rotation matrix
 */
DLL_PUBLIC mat4x4 rotateYZ3D(float angle);

/**
 * Returns a rotation around the Y-axis matrix for 3D space by the specified angle in radians.
 * @param angle the specified angle
 * @returns the rotation matrix
 */
DLL_PUBLIC mat4x4 rotateXZ3D(float angle);

/**
 * Returns a translation matrix for 3D space by the specified offset.
 * @param x the x offset
 * @param y the y offset
 * @param z the z offset
 * @return the translation matrix
 */
DLL_PUBLIC mat4x4 translate3D(float x, float y, float z);

/**
 * Returns a scaling matrix for 3D space by the specified values in the x, y and z direction.
 * @param scaleX the x scale factor
 * @param scaleY the y scale factor
 * @param scaleZ the z scale factor
 * @returns the scaling matrix
 */
DLL_PUBLIC mat4x4 scale3D(float scaleX, float scaleY, float scaleZ);

/**
 * Calculates an orthographic projection matrix, also known as a perspective projection matrix, with the specified limits.
 * @param left The leftmost value in the resulting area
 * @param top The highest value that is in the resulting range
 * @param right the rightmost value that is in the resulting space
 * @param bottom The lowest value that is in the resulting range
 * @param near the nearest value found in the resulting space
 * @param far the furthest value that is in the resulting space
 * @returns the resulting orthographic projection matrix
 */
DLL_PUBLIC mat4x4 orthographic(float left, float top, float right, float bottom, float near, float far);

/**
 * Calculates the view matrix by the specified position and rotation.
 * @param position the specified position
 * @param rotation the specified rotation
 * @returns the resulting view matrix
 */
DLL_PUBLIC mat4x4 calcViewMatrix(vec3 position, vec3 rotation);

/**
 * Calculates the perspective projection matrix.
 * @param near the nearest possible value
 * @param far the furthest possible value
 * @param fov the field of view in degree
 * @param aspectRatio the ratio from width to height
 * @returns the resulting perspective projection matrix
 */
DLL_PUBLIC mat4x4 perspective(float near, float far, float fov, float aspectRatio);

/**
 * Multiplies the specified matrix and the specified float element by element.
 * @param mat the specified matrix
 * @param a the specified float
 * @returns the result
 */
DLL_PUBLIC mat4x4 operator*(const mat4x4& mat, float a);

/**
 * Multiplies the specified matrix and the specified float element by element.
 * @param a the specified float
 * @param mat the specified matrix
 * @returns the result
 */
DLL_PUBLIC mat4x4 operator*(float a, const mat4x4& mat);

}
