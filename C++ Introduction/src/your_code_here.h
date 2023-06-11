#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <span>
#include <tuple>
#include <vector>

// ==================================
// ========    Exercise 1    ========
// ==================================
std::pair<float, float> statistics(std::span<const float> values)
{
    // Your solution goes here
    if (values.empty())
        return { 0, 0 };
    int i = 0;
    float avg = 0;
    for (float x : values) {
        i++;
        avg = avg + x;
    }
    avg = avg / i;
    float s = 0;
    for (float y : values) {
        s = s + ((y - avg) * (y - avg));         
    }
    s = s / i;
    s = sqrt(s);
    return {avg, s};
}

// ==================================
// ========    Exercise 2    ========
// ==================================
struct Tree {
    float value;
    std::vector<Tree> children;
};
// Your implementation goes here. Feel free to define helper functions/structs/classes in this
// file if you need to. Make sure to put them above this function to prevent linker warnings.
float countTree(const Tree& tree, bool countOnlyEvenLevels)
{
    // Your solution goes here
    float result = tree.value;
    if (tree.children.empty()) {
        return result;
    } else {
        if (countOnlyEvenLevels) {
            for (Tree child : tree.children) {
                if (child.children.empty()) {
                    break;
                } else {
                    for (Tree grandchild : child.children) {
                        result = result + countTree(grandchild, true);
                    }
                }
            }
        } else {
            for (Tree child : tree.children) {
                result = result + countTree(child, false);
            } 
        }
    }
    return result;
}

// =================================================
// ========    Vector & Matrix Exercises    ========
// =================================================
// 
// !!! DO NOT MODIFY THIS STRUCT !!!
struct Vector3 {
    float x, y, z;
};

