#include "ray_tracing.h"
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/geometric.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtx/string_cast.hpp>
DISABLE_WARNINGS_POP()
#include <cmath>
#include <iostream>
#include <limits>


bool intersectRayWithScene(const Scene& scene, Ray& ray)
{
    bool hit = false;
    for (const auto& mesh : scene.meshes)
        hit |= intersectRayWithShape(mesh, ray);
    for (const auto& sphere : scene.spheres)
        hit |= intersectRayWithShape(sphere, ray);
    for (const auto& box : scene.boxes)
        hit |= intersectRayWithShape(box, ray);
    return hit;
}

bool pointInTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& n, const glm::vec3& p)
{
    glm::vec3 a = v1 - v0;
    glm::vec3 b = v2 - v0;
    float area = glm::length(glm::cross(a, b))/ 2.0f;
    float alpha = glm::length(glm::cross((v1 - p), (v2 - p))) / (2.0f * area);
    float beta = glm::length(glm::cross((v1 - p), (v0 - p))) / (2.0f * area);
    float gamma = glm::length(glm::cross((p - v2), (v0 - v2))) / (2.0f * area);

    if (alpha + beta + gamma > 1)
        return false;

    return true;
}

bool intersectRayWithPlane(const Plane& plane, Ray& ray)
{
    float dot = glm::dot(plane.normal, ray.direction);
    if (dot == 0)
        return false;
    float t = (plane.D - glm::dot(ray.origin, plane.normal)) / dot;
    if (t <= 0)
        return false;
    if (t < ray.t) {
        ray.t = t;
        return true;
    }
    return false;

}

Plane trianglePlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
    Plane result;
    glm::vec3 a = v1 - v0;
    glm::vec3 b = v2 - v0;
    result.normal = glm::normalize(glm::cross(a, b));
    result.D = glm::dot(result.normal, v0);
    return result;
}

/// Input: the three vertices of the triangle
/// Output: if intersects then modify the hit parameter ray.t and return true, otherwise return false
bool intersectRayWithTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, Ray& ray)
{
    Plane triangle = trianglePlane(v0, v1, v2);
    float t = ray.t;
    if (intersectRayWithPlane(triangle, ray)) {

        if (pointInTriangle(v0, v1, v2, triangle.normal, (ray.origin + (ray.direction * ray.t))) && t > ray.t) {
            return true;
            
        }
        ray.t = t;
        return false;
        
    }
    ray.t = t;
    return false;
}

bool intersectRayWithShape(const Mesh& mesh, Ray& ray)
{
    bool hit = false;
    for (const auto& tri : mesh.triangles) {
        const auto v0 = mesh.vertices[tri[0]];
        const auto v1 = mesh.vertices[tri[1]];
        const auto v2 = mesh.vertices[tri[2]];
        hit |= intersectRayWithTriangle(v0.position, v1.position, v2.position, ray);
    }
    return hit;
}


/// Input: a sphere with the following attributes: sphere.radius, sphere.center
/// Output: if intersects then modify the hit parameter ray.t and return true, otherwise return false
bool intersectRayWithShape(const Sphere& sphere, Ray& ray)
{
    //float A = glm::pow(ray.direction.x, 2) + glm::pow(ray.direction.y, 2) + glm::pow(ray.direction.z, 2);
    //float B = 2.0f * (ray.direction.x * ray.origin.x + ray.direction.y * ray.origin.y + ray.direction.z * ray.origin.z);
    //float C = glm::pow(ray.origin.x, 2) + glm::pow(ray.origin.y, 2) + glm::pow(ray.origin.z, 2);
    //float D = glm::pow(B, 2) - 4.0f * A * C;

    float D = 4.0f * glm::pow(glm::dot(ray.direction, ray.origin - sphere.center), 2) - 4.0f * glm::pow(glm::length(ray.direction), 2) * (glm::pow(glm::length(ray.origin - sphere.center), 2) - glm::pow(sphere.radius, 2));
    if (D < 0)
        return false;    
    float solution = solution = (-2.0f * glm::dot(ray.direction, ray.origin - sphere.center) - glm::sqrt(D)) / (2.0f * glm::pow(glm::length(ray.direction), 2)); 



    if (solution <= 0) {
        solution = (-2.0f * glm::dot(ray.direction, ray.origin - sphere.center) + glm::sqrt(D)) / (2.0f * glm::pow(glm::length(ray.direction),2));
    }
    if (solution > 0 && solution < ray.t) {
        ray.t = solution;
        return true;
    }
    return false;
}

/// Input: an axis-aligned bounding box with the following parameters: minimum coordinates box.lower and maximum coordinates box.upper
/// Output: if intersects then modify the hit parameter ray.t and return true, otherwise return false
bool intersectRayWithShape(const AxisAlignedBox& box, Ray& ray)
{

    float xmin = box.lower.x;
    float ymin = box.lower.y;
    float zmin = box.lower.z;

    float xmax = box.upper.x;
    float ymax = box.upper.y;
    float zmax = box.upper.z;

    float txmin;
    float tymin;
    float tzmin;

    float txmax;
    float tymax;
    float tzmax;

    if (ray.direction.x == 0) {
        txmin = std::numeric_limits<float>::min();
        txmax = std::numeric_limits<float>::max();
    } else {
        txmin = (xmin - ray.origin.x) / ray.direction.x;
        txmax = (xmax - ray.origin.x) / ray.direction.x;
    }

    if (ray.direction.y == 0) {
        tymin = std::numeric_limits<float>::min();
        tymax = std::numeric_limits<float>::max();
    } else {
        tymin = (ymin - ray.origin.y) / ray.direction.y;
        tymax = (ymax - ray.origin.y) / ray.direction.y;
    }

    if (ray.direction.z == 0) {
        tzmin = std::numeric_limits<float>::min();
        tzmax = std::numeric_limits<float>::max();
    } else {
        tzmin = (zmin - ray.origin.z) / ray.direction.z;
        tzmax = (zmax - ray.origin.z) / ray.direction.z;
    }

    float tIn = glm::max(glm::min(txmin, txmax), glm::min(tymin, tymax));
    tIn = glm::max(tIn, glm::min(tzmin, tzmax));
    
    float tOut = glm::min(glm::max(txmax, txmin), glm::max(tymax, tymin));
    tOut = glm::min(tOut, glm::max(tzmax, tzmin));

    if (tIn > tOut || tOut < 0)
        return false;

    if (tIn < 0 && tOut > 0 && tOut < ray.t) {
        ray.t = tOut;
        return true;
    }

    if (ray.t > tIn) {
        ray.t = tIn;
        return true;
    }

    return false;
}
