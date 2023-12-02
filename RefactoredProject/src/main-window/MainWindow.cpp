#include "MainWindow.h"
#include "RenderSceneCallback.h"

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

MainWindow::MainWindow(int argc, char** argv)
: _argc{argc}
, _argv{argv}
{
}

std::tuple<int,int> MainWindow::getScreenResolution()
{
#ifdef _WIN32
    // Get the handle to the desktop window
    HWND hDesktop = GetDesktopWindow();

    // Get information about the desktop
    RECT desktopRect;
    GetWindowRect(hDesktop, &desktopRect);

    // Calculate the width and height of the screen
    int screenWidth = desktopRect.right;
    int screenHeight = desktopRect.bottom;

    return {screenWidth, screenHeight};
#endif
    return {0, 0};
}

void MainWindow::setWindowPositionAndSize()
{
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;
    auto [screenWidth, screenHeight] = getScreenResolution();
    if (screenWidth >= windowWidth && screenHeight >= windowHeight)
    {
        glutInitWindowPosition(screenWidth / 2 - windowWidth / 2, screenHeight / 2 - windowHeight / 2);
    }
    else
    {
        glutInitWindowPosition(0, 0);
    }
    glutInitWindowSize(windowWidth, windowHeight);
}

void display()
{
    // OpenGL rendering code goes here

    // For demonstration purposes, let's draw a colored triangle
    glBegin(GL_TRIANGLES);
      glColor3f(1.0, 0.0, 0.0);  // Red
      glVertex2f(-0.5, -0.5);

      glColor3f(0.0, 1.0, 0.0);  // Green
      glVertex2f(0.5, -0.5);

      glColor3f(0.0, 0.0, 1.0);  // Blue
      glVertex2f(0.0, 0.5);
    glEnd();

    // Swap the front and back buffers (if using double buffering)
    glutSwapBuffers();
}

void MainWindow::draw()
{
    glutInit(&_argc, _argv);

    /*
    * GLUT_RGBA: Specifies that the color model should include red, green, blue, and alpha components.
    * This is necessary if you want to use alpha blending for transparency.
    *
    * GLUT_DOUBLE: Enables double buffering, which helps eliminate flickering in animations
    * by using two buffers to alternate between rendering and displaying frames.
    *
    * GLUT_DEPTH: Indicates that a depth buffer for depth testing should be allocated.
    * This is important for 3D graphics to handle occlusions correctly.
    *
    * GLUT_MULTISAMPLE: Enables multi-sampling for smoother rendering.
    */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    setWindowPositionAndSize();

    constexpr auto windowName = "Tetris 3d";
    glutCreateWindow(windowName);

    // background default color
    Color color{ 0.60f, 0.60f, 0.60f, 1.0f };
    glClearColor(color.red, color.green, color.blue, color.alpha);

    /*
    * GL_PROJECTION: This mode is used for setting up the projection matrix,
    * which controls the perspective or orthographic projection of the scene.
    */
    glMatrixMode(GL_PROJECTION);

    // Load the identity matrix into the projection matrix
    glLoadIdentity();

    /*
    * GL_MODELVIEW: This mode is used for setting up the camera and modeling transformations.
    * It is the default matrix mode.
    */
    glMatrixMode(GL_MODELVIEW);

    /*
    * GL_SMOOTH: In smooth shading, colors are interpolated across the surface of the primitive.
    * This interpolation is based on the colors specified at each vertex of the primitive.
    * This results in a smoother transition of colors across the surface.
    */
    glShadeModel(GL_SMOOTH);

    /*
    * glClearDepth(1.0) is used to set the clear depth value to 1.0.
    * The depth values in the depth buffer typically range from 0.0 to 1.0,
    * with 1.0 representing the farthest depth.
    */
    glClearDepth(1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
    * glEnable(GL_DEPTH_TEST) is used to enable depth testing.
    * When depth testing is enabled, OpenGL automatically compares the depth values of pixels
    * and only renders the pixel if it is closer to the viewer than the existing pixel in the depth buffer.
    */
    glEnable(GL_DEPTH_TEST);

    /*
    * GL_LIGHT_MODEL_LOCAL_VIEWER: If enabled, the lighting calculations treat the view vector
    * as if it is in local object space rather than in eye space.
    */
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // OpenGL starts performing lighting calculations during the rendering process.
    glEnable(GL_LIGHTING);

    // configure and enable individual light sources, 2 here
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    /* OpenGL will automatically normalize normal vectors after any transformation
    * (e.g., modelview or projection transformations).
    */
    glEnable(GL_NORMALIZE);

    /*
    * OpenGL automatically updates the material properties (ambient, diffuse, specular, and emission)
    * based on the current color set by glColor
    */
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(RenderSceneCallback::render);
    glutReshapeFunc(RenderSceneCallback::changeSize);
    glutIdleFunc(RenderSceneCallback::renderScene);
    glutKeyboardFunc(RenderSceneCallback::processNormalKeys);
    glutSpecialFunc(RenderSceneCallback::processSpecialKeys);

    glutMainLoop(); // enter GLUT event processing cycle
}