// Basic operations of a 3D vector (addition, subtraction, multiplication and division)
Vector3 add(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
Vector3 sub(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
Vector3 mul(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}
Vector3 div(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 { (lhs.x / rhs.x), (lhs.y / rhs.y), (lhs.z / rhs.z) };
}

// Dot product, cross product and vector length.
// If you need a Linear Algebra refresher, please check out:
// https://www.tudelft.nl/en/eemcs/study/online-education/math-explained/vectors/
float dot(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}
Vector3 cross(const Vector3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    return Vector3 { (lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z), (lhs.x * rhs.y) - (lhs.y * rhs.x) };
}
float length(const Vector3& lhs)
{
    return sqrt(((lhs.x) * (lhs.x)) + ((lhs.y) * (lhs.y)) + ((lhs.z) * (lhs.z)));
}

// The matrix is defined as:
// | m00  m01  m02 |
// | m10  m11  m12 |
// | m20  m21  m22 |
//
// The columns are stored as Vector3's, where:
// col1 = (m00, m10, m20)
// col2 = (m01, m11, m21)
// col3 = (m02, m12, m22)
//
// !!! DO NOT MODIFY THIS STRUCT !!!
struct Matrix3 {
    Vector3 col1, col2, col3;
};

// Element-wise operations of a 3x3 matrix (addition and subtraction)
Matrix3 add(const Matrix3& lhs, const Matrix3& rhs)
{
    // Your solution goes here
    return Matrix3 { add(lhs.col1, rhs.col1), add(lhs.col2, rhs.col2), add(lhs.col3, rhs.col3) };
}
Matrix3 sub(const Matrix3& lhs, const Matrix3& rhs)
{
    // Your solution goes here
    return Matrix3 { sub(lhs.col1, rhs.col1), sub(lhs.col2, rhs.col2), sub(lhs.col3, rhs.col3) };
}

// Matrix multiplication with a scalar, a vector, and another matrix.
Matrix3 mul(const Matrix3& lhs, float rhs)
{
    // Your solution goes here
    Vector3 v1 = { lhs.col1.x * rhs, lhs.col1.y * rhs, lhs.col1.z * rhs };
    Vector3 v2 = { lhs.col2.x * rhs, lhs.col2.y * rhs, lhs.col2.z * rhs };
    Vector3 v3 = { lhs.col3.x * rhs, lhs.col3.y * rhs, lhs.col3.z * rhs };

    return Matrix3 {v1, v2, v3};
}
Vector3 mul(const Matrix3& lhs, const Vector3& rhs)
{
    // Your solution goes here
    float x = (lhs.col1.x * rhs.x) + (lhs.col2.x * rhs.y) + (lhs.col3.x * rhs.z);
    float y = (lhs.col1.y * rhs.x) + (lhs.col2.y * rhs.y) + (lhs.col3.y * rhs.z);
    float z = (lhs.col1.z * rhs.x) + (lhs.col2.z * rhs.y) + (lhs.col3.z * rhs.z);
    
    return Vector3 {x, y, z};
}
Matrix3 mul(const Matrix3& lhs, const Matrix3& rhs)
{
    // Your solution goes here
    Vector3 v1 = { (lhs.col1.x * rhs.col1.x) + (lhs.col2.x * rhs.col1.y) + (lhs.col3.x * rhs.col1.z),
        (lhs.col1.y * rhs.col1.x) + (lhs.col2.y * rhs.col1.y) + (lhs.col3.y * rhs.col1.z),
        (lhs.col1.z * rhs.col1.x) + (lhs.col2.z * rhs.col1.y) + (lhs.col3.z * rhs.col1.z) };

    Vector3 v2 = { (lhs.col1.x * rhs.col2.x) + (lhs.col2.x * rhs.col2.y) + (lhs.col3.x * rhs.col2.z), 
        (lhs.col1.y * rhs.col2.x) + (lhs.col2.y * rhs.col2.y) + (lhs.col3.y * rhs.col2.z),
        (lhs.col1.z * rhs.col2.x) + (lhs.col2.z * rhs.col2.y) + (lhs.col3.z * rhs.col2.z)
    };

    Vector3 v3 = { (lhs.col1.x * rhs.col3.x) + (lhs.col2.x * rhs.col3.y) + (lhs.col3.x * rhs.col3.z) ,
        (lhs.col1.y * rhs.col3.x) + (lhs.col2.y * rhs.col3.y) + (lhs.col3.y * rhs.col3.z), 
        (lhs.col1.z * rhs.col3.x) + (lhs.col2.z * rhs.col3.y) + (lhs.col3.z * rhs.col3.z)
    };

    return Matrix3 { v1, v2, v3 };
}

// Taking the transpose of a matrix means changing it's columns into rows and vice versa.
// Following our previous definition, the transpose of our matrix should look like:
// | m00  m10  m20 |
// | m01  m11  m21 |
// | m02  m12  m22 |
Matrix3 transpose(const Matrix3& m)
{
    // Your solution goes here
    Vector3 v1 = { m.col1.x, m.col2.x, m.col3.x };
    Vector3 v2 = { m.col1.y, m.col2.y, m.col3.y };
    Vector3 v3 = { m.col1.z, m.col2.z, m.col3.z };

    return Matrix3 {v1, v2, v3};
}

// The determinant is needed to compute the inverse of a matrix.
// If you need a Linear Algebra refresher, please check out:
// https://www.tudelft.nl/en/eemcs/study/online-education/math-explained/linear-algebra/#c144161
float determinant(const Matrix3& m)
{
    // Your solution goes here
    float a = m.col1.x * ((m.col2.y * m.col3.z) - (m.col3.y * m.col2.z)); 
    float b = m.col2.x * ((m.col1.y * m.col3.z) - (m.col3.y * m.col1.z));
    float c = m.col3.x * ((m.col1.y * m.col2.z) - (m.col2.y * m.col1.z));
    return a - b + c;
}

// Computing the inverse of the given matrix. If you implemented it correctly then matrix M multiplied
// by its inverse should give the identity matrix). More information on how to compute the inverse of a
// 3x3 matrix can be found here:
// https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
Matrix3 inverse(const Matrix3& m)
{
    // Your solution goes here
    float a = (m.col2.y * m.col3.z) - (m.col3.y * m.col2.z);
    float b = (m.col1.y * m.col3.z) - (m.col3.y * m.col1.z);
    float c = (m.col1.y * m.col2.z) - (m.col2.y * m.col1.z);
    float d = (m.col2.x * m.col3.z) - (m.col3.x * m.col2.z);
    float e = (m.col1.x * m.col3.z) - (m.col3.x * m.col1.z);
    float f = (m.col1.x * m.col2.z) - (m.col2.x * m.col1.z);
    float g = (m.col2.x * m.col3.y) - (m.col3.x * m.col2.y);
    float h = (m.col1.x * m.col3.y) - (m.col3.x * m.col1.y);
    float i = (m.col1.x * m.col2.y) - (m.col2.x * m.col1.y);

    Vector3 v1 = { a, -d, g };
    Vector3 v2 = { -b, e, -h };
    Vector3 v3 = { c, -f, i };

    Matrix3 result = { v1, v2, v3 };
    result = transpose(result);
    float det = determinant(m);
    return mul(result, 1 / det);
}
