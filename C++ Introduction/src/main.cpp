#include "your_code_here.h"
#include <cassert>
#include <cmath>
#include <iostream>

void fillTree(Tree& tree, int level)
{
    tree.value = 2.0f * static_cast<float>(level);
    if (level >= 4)
        return;

    Tree child;
    fillTree(child, level + 1);

    for (int i = 0; i < level + 2; ++i) {
        tree.children.push_back(child);
    }
}

static constexpr float EPSILON = 10e-3f;
bool equalFloats(float f1, float f2)
{
    return std::abs(f1 - f2) < EPSILON;
}
int main()
{
    // Basic test for exercise 1.
    std::vector<float> stats;
    stats.push_back(-3.0f);
    stats.push_back(-1.0f);
    stats.push_back(1.0f);
    stats.push_back(1.0f);
    stats.push_back(1.0f);
    stats.push_back(3.0f);
    stats.push_back(3.0f);
    stats.push_back(3.0f);

    const std::pair<float, float> statsAnswer = statistics(stats);

    if (equalFloats(statsAnswer.first, 1.0f) && equalFloats(statsAnswer.second, 2.0f)) {
        std::cout << "Exercise 1: Correct for this specific test case" << std::endl;
    } else {
        std::cout << "Exercise 1: Incorrect for this specific test case" << std::endl;
    }

    // Basic test for exericse 2.
    Tree root;
    fillTree(root, 0);
    float visitAnswer1 = countTree(root, false);
    float visitAnswer2 = countTree(root, true);

    if (equalFloats(visitAnswer1, 1132.0f) && equalFloats(visitAnswer2, 984.0f)) {
        std::cout << "Exercise 2: Correct for this specific test case" << std::endl;
    } else {
        std::cout << "Exercise 2: Incorrect for this specific test case" << std::endl;
    }

    // If you want to test your implementation of the linear algebra exercises then you can put them here.
    // Use the equalsFloats function to compare two floating point numbers with an epsilon to prevent tests
    // from failing because of small rounding errors.
    std::cout << "Exercise 3: " << std::endl;
    {
        // Example test for addition...
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 4, 5, 6 };
        const Vector3 c = add(a, b);
        if (c.x == 5 && c.y == 7 && c.z == 9)
            std::cout << "Vector add correct" << std::endl;
        else
            std::cout << "Vector add incorrect" << std::endl;
    }

     
    {
        // test for sub
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 4, 5, 6 };
        const Vector3 c = sub(a, b);
        if (c.x == -3 && c.y == -3 && c.z == -3)
            std::cout << "Vector sub correct" << std::endl;
        else
            std::cout << "Vector sub incorrect" << std::endl;
    }

    {
        // test for mul
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 4, 5, 6 };
        const Vector3 c = mul(a, b);
        if (c.x == 4 && c.y == 10 && c.z == 18)
            std::cout << "Vector mul correct" << std::endl;
        else
            std::cout << "Vector mul incorrect" << std::endl;
    }

    {
        // test for div
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 4, 5, 6 };
        const Vector3 c = div(a, b);
        if (c.x == 0.25f && c.y == 0.4f && c.z == 0.5f) {
            std::cout << "Vector div correct" << std::endl;
        } else
            std::cout << "Vector div incorrect" << std::endl;
              
    }

    std::cout << "Exercise 4: " << std::endl;

    {
        // test for dot
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 1, 5, 7 };
        float c = dot(a, b);
        if (c == 32)
            std::cout << "dot product correct" << std::endl;
        else
            std::cout << "dot product incorrect" << std::endl;
    }

    {
        // test for cross
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 1, 5, 7 };
        const Vector3 c = cross(a, b);
        if (c.x == -1 && c.y == -4 && c.z == 3)
            std::cout << "cross product correct" << std::endl;
        else
            std::cout << "cross product incorrect" << std::endl;
    }

    {

        // test for length
    const Vector3 a { 1, 2, 3 };
    float c = length(a);
    float d = sqrt(14);
    if (c == d)
        std::cout << "length correct" << std::endl;
    else
        std::cout << "length incorrect" << std::endl;
    }

    std::cout << "Exercise 5:" << std::endl;

    {
        // test for addition
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 1, 5, 7 };
        const Vector3 c { 4, 5, 9 };

        const Vector3 aa { 2, 4, 6 };
        const Vector3 bb { 2, 10, 14 };
        const Vector3 cc { 8, 10, 18 };



        const Matrix3 m { a, b, c };

        const Matrix3 result = add(m, m);
        if (result.col1.x == aa.x && result.col1.y == aa.y && result.col1.z == aa.z &&
            result.col2.x == bb.x && result.col2.y == bb.y && result.col2.z == bb.z &&
            result.col3.x == cc.x && result.col3.y == cc.y && result.col3.z == cc.z)
            std::cout << "addition correct" << std::endl;
        else
            std::cout << "addition incorrect" << std::endl;
    }

