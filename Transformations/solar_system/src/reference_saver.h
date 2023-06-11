// ==============================================
// ========   DO NOT MODIFY THIS FILE!   ========
// ==============================================
#pragma once
#include "your_code_here.h"
#include <algorithm> // std::copy
#include <cassert>
#include <filesystem>
#include <fstream>
#include <span>
#include <vector>

class ReferenceSolutionSaver {
public:
    ReferenceSolutionSaver(const std::filesystem::path& filePath);

    void addReferenceSolution(float time, std::span<const CelestrialBody> bodies, std::span<const glm::mat4> transforms);

private:
    struct Solution {
        float time;
        std::vector<CelestrialBody> bodies;
        std::vector<glm::mat4> transforms;
    };

    std::vector<Solution> m_solutions;
    std::ofstream m_file;
};

ReferenceSolutionSaver::ReferenceSolutionSaver(const std::filesystem::path& filePath)
    : m_file(filePath, std::ios::out | std::ios::binary)
{
}

void ReferenceSolutionSaver::addReferenceSolution(
    float time, std::span<const CelestrialBody> bodies, std::span<const glm::mat4> transforms)
{
    m_file.write(reinterpret_cast<const char*>(&time), sizeof(time));
    const size_t numBodies = bodies.size();
    m_file.write(reinterpret_cast<const char*>(&numBodies), sizeof(numBodies));
    for (const auto& body : bodies) {
        m_file.write(reinterpret_cast<const char*>(&body.radius), sizeof(float));
        m_file.write(reinterpret_cast<const char*>(&body.spinPeriod), sizeof(float));
        m_file.write(reinterpret_cast<const char*>(&body.orbitAround), sizeof(int32_t));
        m_file.write(reinterpret_cast<const char*>(&body.orbitAltitude), sizeof(float));
        m_file.write(reinterpret_cast<const char*>(&body.orbitPeriod), sizeof(float));
    }
    m_file.write(reinterpret_cast<const char*>(transforms.data()), transforms.size() * sizeof(transforms[0]));
}