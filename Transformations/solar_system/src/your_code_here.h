#pragma once
#include <framework/opengl_includes.h>
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
DISABLE_WARNINGS_POP()
#include <span>
#include <string>
#include <vector>

static glm::mat4 rotationMatrix(float angle, const glm::vec3& axis)
{
    return glm::rotate(glm::identity<glm::mat4>(), angle, axis);
}
static glm::mat4 translationMatrix(const glm::vec3& translation)
{
    return glm::translate(glm::identity<glm::mat4>(), translation);
}
static glm::mat4 scaleMatrix(const glm::vec3& scale)
{
    return glm::scale(glm::identity<glm::mat4>(), scale);
}

// !!! DO NOT MODIFY THIS STRUCT !!!
// Celestial body = planet / moon / star (including the sun)
struct CelestrialBody {
    // The radius of the sphere that represents the celestial body.
    float radius { 1.0f };
    // A celestial body may spin around it's own axis. The speed at which it rotates is
    //  expressed as the spin period. The spin period is the amount of time (in hours) it
    // takes for the body to complete one full rotation.
    float spinPeriod { 0.0f };

    // A celestial body may orbit another celestial body (e.g. a moon orbiting a planet).
    // orbitAround stores the index of the object inside the bodies collection that this
    // celestial body is orbiting. The altitude describes the distance at which the object
    // is orbited (as measured from the center of both objects). The orbit period describes
    // how much time it takes for the celestial body to complete one rotation around the
    // object being orbited.
    //
    // If orbitAround is negative then this celestial body is not orbiting anything and it
    // should instead be placed at the origin (and orbitAltitude/orbitVelocity may be ignored).
    int orbitAround { -1 };
    float orbitAltitude { 0.0f };
    float orbitPeriod { 0.0f };

    // !!! YOU SHOULD IGNORE THE VARIABLES BELOW !!!
    // Only used for UI & drawing.
    std::string name;
    GLuint texture;
};


glm::mat4 helper(const CelestrialBody body, std::span<const CelestrialBody> celestialBodies, float time) {

    if (body.orbitAround == -1) {
        return translationMatrix(glm::vec3(0, 0, 0));
    }
    float x = glm::cos((2.0f * glm::pi<float>() * time) / body.orbitPeriod) * (body.orbitAltitude);
    float z = glm::sin((2.0f * glm::pi<float>() * time) / body.orbitPeriod) * (body.orbitAltitude);
        

    glm::mat4 translate = helper(celestialBodies[body.orbitAround], celestialBodies, time) * translationMatrix(glm::vec3(x, 0, -z));
    return translate;

}
 
// ==========================
// =====    EXERCISE    =====
// ==========================
// Compute the positions/orientations of the solar system (list of celestial bodies) at the given time.
// Orbiting should not impact the spin of the body in any way. E.g. if the body had zero spin and was
// orbiting around another body then it should not rotate at all (only translate). If you implement
// orbit/spin correctly then the earth should always see the same side of the moon.
//
// Both spin and orbiting should be performed around the positive Y axis (glm::vec3(0, 1, 0)).
std::vector<glm::mat4> computeCelestrialBodyTransformations(std::span<const CelestrialBody> celestialBodies, float time)
{
    // For each celestial body, compute the matrix which scales the object (relative to a sphere at the origin with radius=1),
    //  translates and rotates it such that it respects the orbit / spin / scale stored in the input.
    std::vector<glm::mat4> transforms;
    for (const CelestrialBody& body : celestialBodies) {
        // Your implementation goes here
        glm::mat4 matrix = glm::identity<glm::mat4>();

        glm::mat4 scale = scaleMatrix(glm::vec3(body.radius, body.radius, body.radius));
        glm::mat4 spin = rotationMatrix((time*2.0f * glm::pi<float>()/ body.spinPeriod),glm::vec3(0, 1, 0));

        transforms.push_back(helper(body, celestialBodies, time) * spin * scale);
    }
    return transforms;
}