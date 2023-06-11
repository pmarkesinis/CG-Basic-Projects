#pragma once
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
DISABLE_WARNINGS_POP()
#include <array>
#include <framework/mesh.h>
#include <framework/opengl_includes.h>
#include <span>
#include <vector>

// ===============================
// =====   HELPER FUNCTIONS  =====
// ===============================
// glm::scale / glm::translate / glm::translate apply the matrices in reverse order.
// E.g. calling glm::rotate(glm::translate(inMatrix, ...), ...) will first rotate and then
// translate (reversed order in which the C++ functions are called). We provide these simple
// helpers methods that return a scale/translation/rotation matrix matrix so that you can
// decide on the multiplication order yourself.
// Example: rotationMatrix(...) * translationMatrix(...)
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

int x = 0;


void animate()
{
    x += 2;
}

// =========================
// =====   EXERCISE 1  =====
// =========================
void drawTriangle()
{
    // A simple example of a drawing function for a triangle.
    // 1) Try changing its color to red
    // 2) Try changing its vertex positions
    // 3) Add a second triangle in blue
    // 4) Add a global variable (initialized at 0), which represents the x-coordinate of the first vertex of each triangle
    // 5) Go to the function animate and increment this variable by a small value - observe the animation.

    glColor3f(1, 0, 0);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(x, 3, 1);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 1);
    glEnd();

    glColor3f(0, 0, 1);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(x, 0, 3);
    glVertex3f(1, 0, 2);
    glVertex3f(1, 1, 1);
    glEnd();
}





void drawUnitFace()
{
    // 1) Draw a unit quad in the x,y plane oriented along the z axis
    // 2) Make sure the orientation of the vertices is positive (counter-clock wise)
    // 3) What happens if the order is inversed?
    glColor3f(0, 1, 0);
    glNormal3f(1, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glEnd();
}

void drawUnitCube()
{
    // 1) Draw a unit cube using your function drawUnitFace, rely on glm::translate and glm::rotate to move the faces into position
    //     and apply the transformation using "glLoadMatrixf(glm::value_ptr(nameOfMatrixVariable));"
    //    The cube should span from (0,0,0) to (1,1,1) .

    drawUnitFace();

    glm::mat4 oppositeOfFirst = translationMatrix(glm::vec3(0, 0, 1));
    glLoadMatrixf(glm::value_ptr(oppositeOfFirst));
    drawUnitFace();

    glm::mat4 downOfFirst = rotationMatrix(glm::radians(90.0f), glm::vec3(1, 0, 0));
    glLoadMatrixf(glm::value_ptr(downOfFirst));
    drawUnitFace();

    glm::mat4 rightOfFirst = rotationMatrix(glm::radians(-90.0f), glm::vec3(0, 1, 0));
    glLoadMatrixf(glm::value_ptr(rightOfFirst));
    drawUnitFace();

    glm::mat4 leftOfFirst = rotationMatrix(glm::radians(-90.0f), glm::vec3(0, 1, 0)) * translationMatrix(glm::vec3(0, 0, -1));
    glLoadMatrixf(glm::value_ptr(leftOfFirst));
    drawUnitFace();

    glm::mat4 upOfFirst = rotationMatrix(glm::radians(90.0f), glm::vec3(1, 0, 0)) * translationMatrix(glm::vec3(0, 0, -1));
    glLoadMatrixf(glm::value_ptr(upOfFirst));
    drawUnitFace();
}

struct ArmSegment {
    // Rotation around the positive x axis (glm::vec3(1,0,0)) in radians.
    float rotationX { 0.0f };
    glm::vec3 boxSize;
};

void drawArm(const ArmSegment& segment1, const ArmSegment& segment2, const ArmSegment& segment3)
{
    // Produce a three-unit arm (upper arm, forearm, hand) making use of your function drawUnitCube to define each of them.
    //1) Use the specified segments to define your arm; use glm::scale(matrix, scale) to achieve different arm length and
    //    use glm::rotate(matrix, angleInRadians, axis) and glm::translate(matrix, translation) to correctly place the elements.
    //   Modify the drawUnitCube function to take a glm::mat4 as an argument which is applied to the cube.

    glm::mat4 matrix = glm::identity<glm::mat4>(); // Creates an identity matrix.
    glLoadMatrixf(glm::value_ptr(matrix)); // Anything drawn after this line will be transformed by the given matrix.
    drawUnitCube();


}

void drawMesh(Mesh mesh)
{
    //1) Use the mesh data structure; each triangle is defined with 3 consecutive indices in the mesh.triangles vector.
    //   These indices correspond to vertices stored in mesh.vertices. Provide a function that draws these triangles.
    //   Each vertex stores a position and some additional data. For this exercise you only need to look at the position.
    //
    // The exact definition of Mesh can be found in "framework/include/framework/mesh.h":
    //
    // struct Vertex {
    //     glm::vec3 position;
    //     glm::vec3 normal; // [initially zero]
    //     ...
    // };
    //
    // struct Mesh {
    //     std::vector<Vertex> vertices;
    //     std::vector<glm::uvec3> triangles;
    // };

    //2) Compute the normals of these triangles

    //3) Try computing a normal per vertex as the average of the adjacent face normals, call glNormal3f with the corresponding
    //   values before each vertex. What do you observe with respect to the lighting?

    //4) Try loading your own model (export it from Blender as a Wavefront obj) and replace the provided mesh file.

}