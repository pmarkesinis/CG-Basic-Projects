// ==============================================
// ========   DO NOT MODIFY THIS FILE!   ========
// ==============================================
#include "draw.h"
#include <framework/disable_all_warnings.h>
// Suppress warnings in third-party code.
DISABLE_WARNINGS_PUSH()
#include <glm/gtc/type_ptr.hpp>
DISABLE_WARNINGS_POP()
#include <framework/opengl_includes.h>

void drawCoordSystem(const float length)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Remember all states of the GPU.
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    // Deactivate the lighting state.
    glDisable(GL_LIGHTING);

    // Draw axes.
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, length);
    glEnd();

    // Restore previous GPU state.
    glPopAttrib();
}