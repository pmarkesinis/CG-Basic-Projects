// ==============================================
// ========   DO NOT MODIFY THIS FILE!   ========
// ==============================================
#include "draw.h"
#include "reference_loader.h"
#include "reference_saver.h"
#include "your_code_here.h"
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
DISABLE_WARNINGS_POP()
#include <array>
#include <framework/trackball.h>
#include <framework/window.h>
#include <fstream>
#include <iostream>
#include <random>

static const std::filesystem::path dataDirPath { DATA_DIR };

int main(int /* argc */, char** argv)
{
    Trackball::printHelp();

    Window window { argv[0], glm::ivec2(1280, 720), OpenGLVersion::GL2 };
    window.registerKeyCallback(
        [&](int key, int /* scancode */, int /* action */, int /* mods */) {
            switch (key) {
            case GLFW_KEY_ESCAPE: {
                window.close();
                break;
            }
            case GLFW_KEY_L: {
                // Turn lighting on.
                if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT) || window.isKeyPressed(GLFW_KEY_RIGHT_SHIFT))
                    glEnable(GL_LIGHTING);
                else
                    glDisable(GL_LIGHTING);

                break;
            }
            };
        });
    Trackball trackball { &window, glm::radians(80.0f), glm::vec3(5.0f, 0.0f, 0.0f), 20.0f, 0.424115360f, 3.28295612f };

    // Generate a random solar system.
    //std::random_device rd;
    std::default_random_engine rng { 327489328 };
    std::uniform_real_distribution<float> distribution { 0.0f, 1.0f };
    auto generateName = [&](std::string prefix) {
        return prefix + " " + std::to_string(rng() % 64);
    };
    std::vector<CelestrialBody> bodies;
    // At the center of the solar system will always be a sun (star).
    bodies.push_back(CelestrialBody {
        .radius = 1.0f + 0.5f * distribution(rng),
        .spinPeriod = 50 + distribution(rng) + 800,
        .name = generateName("sun"),
        .texture = loadTexture(dataDirPath / "2k_sun.jpg") });
    // Generate up to 4 planets around the sun.
    float minOrbitAltitude = bodies[0].radius;
    constexpr int numPlanets = 4;
    for (int i = 0; i < numPlanets; i++) {
        // Earth orbiting around the sun.
        bodies.push_back(CelestrialBody {
            .radius = 0.3f + 0.3f * distribution(rng),
            .spinPeriod = 20.0f + 600.0f * distribution(rng),
            .orbitAround = 0,
            .orbitAltitude = minOrbitAltitude + 0.5f + 1.0f * distribution(rng),
            .orbitPeriod = (100.0f + 800.0f * distribution(rng)) * (rng() % 2 == 0 ? +1.0f : -1.0f),
            .name = generateName("Planet"),
            .texture = rng() % 2 == 0 ? loadTexture(dataDirPath / "2k_earth_daymap.jpg") : loadTexture(dataDirPath / "2k_mars.jpg") });
        minOrbitAltitude += bodies.back().orbitAltitude + bodies.back().radius;
    }
    // Generate a couple of moons rotating around the planets.
    constexpr int numMoons = 3;
    for (int i = 0; i < numMoons; i++) {
        const int orbitAround = (int)(1 + rng() % numPlanets);
        bodies.push_back(CelestrialBody {
            .radius = 0.05f + 0.1f * distribution(rng),
            .spinPeriod = 20.0f + 600.0f * distribution(rng),
            .orbitAround = orbitAround,
            .orbitAltitude = 0.1f + bodies[orbitAround].radius * (1.1f + 0.4f * distribution(rng)),
            .orbitPeriod = (50.0f + 400.0f * distribution(rng)) * (rng() % 2 == 0 ? +1.0f : -1.0f),
            .name = generateName("Moon"),
            .texture = loadTexture(dataDirPath / "2k_moon.jpg") });
    }

    const ReferenceSolutionLoader referenceSolutions { dataDirPath / "reference.bin" };
    int solutionID = -1;
    std::vector<std::string> solutionLabels(referenceSolutions.getNumReferenceSolutions() + 1);
    solutionLabels[0] = "Your solution";
    for (int i = 0; i < referenceSolutions.getNumReferenceSolutions(); i++)
        solutionLabels[i + 1] = "Reference " + std::to_string(i + 1);

    float time = 0.0f, timeIncrement = 0.25f;
    while (!window.shouldClose()) {
        window.updateInput();

        // Initialize viewpoint.
        glViewport(0, 0, window.getFrameBufferSize().x, window.getFrameBufferSize().y);
        // Clear screen.
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Begin("Celestial Bodies");


        // NOTE(Mathijs): solution labels shifted by one such that solutionID == -1 maps to the first label.
        if (ImGui::BeginCombo("Solution", solutionLabels[solutionID + 1].c_str())) {
            for (int i = 0; i < int(solutionLabels.size()); i++) {
                bool isSelected = ((i - 1) == solutionID);
                if (ImGui::Selectable(solutionLabels[i].c_str(), isSelected)) {
                    solutionID = i - 1;
                    if (solutionID != -1)
                        timeIncrement = 0.0f;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();

        if (solutionID != -1) {
            // Disable all inputs when displaying one of the reference solutions.
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        ImGui::Text("Current time: %.1f days", double(time / 24.0f));
        if (ImGui::Button("Pause"))
            timeIncrement = 0.0f;
        ImGui::SliderFloat("Speed (hours per frame)", &timeIncrement, -1.0f, 1.0f);
        ImGui::Spacing();

        for (size_t i = 0; i < bodies.size(); i++) {
            auto& body = bodies[i];

            if (ImGui::CollapsingHeader(body.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                // Without this line it will think all bodies are shared because they use
                // the same labels. When you change the setting for 1 body it will try to
                // change the same value for the other bodies as well. By using IDs we can
                // work around this issue.
                ImGui::PushID(int(i));
                ImGui::SliderFloat("Spin period", &body.spinPeriod, 0.0f, 1000.0f);
                if (body.orbitAround >= 0) {
                    ImGui::SliderFloat("Orbit altitude", &body.orbitAltitude, 1.0f, 20.0f);
                    ImGui::SliderFloat("Orbit period", &body.orbitPeriod, 0.0f, 10000.0f);
                }
                ImGui::Spacing();
                ImGui::PopID();
            }
        }
        if (solutionID != -1) {
            // Undo "Disable all inputs when displaying one of the reference solutions".
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
        ImGui::End();

        // Enable depth test.
        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        // Not how it's designed to be used (view should go into GL_MODELVIEW) but this way students don't need to
        //  worry about the view matrix.
        const glm::mat4 perspectiveMatrix = trackball.projectionMatrix();
        const glm::mat4 viewMatrix = trackball.viewMatrix();
        const glm::mat4 viewProjectionMatrix = perspectiveMatrix * viewMatrix;
        glLoadMatrixf(glm::value_ptr(viewProjectionMatrix));

        drawCoordSystem();
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            // Draw a sphere at the point that the camera is looking at / rotating around to help the user orientate.
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 0.0f, 1.0f);
            drawSphere(trackball.lookAt(), 0.1f);
            glPopAttrib();
        }

        // Enable lighting.
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        // Set the light at the first body (which we assume is the sun).
        constexpr glm::vec4 sunColor { 0.9f, 0.8f, 0.2f, 1 };
        constexpr glm::vec4 sunIntensity = 3.0f * sunColor;
        glEnable(GL_LIGHT0);
        {
            // Create a l-value because passing a pointer to an r-value ( glm::value_ptr(glm::vec4(...)) ) is illegal!
            const glm::vec4 sunPosition { glm::vec3(0), 1 };
            glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(sunPosition));
        }
        glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(sunIntensity));

        std::vector<glm::mat4> transforms;
        if (solutionID == -1) {
            transforms = computeCelestrialBodyTransformations(bodies, time);
        } else {
            referenceSolutions.getReferenceSolution(solutionID, time, bodies, transforms);
        }

        for (size_t i = 0; i < transforms.size(); i++) {
            if (i == 0) {
                // First body is the sun (should appear to emit light).
                glMaterialfv(GL_FRONT, GL_EMISSION, glm::value_ptr(sunColor));
            } else {
                // Other bodies don't emit light.
                constexpr glm::vec4 black { 0, 0, 0, 1 };
                glMaterialfv(GL_FRONT, GL_EMISSION, glm::value_ptr(black));
            }

            drawSphere(transforms[i], bodies[i].texture);
        }

        time += timeIncrement;
        window.swapBuffers();
    }

    std::cout << "X" << std::endl;
}