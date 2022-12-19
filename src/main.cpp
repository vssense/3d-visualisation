#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>
#include "LoadShader.hpp"

struct Vec3
{
    float x;
    float y;
    float z;
};

struct Vec4
{
    float x;
    float y;
    float z;
    float w;
};

struct Vertex
{
    Vec3 pos;
    Vec4 color;
};

int PyramidMain()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1024, 768, "pyramid", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to load glad\n";
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint shader_id = LoadShaders("../src/VertexShader.vert", "../src/FragmentShader.frag");

    GLuint vlocation = glGetUniformLocation(shader_id, "V");
    GLuint plocation = glGetUniformLocation(shader_id, "P");

    glUseProgram(shader_id);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(plocation, 1, GL_FALSE, glm::value_ptr(projection));

    const Vertex pyramid[] = {
        Vertex{Vec3{ 0.0f, 0.0f, 0.0f}, Vec4{1.000f, 0.000f, 0.000f, 1.f}},
        Vertex{Vec3{ 2.0f, 2.0f, 0.0f}, Vec4{0.000f, 1.000f, 0.000f, 1.f}},
        Vertex{Vec3{ 0.0f, 1.0f, 0.0f}, Vec4{0.700f, 0.000f, 0.700f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 0.0f}, Vec4{1.000f, 0.000f, 0.000f, 1.f}},
        Vertex{Vec3{-2.0f, 2.0f, 0.0f}, Vec4{0.000f, 0.700f, 0.700f, 1.f}},
        Vertex{Vec3{ 0.0f, 1.0f, 0.0f}, Vec4{0.700f, 0.000f, 0.700f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 0.0f}, Vec4{1.000f, 0.000f, 0.000f, 1.f}},
        Vertex{Vec3{ 2.0f, 2.0f, 0.0f}, Vec4{0.000f, 1.000f, 0.000f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 1.0f}, Vec4{0.100f, 0.100f, 0.100f, 1.f}},
        Vertex{Vec3{ 2.0f, 2.0f, 0.0f}, Vec4{0.000f, 1.000f, 0.000f, 1.f}},
        Vertex{Vec3{ 0.0f, 1.0f, 0.0f}, Vec4{0.700f, 0.000f, 0.700f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 1.0f}, Vec4{0.100f, 0.100f, 0.100f, 1.f}},
        Vertex{Vec3{ 0.0f, 1.0f, 0.0f}, Vec4{0.700f, 0.700f, 0.700f, 1.f}},
        Vertex{Vec3{-2.0f, 2.0f, 0.0f}, Vec4{0.609f, 0.115f, 0.436f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 1.0f}, Vec4{0.000f, 0.000f, 0.000f, 1.f}},
        Vertex{Vec3{-2.0f, 2.0f, 0.0f}, Vec4{0.000f, 0.700f, 0.700f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 0.0f}, Vec4{1.000f, 0.000f, 0.000f, 1.f}},
        Vertex{Vec3{ 0.0f, 0.0f, 1.0f}, Vec4{0.100f, 0.100f, 0.100f, 1.f}}
    };

    GLuint vert_arr_id;
    glGenVertexArrays(1, &vert_arr_id);
    glBindVertexArray(vert_arr_id);

    GLuint vert_buf_id;
    glGenBuffers(1, &vert_buf_id);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color));

    float angle = 0.0f;
    float dist = 10.0f;

    while (glfwWindowShouldClose(window) == 0)
    {
        angle += 0.02;
        glm::mat4 view = glm::lookAt(glm::vec3(dist * std::cos(angle), dist * std::cos(angle) + dist * std::sin(angle),
                                     dist * std::sin(angle)), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(vlocation, 1, GL_FALSE, glm::value_ptr(view));

        glDrawArrays(GL_TRIANGLES, 0, sizeof(pyramid) / sizeof(Vertex));

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteProgram(shader_id);
    glDeleteBuffers(1, &vert_buf_id);

    glfwTerminate();

    return 0;
}

int TrianglesMain()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1024, 768, "triangles", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to load glad\n";
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint shader1 = LoadShaders("../src/VertexShader.vert", "../src/FragmentShader.frag");

    GLuint vlocation1 = glGetUniformLocation(shader1, "V");
    GLuint plocation1 = glGetUniformLocation(shader1, "P");

    GLuint shader2 = LoadShaders("../src/VertexShader2.vert", "../src/FragmentShader.frag");

    GLuint vlocation2 = glGetUniformLocation(shader2, "V");
    GLuint plocation2 = glGetUniformLocation(shader2, "P");

    glUseProgram(shader1);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(plocation1, 1, GL_FALSE, glm::value_ptr(projection));

    const Vertex triangles[] = {
        Vertex{Vec3{-2.0f, -2.0f, 0.0f}, Vec4{1.0f, 0.0f, 0.0f, 0.5f}},
        Vertex{Vec3{ 2.0f, -2.0f, 0.0f}, Vec4{1.0f, 0.0f, 0.0f, 0.5f}},
        Vertex{Vec3{ 2.0f,  2.0f, 0.0f}, Vec4{1.0f, 0.0f, 0.0f, 0.5f}},

        Vertex{Vec3{-1.0f,  3.0f, -1.0f}, Vec4{0.0f, 0.0f, 1.0f, 0.5f}},
        Vertex{Vec3{-3.0f, -3.0f,  1.0f}, Vec4{0.0f, 0.0f, 1.0f, 0.5f}},
        Vertex{Vec3{ 1.0f, -1.0f,  0.0f}, Vec4{0.0f, 0.0f, 1.0f, 0.5f}}
    };

    GLuint vert_arr_id;
    glGenVertexArrays(1, &vert_arr_id);
    glBindVertexArray(vert_arr_id);

    GLuint vert_buf_id;
    glGenBuffers(1, &vert_buf_id);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color));


    bool c_button_was_released = true;
    bool is_transparent = true;

    GLuint vlocation = vlocation1;

    float angle = 0.0f;
    float dist = 10.0f;

    while (glfwWindowShouldClose(window) == 0)
    {
        angle += 0.02;
        glm::mat4 view = glm::lookAt(glm::vec3(dist * std::cos(angle), dist * std::cos(angle) + dist * std::sin(angle),
                                     dist * std::sin(angle)), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(vlocation, 1, GL_FALSE, glm::value_ptr(view));

        glDrawArrays(GL_TRIANGLES, 0, sizeof(triangles) / sizeof(Vertex));

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        if (!c_button_was_released && glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
        {
            c_button_was_released = true;
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && c_button_was_released)
        {
            c_button_was_released = false;
            is_transparent = !is_transparent;

            if (is_transparent)
            {
                vlocation = vlocation1;
                glUseProgram(shader1);
                glUniformMatrix4fv(plocation1, 1, GL_FALSE, glm::value_ptr(projection));
            }
            else
            {
                vlocation = vlocation2;
                glUseProgram(shader2);
                glUniformMatrix4fv(plocation2, 1, GL_FALSE, glm::value_ptr(projection));
            }
        }
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteProgram(shader1);
    glDeleteProgram(shader2);
    glDeleteBuffers(1, &vert_buf_id);

    glfwTerminate();

    return 0;
}

int main()
{
    // return PyramidMain();

    return TrianglesMain();

    return 0;
}
