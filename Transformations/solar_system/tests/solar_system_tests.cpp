#include "../src/your_code_here.h"
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_decompose.hpp>
DISABLE_WARNINGS_POP()
#include <array>
#include <vector>

// In this file you can add your own unit tests using the Catch2 library.
// You can find the documentation of Catch2 at the following link:
// https://github.com/catchorg/Catch2/blob/devel/docs/assertions.md
//
// These tests are only to help you verify that your code is correct.
// You should NOT hand them in; we will not consider them when grading.

TEST_CASE("MyFirstTestCase")
{
    std::vector<CelestrialBody> bodies;
    bodies.push_back({
        .radius = 1.0f,
        .spinPeriod = 2.0f * glm::pi<float>()
        // ...
    });
    const float time = 0.0f;
    
    const auto transformations = computeCelestrialBodyTransformations(bodies, time);
    const auto transform0 = transformations[0];
    // THIS EXAMPLE IS SUPPOSED TO FAIL! Replace by your own checks.
    REQUIRE(transform0[0][0] == Catch::Approx(123.0f));

    // NOTE:
    // Most decimal numbers cannot be represented exactly due to limited machine precision.
    // In computeCelestrialBodyTransformations the results of each floating point operation is rounded
    // such that it can be represented with 32 bits according to the IEEE754 format. Therefor comparing
    // the result against an exact floating point number will often fail due to tiny rounding errors.
    //
    // The Catch::Approx(123.0f) statement from Catch2 allows for comparisons with some margin to account for rounding.
    // The default margin is quite low; if you need to increase it than you can do so as follows:
    // Catch::Approx(123.0f).margin(0.001f)
}
