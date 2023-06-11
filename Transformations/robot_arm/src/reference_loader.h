// ==============================================
// ========   DO NOT MODIFY THIS FILE!   ========
// ==============================================
#pragma once
#include "your_code_here.h"
#include <cassert>
#include <filesystem>
#include <framework/opengl_includes.h>
#include <fstream>
#include <span>
#include <vector>

class ReferenceSolutionLoader {
public:
    ReferenceSolutionLoader(const std::filesystem::path& file);

    int getNumReferenceSolutions() const;
    void setAndDrawReferenceSolution(int id, std::span<ArmSegment> armSegments) const;

private:
    struct Solution {
        std::vector<ArmSegment> armSegments;
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
        size_t numSegments { 0 };
        file.read(reinterpret_cast<char*>(&numSegments), sizeof(numSegments));

        solution.armSegments.resize(numSegments);
        file.read(reinterpret_cast<char*>(solution.armSegments.data()), solution.armSegments.size() * sizeof(solution.armSegments[0]));

        solution.transforms.resize(numSegments);
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

void ReferenceSolutionLoader::setAndDrawReferenceSolution(int id, std::span<ArmSegment> armSegments) const
{
    const auto& solution = m_solutions[id];
    std::copy(std::begin(solution.armSegments), std::end(solution.armSegments), std::begin(armSegments));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (const auto& transform : solution.transforms) {
        glLoadMatrixf(glm::value_ptr(transform));
        // Do not copy the code from here into the drawUnitCube function, you won't learn anything from it.
        glBegin(GL_QUADS);
        // Left face.
        glNormal3f(-1, 0, 0);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        // Right face.
        glNormal3f(1, 0, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 0, 0);
        // Back face.
        glNormal3f(0, 0, 1);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(0, 1, 1);
        glVertex3f(0, 0, 1);
        // Front face.
        glNormal3f(0, 0, -1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 0, 0);
        // Top face.
        glNormal3f(0, 1, 0);
        glVertex3f(0, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, 0);
        glVertex3f(0, 1, 0);
        // Bot face.
        glNormal3f(0, -1, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glEnd();
    }
}