#pragma once
// Disable warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
DISABLE_WARNINGS_POP()
#include <iostream>
#include <limits>
#include <utility>

using Color = glm::vec3;

// !!! DO NOT MODIFY THIS STRUCT !!!
struct MaterialInformation {
    Color Kd { 0.5f, 0.5f, 0.5f }; // Diffuse coefficient per vertex.
    Color Ks { 0.5f, 0.5f, 0.5f }; // Specularity coefficient per vertex.
    float shininess = 20.0f; // Exponent for Phong and Blinn-Phong specularities per vertex.

    // Gooch shading
    float goochB, goochY, goochAlpha, goochBeta;
};
struct Positions {
    glm::vec3 vertex; // Vertex position in world space
    glm::vec3 light; // Light position in world space
    glm::vec3 camera; // Camera position in world space
};

// ==========================
// =====    EXERCISE    =====
// ==========================

// Debug function.
Color debugColor(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    // This function you can use in any way you like!
    // E.g., for debugging purposes!
    return (normal + 1.0f) / 2.0f;

    // or random color per vertex:
    // const uint32_t hashX = std::hash<float>()(positions.vertex.x);
    // const uint32_t hashY = std::hash<float>()(positions.vertex.y);
    // const uint32_t hashZ = std::hash<float>()(positions.vertex.z);
    // return Color {
    //     (double)hashX / std::numeric_limits<uint32_t>::max(),
    //     (double)hashY / std::numeric_limits<uint32_t>::max(),
    //     (double)hashZ / std::numeric_limits<uint32_t>::max()
    // };

    // or material information:
    // return materialInformation.Kd;
}



// Standard lambertian shading: Kd * dot(N,L), clamped to zero when negative. Where L is the light vector.
Color diffuseOnly(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    

    if (glm::dot(normal, (positions.light - positions.vertex)) < 0)
        return glm::vec3 { 0, 0, 0 };

    return lightColor * materialInformation.Kd * glm::dot(normal, (positions.light - positions.vertex));
}

// Phong (!) Shading Specularity (http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
// Follow the course, only calculate Ks pow(dot(V,R),shininess), where V is the view vector and R is the Reflection vector of the light (like in pool billard computed from the LightPos, vertexPos and normal).
// When computing specularities like this, verify that the light is on the right side of the surface, with respect to the normal
// E.g., for a plane, the light source below the plane cannot cast light on the top, hence, there can also not be any specularity.
Color phongSpecularOnly(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    glm::vec3 R = glm::normalize((2 * glm::dot(normal, (positions.light - positions.vertex)) * normal) - (positions.light - positions.vertex)); 
    glm::vec3 V = glm::normalize(positions.camera - positions.vertex);
    float RV = glm::dot(R, V);


    if (RV <= 0)
        return glm::vec3 { 0, 0, 0 };

    return lightColor * materialInformation.Ks * glm::pow(RV, materialInformation.shininess);
}

// Blinn-Phong Shading Specularity (http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model)
// Be careful!!! The pseudo code does some additional modifications to the formula seen in the course.
// Follow the course version and calculate ONLY Ks * pow(dot(N,H), shininess). The definition of H is given on the page above and in the course.
// The same test as before should be used.
Color blinnPhongSpecularOnly(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    glm::vec3 H = glm::normalize(((positions.light - positions.vertex) + (positions.camera - positions.vertex)) / glm::length((positions.light - positions.vertex) + (positions.camera - positions.vertex)));
    float HN = glm::dot(H, normal);
    if (HN <= 0)
        return glm::vec3 { 0, 0, 0 };

    return lightColor * materialInformation.Ks * glm::pow(HN, materialInformation.shininess);
}

// Gooch shading model
// A shading model that uses an interpolation between warm and cool colours for increased depth perception.
Color gooch(const MaterialInformation& materialInformation, const Positions& positions, const glm::vec3& normal, const Color& lightColor)
{
    glm::vec3 kblue =  {0, 0, materialInformation.goochB};
    glm::vec3 kyellow = { materialInformation.goochY, materialInformation.goochY, 0 };
    glm::vec3 kcool = kblue + materialInformation.goochAlpha * materialInformation.Kd;
    glm::vec3 kwarm = kyellow + materialInformation.goochBeta * materialInformation.Kd;
    float NL = glm::dot(normal, glm::normalize(positions.light - positions.vertex));
    float coefficient = (1 + NL) / 2;
    
    return lightColor * (coefficient * kwarm + (1 - coefficient) * kcool);



}

// RETURN the new light position, defined as follows:
// selectedPosition is a location on the mesh. Use this location to place the light source to cover the location as seen from cameraPosition (cover the cursor).
// Further, the light should be at a distance of 1.5 from the origin of the scene - in other words, located on a sphere of radius 1.5 around the origin.
// The selectedPos is guaranteed to always be within the sphere.
glm::vec3 userInteractionSphere(const glm::vec3& selectedPosition, const glm::vec3& cameraPosition)
{   
    glm::vec3 u = (cameraPosition - selectedPosition);
    float d = (-2.0f * (glm::dot(u, selectedPosition)) + glm::sqrt((4.0f * glm::pow(glm::dot(u, selectedPosition), 2.0f) - 4.0f * (glm::pow(glm::length(u), 2)) * (glm::pow(glm::length(selectedPosition), 2.0f) - glm::pow(1.5f, 2.0f))))) / (2.0f * glm::pow(glm::length(u), 2.0f));
    return (u * d + selectedPosition);
     

}

// RETURN the new light position such that the light towards the selectedPosition is orthgonal to the normal at that location
// --- in this way, the shading boundary will be exactly at this location.
// there are several ways to do this, choose one you deem appropriate given the current light position
// no panic, I will not judge what solution you chose, as long as the above condition is met.
glm::vec3 userInteractionShadow(const glm::vec3& selectedPosition, const glm::vec3& selectedNormal, const glm::vec3& lightPos)
{
    glm::vec3 x = glm::cross(glm::vec3(1, 0, 0), glm::normalize(selectedNormal));
    return x + selectedPosition;
}

// RETURN the new light position such that a specularity (highlight) will be located at selectedPosition, when viewed from cameraPos.
// Please ensure also that the light is at a distance of 1 from selectedPosition!
// There is only ONE way of doing this!
glm::vec3 userInteractionSpecular(const glm::vec3& selectedPosition, const glm::vec3& selectedNormal, const glm::vec3& cameraPosition)
{
    glm::vec vec1 = cameraPosition - selectedPosition;
    glm::vec reflected = glm::normalize(glm::reflect(-glm::normalize(vec1), glm::normalize(selectedNormal)));

    return reflected + selectedPosition;
}
