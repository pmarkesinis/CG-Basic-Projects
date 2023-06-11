#include "draw.h"
#include "ray_tracing.h"
// Disable compiler warnings in third-party code (which we cannot change).
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <imgui/imgui.h>
DISABLE_WARNINGS_POP()
#include <array>
#include <framework/trackball.h>
#include <framework/window.h>
#include <fstream>
#include <iostream>
#include <optional>


// This is the main application. The code in here does not need to be modified.
constexpr glm::ivec2 windowResolution { 800, 800 };


int main(int argc, char** argv)
{
    Trackball::printHelp();
    std::cout << "\n Usage: \"Practical4 [scene_id]\"" << std::endl;
    std::cout << "\n scene_id = 0 : Single triangle" << std::endl;
    std::cout << "\n scene_id = 1 : Cube (12 triangles)" << std::endl;
    std::cout << "\n scene_id = 2 : Dragon (87K triangles)" << std::endl;
    std::cout << "\n scene_id = 3 : 3 AABBs" << std::endl;
    std::cout << "\n scene_id = 4 : 3 spheres" << std::endl;
    std::cout << "\n scene_id = 5 : Mixed scene (AABBs, spheres and triangles)" << std::endl;
    std::cout << std::endl;
    std::cout << "\n Press the [R] key on your keyboard to create a ray towards the mouse cursor" << std::endl
              << std::endl;

    Window window { argv[0], windowResolution, OpenGLVersion::GL2 };
    Trackball camera { &window, glm::radians(50.0f), 3.0f };
    camera.setCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(glm::vec3(20.0f, 20.0f, 0.0f)), 3.0f);

    std::optional<Ray> optRay;

    SceneType type = SingleTriangle;
    if (argc > 1)
        type = static_cast<SceneType>(atoi(argv[1]));

    optRay = camera.generateRay(window.getNormalizedCursorPos() * -2.0f + 1.0f);
    Scene scene = loadScene(type, DATA_DIR, optRay);

    if (type == Custom)
        intersectRayWithScene(scene, *optRay);
    else
        optRay.reset();


    //const glm::vec3 v0 = scene.meshes[0].vertices[0].position;
    //const glm::vec3 v1 = scene.meshes[0].vertices[1].position;
    //const glm::vec3 v2 = scene.meshes[0].vertices[2].position;

    //const glm::vec3 e01 = v1 - v0;
    //const glm::vec3 e02 = v2 - v0;
    //glm::vec3 normal = glm::normalize(glm::cross(e01, e02));


    window.registerKeyCallback([&](int key, int /* scancode */, int action, int /* mods */) {
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_R: {
                // Shoot a ray. Produce a ray from camera to the far plane.
                optRay = camera.generateRay(window.getNormalizedCursorPos() * 2.0f - 1.0f);
                intersectRayWithScene(scene, *optRay);


            } break;
            case GLFW_KEY_T: {
                // Shoot a ray. Produce a ray from camera to the far plane.
                //optRay = camera.generateRay(window.getNormalizedCursorPos() * 2.0f - 1.0f);
                optRay->t = std::numeric_limits<float>::max();

                // optRay->origin = glm::vec3(-1.711633, 4.787903, 2.941202);
                // optRay->direction = glm::vec3(0.762519, -0.015091, -0.646789);

                optRay->origin = glm::vec3(-1.590500, -5.043800, -4.981800);
                optRay->direction = glm::vec3(0.040139, 0.568419, 0.821759);


                intersectRayWithScene(scene, *optRay);



            } break;
            case GLFW_KEY_ESCAPE: {
                window.close();
            } break;
            };
        }
    });

    while (!window.shouldClose()) {

        window.updateInput();

        ImGui::Begin("Ray Tracing Assignment");
        {
            ImGui::Text("Press [R] to trace a ray from the camera.\nMove camera afterwards to make it visible.");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            const std::array items { "SingleTriangle", "Cube", "Dragon", "AABBs", "Spheres", "Mixed", "Custom" };
            //if (ImGui::Combo("Scene", reinterpret_cast<int*>(&type), items.data(), (int)items.size())) {
            if (ImGui::ListBox("Scenes", reinterpret_cast<int*>(&type), items.data(), int(items.size()), int(items.size()))) {
                scene = loadScene(type, DATA_DIR, optRay);
                if (optRay) {
                    optRay->t = std::numeric_limits<float>::max();
                    intersectRayWithScene(scene, *optRay);
                }
            }
        }

        glViewport(0, 0, window.getFrameBufferSize().x, window.getFrameBufferSize().y);

        // Clear screen.
        glClearDepth(1.0);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate the light in the legacy OpenGL mode.
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        constexpr glm::vec4 lightPos { 0, 0, -2, 0 };
        glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(lightPos));

        // Normals will be normalized in the graphics pipeline.
        glEnable(GL_NORMALIZE);

        // Activate rendering modes.
        glEnable(GL_DEPTH_TEST);
        // Draw front and back facing triangles filled
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        // Interpolate vertex colors over the triangles.
        glShadeModel(GL_SMOOTH);

        // Load view matrix.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        const glm::mat4 viewMatrix = camera.viewMatrix();
        glMultMatrixf(glm::value_ptr(viewMatrix));

        // Load projection matrix.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        const glm::mat4 projectionMatrix = camera.projectionMatrix();
        glMultMatrixf(glm::value_ptr(projectionMatrix));

        // Draw the scene and the ray (if any).
        drawScene(scene);

        if (optRay) {
            drawRay(*optRay);
        }

        // Draw a colored sphere at the location at which the trackball is looking/rotating around.
        glDisable(GL_LIGHTING);
        const Sphere trackballSphere { camera.lookAt(), 0.01f, { glm::vec3(0.2f, 0.2f, 1.0f) } };
        drawShape(trackballSphere);

        ImGui::End();

        window.swapBuffers();
    }

    return 0; // execution never reaches this point
}
