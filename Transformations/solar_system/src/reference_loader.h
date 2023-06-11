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

class ReferenceSolutionLoader {
public:
    ReferenceSolutionLoader(const std::filesystem::path& file);

    int getNumReferenceSolutions() const;
    void getReferenceSolution(int id, float& time, std::vector<CelestrialBody>& bodies, std::vector<glm::mat4>& transforms) const;

private:
    struct Solution {
        float time;
        std::vector<CelestrialBody> bodies;
        std::vector<glm::mat4> transforms;
    };

    std::vector<Solution> m_solutions;
};

ReferenceSolutionLoader::ReferenceSolutionLoader(const std::filesystem::path& filePath)
{
    assert(std::filesystem::exists(filePath));

    std::fstream file { filePath, std::ios::binary | std::ios::in };
    //return;
    while (!file.eof()) {
        Solution solution {};
        file.read(reinterpret_cast<char*>(&solution.time), sizeof(solution.time));
        size_t numBodies { 0 };
        file.read(reinterpret_cast<char*>(&numBodies), sizeof(numBodies));

        solution.bodies.resize(numBodies);
        //file.read(reinterpret_cast<char*>(solution.bodies.data()), solution.bodies.size() * sizeof(solution.bodies[0]));
        for (auto& body : solution.bodies) {
            file.read(reinterpret_cast<char*>(&body.radius), sizeof(float));
            file.read(reinterpret_cast<char*>(&body.spinPeriod), sizeof(float));
            file.read(reinterpret_cast<char*>(&body.orbitAround), sizeof(int32_t));
            file.read(reinterpret_cast<char*>(&body.orbitAltitude), sizeof(float));
            file.read(reinterpret_cast<char*>(&body.orbitPeriod), sizeof(float));
        }

        solution.transforms.resize(numBodies);
        file.read(reinterpret_cast<char*>(solution.transforms.data()), solution.transforms.size() * sizeof(solution.transforms[0]));

        m_solutions.push_back(solution);
    }
    // Closes 1 too late?
    m_solutions.resize(m_solutions.size() - 1);
}

int ReferenceSolutionLoader::getNumReferenceSolutions() const
{
    return static_cast<int>(m_solutions.size());
}

void ReferenceSolutionLoader::getReferenceSolution(int id, float& time, std::vector<CelestrialBody>& bodies, std::vector<glm::mat4>& transforms) const
{
    const auto& solution = m_solutions[id];
    time = solution.time;
    assert(bodies.size() == solution.bodies.size());
    for (size_t i = 0; i < bodies.size(); i++) {
        // OpenGL texture & std::string are not trivially copyable so we don't store them to disk.
        const auto texture = bodies[i].texture;
        const auto name = bodies[i].name;
        bodies[i] = solution.bodies[i];
        bodies[i].texture = texture;
        bodies[i].name = name;
    }
    transforms = solution.transforms;
}