{
        // test for subtraction
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 1, 5, 7 };
        const Vector3 c { 4, 5, 9 };

        const Vector3 aa { 2, 4, 6 };
        const Vector3 bb { 2, 10, 14 };
        const Vector3 cc { 8, 10, 18 };

        const Matrix3 m { a, b, c };
        const Matrix3 mm { aa, bb, cc };
        const Matrix3 result = sub(mm, m);

        if (result.col1.x == a.x && result.col1.y == a.y && result.col1.z == a.z &&
            result.col2.x == b.x && result.col2.y == b.y && result.col2.z == b.z && 
            result.col3.x == c.x && result.col3.y == c.y && result.col3.z == c.z)
            std::cout << "subtraction correct" << std::endl;
        else
            std::cout << "subtraction incorrect" << std::endl;
    }

    std::cout << "Exercise 6" << std::endl;

    {
        // test for mul1
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 1, 5, 7 };
        const Vector3 c { 4, 5, 9 };

        const Vector3 aa { 2, 4, 6 };
        const Vector3 bb { 2, 10, 14 };
        const Vector3 cc { 8, 10, 18 };

        const Matrix3 m { a, b, c };
        const Matrix3 result = mul(m, 2);

        if (result.col1.x == aa.x && result.col1.y == aa.y && result.col1.z == aa.z && 
            result.col2.x == bb.x && result.col2.y == bb.y && result.col2.z == bb.z && 
            result.col3.x == cc.x && result.col3.y == cc.y && result.col3.z == cc.z)
            std::cout << "mul1 correct" << std::endl;
        else
            std::cout << "mul1 incorrect" << std::endl;
    }


    {
        // test for mul2
        const Vector3 a { 2, 2, 2 };
        const Vector3 b { 3, 8, 5 };
        const Vector3 c { 1, 7, 3 };

        const Vector3 aa { 3, 7, 5 };

        const Matrix3 m { a, b, c };

        const Vector3 result { 32, 97, 56 };
        const Vector3 calc = mul(m, aa);


        if (result.x == calc.x && result.y == calc.y && result.z == calc.z)
            std::cout << "mul2 correct" << std::endl;
        else
            std::cout << "mul2 incorrect" << std::endl;
    }

    {
        // test for mul3
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 1, 5, 7 };
        const Vector3 c { 4, 5, 9 };

        const Vector3 aa { 2, 4, 6 };
        const Vector3 bb { 2, 10, 14 };
        const Vector3 cc { 8, 10, 18 };

        const Vector3 ra { 30, 54,88 };
        const Vector3 rb { 68, 124, 202 };
        const Vector3 rc { 90, 156, 256 };

        const Matrix3 m { a, b, c };
        const Matrix3 mm { aa, bb, cc };
        const Matrix3 result = mul(m, mm);
        const Matrix3 calc { ra, rb, rc };

        if (result.col1.x == calc.col1.x && result.col1.y == calc.col1.y && result.col1.z == calc.col1.z && 
            result.col2.x == calc.col2.x && result.col2.y == calc.col2.y && result.col2.z == calc.col2.z && 
            result.col3.x == calc.col3.x && result.col3.y == calc.col3.y && result.col3.z == calc.col3.z)
            std::cout << "mul3 correct" << std::endl;
        else
            std::cout << "mul3 incorrect" << std::endl;
    }

    std::cout << "Exercise 7" << std::endl;
    
    {
        // test for transpose
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 4, 5, 6 };
        const Vector3 c { 7, 8, 9 };

        const Vector3 aa { 1, 4, 7 };
        const Vector3 bb { 2, 5, 8 };
        const Vector3 cc { 3, 6, 9 };

        const Matrix3 m { a, b, c };
        const Matrix3 calc {aa, bb, cc};
        const Matrix3 result = transpose(m);

        if (result.col1.x == calc.col1.x && result.col1.y == calc.col1.y && result.col1.z == calc.col1.z && 
            result.col2.x == calc.col2.x && result.col2.y == calc.col2.y && result.col2.z == calc.col2.z &&
            result.col3.x == calc.col3.x && result.col3.y == calc.col3.y && result.col3.z == calc.col3.z)
            std::cout << "transpose correct" << std::endl;
        else
            std::cout << "transpose incorrect" << std::endl;

    }

    {
        // test for determinant
        const Vector3 a { 1, 2, 3 };
        const Vector3 b { 4, 5, 6 };
        const Vector3 c { 7, 8, 9 };
        const Matrix3 m { a, b, c };
        float result = determinant(m);
        float calculate = 0;
        if (result == calculate)
            std::cout << "determinant correct" << std::endl;
        else
            std::cout << "determinant incorrect" << std::endl;
    }

    {
        // test for inverse

        const Vector3 aa { (float)2, (float)4, (float)6 };
        const Vector3 bb { (float)2, (float)10, (float)14 };
        const Vector3 cc { (float)8, (float)10, (float)18 };

        const Vector3 ra { (float)5/3, (float)-1/2, (float)-1/6 };
        const Vector3 rb { (float)19/6, (float)-1/2, (float)-2/3 };
        const Vector3 rc { (float)-5/2, (float) 1/2, (float)1/2 };

        const Matrix3 mm { aa, bb, cc };
        const Matrix3 calc { ra, rb, rc };
        const Matrix3 result = inverse(mm);

        if (result.col1.x == calc.col1.x && result.col1.y == calc.col1.y && result.col1.z == calc.col1.z && result.col2.x == calc.col2.x && result.col2.y == calc.col2.y && result.col2.z == calc.col2.z && result.col3.x == calc.col3.x && result.col3.y == calc.col3.y && result.col3.z == calc.col3.z)
            std::cout << "inverse correct" << std::endl;
        else if ((calc.col3.y == result.col3.y))
            std::cout << "true" << std::endl;
        else
            std::cout << "false" << std::endl;
    }






    std::cout << "All done!" << std::endl;
    return 0;
}
