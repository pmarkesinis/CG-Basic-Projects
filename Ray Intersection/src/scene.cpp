#include "scene.h"
#include <iostream>
#include <random>

Scene loadScene(SceneType type, const std::filesystem::path& dataDir, std::optional<Ray>& optRay)
{
    std::default_random_engine rng { 814912384 };
    std::uniform_real_distribution<float> distribution_dir { -1.0f, 1.0f };
    std::uniform_real_distribution<float> distribution_mat { 0.0f, 1.0f };
    std::uniform_real_distribution<float> distribution_radius { 0.01f, 1.0f };
    std::uniform_real_distribution<float> distribution_vertex { -2.0f, 2.0f };
    std::uniform_real_distribution<float> distribution_pos { -5.0f, 5.0f };

    Scene scene;
    switch (type) {
    case SingleTriangle: {
        // Load a 3D model with a single triangle
        auto subMeshes = loadMesh(dataDir / "triangle.obj");
        std::move(std::begin(subMeshes), std::end(subMeshes), std::back_inserter(scene.meshes));
    } break;
    case Cube: {
        // Load a 3D model of a cube with 12 triangles
        auto subMeshes = loadMesh(dataDir / "cube.obj");
        std::move(std::begin(subMeshes), std::end(subMeshes), std::back_inserter(scene.meshes));
    } break;
    case Dragon: {
        // Load a 3D model of a Dragon
        auto subMeshes = loadMesh(dataDir / "dragon.obj");
        std::move(std::begin(subMeshes), std::end(subMeshes), std::back_inserter(scene.meshes));
    } break;
    case AABBs: {
        scene.boxes.push_back(AxisAlignedBox { glm::vec3(-2.0f, -2.0f, 5.0f), glm::vec3(-1.0f, -1.0f, 6.0f), Material { glm::vec3(0.8f, 0.8f, 0.2f) } });
        scene.boxes.push_back(AxisAlignedBox { glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.5f, 1.5f, 7.0f), Material { glm::vec3(0.8f, 0.2f, 0.2f) } });
        scene.boxes.push_back(AxisAlignedBox { glm::vec3(0.5f, 0.5f, 2.0f), glm::vec3(0.9f, 0.9f, 2.5f), Material { glm::vec3(0.2f, 0.2f, 0.8f) } });
    } break;
    case Spheres: {

        for (int i = 0; i < 40; ++i) {
            scene.spheres.push_back(Sphere { glm::vec3(distribution_pos(rng), distribution_pos(rng), distribution_pos(rng)), distribution_radius(rng),
                Material { glm::vec3(distribution_mat(rng), distribution_mat(rng), distribution_mat(rng)) } });
        }   

        // scene.spheres.push_back(Sphere { glm::vec3(3.0f, -2.0f, 10.2f), 1.0f, Material { glm::vec3(0.8f, 0.2f, 0.2f) } });
        // scene.spheres.push_back(Sphere { glm::vec3(-2.0f, 2.0f, 4.0f), 2.0f, Material { glm::vec3(0.6f, 0.8f, 0.2f) } });
        // scene.spheres.push_back(Sphere { glm::vec3(0.0f, 0.0f, 6.0f), 0.75f, Material { glm::vec3(0.2f, 0.2f, 0.8f) } });
    } break;
    case Mixed: {

//        Load a 3D model of a car on top of a colored cube.
        {
            auto subMeshes = loadMesh(dataDir / "dodgeColorTest.obj");
            std::move(std::begin(subMeshes), std::end(subMeshes), std::back_inserter(scene.meshes));
        }
        // Let's create some more shapes (spheres and axis aligned boxes). You can modify or create
        // more to your applications. These are not OBJ files, the are defined in the shapes file.
        scene.spheres.push_back(Sphere { glm::vec3(3.0f, -2.0f, 10.2f), 1.0f, Material { glm::vec3(0.8f, 0.2f, 0.2f) } });
        scene.spheres.push_back(Sphere { glm::vec3(-2.0f, 2.0f, 4.0f), 2.0f, Material { glm::vec3(0.2f, 0.8f, 0.2f) } });
        scene.boxes.push_back(AxisAlignedBox { glm::vec3(-2.0f, -2.0f, 5.0f), glm::vec3(-1.0f, -1.0f, 6.0f), Material { glm::vec3(0.8f, 0.8f, 0.2f) } });
    } break;
    default: {
        // custom scene, here you can create your own scene using the examples above. Try to set up scenes to test particular scenarios, such as corner cases.

        scene.boxes.push_back(AxisAlignedBox { glm::vec3(-2.0f, -2.0f, 5.0f), glm::vec3(-1.0f, -1.0f, 6.0f), Material { glm::vec3(0.8f, 0.8f, 0.2f) } });
        scene.boxes.push_back(AxisAlignedBox { glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.5f, 1.5f, 7.0f), Material { glm::vec3(0.8f, 0.2f, 0.2f) } });
        scene.boxes.push_back(AxisAlignedBox { glm::vec3(0.5f, 0.5f, 2.0f), glm::vec3(0.9f, 0.9f, 2.5f), Material { glm::vec3(0.2f, 0.2f, 0.8f) } });

        // here you can define a custom ray to start with, so you can precisely set the values
        optRay = {
            .origin = glm::vec3(0.0f, 0.0f, -3.0f),
            .direction = glm::vec3(0.0f, 0.0f, 1.0f)
        };

    } break;
    };

    return scene;
}