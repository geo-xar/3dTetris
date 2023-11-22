#include "Grid.h"

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define N 15 // grid size

constexpr float K = N / 2.0f;

void Grid::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float i;

    glColor3f(0, 0, 0);

    for (i = -K; i <= K; i += 1.0)
    {
        glBegin(GL_LINES);
        glVertex3f(-K, i, -K);
        glVertex3f(K, i, -K);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(i, -K, -K);
        glVertex3f(i, K, -K);
        glEnd();

    }

    for (i = -K; i <= K; i += 1.0)
    {
        glBegin(GL_LINES);
        glVertex3f(K, i, -K);
        glVertex3f(K, i, K);
        glEnd();
    }

    for (i = -K; i <= (K + 0.1); i += 1.0)
    {
        glBegin(GL_LINES);
        glVertex3f(K, -K, i);
        glVertex3f(K, K, i);
        glEnd();
    }

    for (i = -K; i <= K; i += 1.0)
    {
        glBegin(GL_LINES);
        glVertex3f(i, -K, -K);
        glVertex3f(i, -K, K);
        glEnd();
    }

    for (i = -K; i <= (K + 0.1); i += 1.0)
    {
        glBegin(GL_LINES);
        glVertex3f(-K, -K, i);
        glVertex3f(K, -K, i);
        glEnd();
    }
}
