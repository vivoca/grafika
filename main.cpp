#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #else
//  #include <GL/gl.h>
// #include <GL/glut.h>
#endif

#include "logs.h"
#include <iostream>
#include <fstream>

const size_t WIDTH = 440;
const size_t HEIGHT = 440;
const char *WINDOW_NAME = "OpenGL Explorer";

const char *vertexShaderSource = "#version 410 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 410 core\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "} ";

/*
 * Callback to handle the "close window" event, once the user pressed the Escape key.
 */
static void quitCallback(GLFWwindow *window, int key, int scancode, int action, int _mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/*
 * Initializes the window and viewport via GLFW.
 * The viewport takes the all window.
 * If an error happens, the function returns `NULL` but **does not** free / terminate the GLFW library.
 * Then, do not forget to call `glfwTerminate` if this function returns `NULL`.
 */
GLFWwindow *initializeWindow() {
    // Minimum target is OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);
    if (!window) {
        error("window creation failed");
        return NULL;
    }
    // Close the window as soon as the Escape key has been pressed
    glfwSetKeyCallback(window, quitCallback);
    // Makes the window context current
    glfwMakeContextCurrent(window);

    glViewport( 0, 0, WIDTH, HEIGHT ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, WIDTH, 0, HEIGHT, 0, 1 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment

    return window;
}

using namespace std;
struct Data {
    float x, y;
};

double interpolate(Data function[], int xi, int n) {
    double result = 0;
    for (int i = 0; i < n; i++) {
        double term = function[i].y;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                term = term * (xi - function[j].x) / double(function[i].x - function[j].x);
            }
        }
        result += term;
    }
    return result;
}

float *getPointArray() {
    int n = 4;
    Data function[] = {{1, 2},
                       {2, 3},
                       {3, 1},
                       {4, 4}};
    cout << interpolate(function, 3, n) << endl;

    int j = 0;
    static float vertices[18] = {
            2, 1, 0,
            1, 3, 0,
            5, 5, 0,
            3, 1, 0,
            0.5, 0.5, 0
    };


    for (int i = 0; i < n; i++) {
        vertices[j] = function[i].x/10;
        j++;
        vertices[j] = function[i].y/10;
        j++;
        vertices[j] = 0;
        j++;
    }


    for (int i = 0; i < (n * 4); i++) {
        cout << "*(vertices + " << i << ") : ";
        cout << *(vertices + i) << endl;
    }

    return vertices;
}


int main() {

    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return -1; // Initialize the lib
    }

    GLFWwindow *window = initializeWindow();
    if (!window) {
        glfwTerminate();
        return -1;
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;


    float *vertices;
    vertices = getPointArray();

    // create vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);





    // create vertex shader - transform 3D coordinates into different 3D coordinates
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // fragment shader - calculate the final color of a pixel
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // linking shader program object (multiple shaders combined)
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    while (!glfwWindowShouldClose(window)) {

        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_POINTS, 0, 5);


        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
        // update other events like input handling
        glfwPollEvents();
    }

    // ... here, the user closed the window
    glfwTerminate();
    return 0;
}