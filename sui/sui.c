#include "sui.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>

void sui_init(const char *title, int width, int height)
{
    if(!glfwInit()) return;
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!window) return;
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 0, 1);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        sui_loop();
        glFlush();
        glfwSwapBuffers(window);
    }
}

void sui_pixel(int x, int y, unsigned int color)
{
    glColor3ub(color >> 16, (color >> 8) & 0xFF, color & 0xFF);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

