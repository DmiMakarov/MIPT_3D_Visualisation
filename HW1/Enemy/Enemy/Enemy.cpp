#include<stdio.h>
#include<stdlib.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"shader.hpp"

using namespace glm;

GLFWwindow* window;

double get_x(double t) {
    return 3 * glm::cos(t);
}

double get_y(double t) {
    return 3 * glm::sin(t);
}

double get_z(double t) {
    return t;
}

int main() {
    if (!glfwInit())
    {
        fprintf(stderr, "Ошибка при инициализации GLFWn");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(2048, 1536, "Triangles", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Невозможно открыть окно GLFW. Если у вас Intel GPU, то он не поддерживает версию 3.3. Попробуйте версию уроков для OpenGL 2.1.n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Невозможно инициализировать GLEWn");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.7f, 0.7f, 0.7f, 0.5f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    double t = 0;
    double z = -2;
    double step_t = 0.002;
    double step_z = 0.001;
    glm::mat4 View = glm::lookAt(
        glm::vec3(get_x(t), get_y(t), get_z(z)),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 MVP = Projection * View * Model;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static const GLfloat g_vertex_buffer_data[] = {
       -0.4f,-0.4f,0.6f, -0.4f,0.4f,0.6f, 0.4f,0.4f,0.6f, -0.4f,-0.4f,0.6f, 0.4f,0.4f,0.6f, 0.4f, -0.4f, 0.6f, 0.4f, 0.4f, 0.6f, 0.4f, 0.4f, -0.2f,
       -0.4f,0.4f, -0.2f, 0.4f, 0.4f, 0.6f, -0.4f,0.4f, -0.2f, -0.4f,0.4f, 0.6f,-0.4f,0.4f, 0.6f, -0.4f,0.4f, -0.2f, -0.4f, -0.4, -0.2f,-0.4f,0.4f, 0.6f,
       -0.4f, -0.4f, -0.2f, -0.4f,-0.4f, 0.6f, -0.4f,-0.4f, 0.6f, -0.4f,-0.4f, -0.2f,0.4f, -0.4f, -0.2f, -0.4f,-0.4f, 0.6f,0.4f, -0.4f, -0.2f,
       0.4f, -0.4f, 0.6f,0.4f, -0.4f, -0.2f, 0.4f, -0.4f, 0.6f,0.4f, 0.4f, -0.2f,0.4f, -0.4, 0.6f, 0.4f, 0.4f, -0.2f,0.4f, 0.4f, 0.6f,
       -0.4, -0.4f, -0.2f , 0.4f, -0.4f, -0.2f, 0.4f, 0.4f, -0.2f, -0.4f, -0.4f, -0.2f, -0.4f, 0.4f, -0.2f, 0.4f, 0.4f, -0.2f,0.0f, 0.0f, 1.0f ,
       -0.2f,-0.2f, 0.6f, 0.2f, -0.2f, 0.6f,0.0f, 0.0f, 1.0f,-0.2f,-0.2f, 0.6f,-0.2f,0.2f, 0.6f,0.0f, 0.0f, 1.0f,-0.2f,0.2f, 0.6f,
       0.2f,0.2f,0.6f, 0.0f, 0.0f, 1.0f, 0.2f,0.2f,0.6f, 0.2f,-0.2f,0.6f, 0.0f, 0.0f, -0.6f, -0.2f,-0.2f, -0.2f, 0.2f, -0.2f, -0.2f,0.0f, 0.0f, -0.6f,
       -0.2f,-0.2f, -0.2f, -0.2f,0.2f, -0.2f, 0.0f, 0.0f, -0.6f, -0.2f,0.2f, -0.2f,0.2f,0.2f,-0.2f, 0.0f,0.0f,-0.6f, 0.2f,0.2f,-0.2f,0.2f,-0.2f,-0.2f,
       0.8f, 0.0f, 0.2f,0.4f, -0.2f, 0.4f, 0.4f, 0.2f, 0.4f,0.8f, 0.0f, 0.2f, 0.4f, -0.2f, 0.4f, 0.4f, -0.2f, 0.0f,0.8f, 0.0f, 0.2f,0.4f, -0.2f, 0.0f,
       0.4f, 0.2f, 0.0f, 0.8f, 0.0f, 0.2f,0.4f, 0.2f, 0.0f, 0.4f, 0.2f, 0.4f,-0.8f, 0.0f, 0.2f,-0.4f, -0.2f, 0.4f,  -0.4f, 0.2f, 0.4f,-0.8f, 0.0f, 0.2f,-0.4f, -0.2f, 0.4f,  -0.4f, -0.2f, 0.0f,
       -0.8f, 0.0f, 0.2f,-0.4f, -0.2f, 0.0f, -0.4f, 0.2f, 0.0f,-0.8f, 0.0f, 0.2f,-0.4f, 0.2f, 0.0f, -0.4f, 0.2f, 0.4f,0.0f, 0.8f, 0.2f,
       0.2f, 0.4f, 0.4f,0.2f, 0.4f, 0.0f,0.0f, 0.8f, 0.2f,0.2f, 0.4f, 0.4f,-0.2f, 0.4f, 0.4f,0.0f, 0.8f, 0.2f,-0.2f, 0.4f, 0.4f,-0.2f, 0.4f, 0.0f,
       0.0f, 0.8f, 0.2f,-0.2f, 0.4f, 0.0f, 0.2f, 0.4f, 0.0f, 0.0f, -0.8f, 0.2f, 0.2f,-0.4f, 0.4f, 0.2f, -0.4f, 0.0f, 0.0f, -0.8f, 0.2f, 0.2f, -0.4f, 0.4f, -0.2f, -0.4f, 0.4f,
       0.0f, -0.8f, 0.2f, -0.2f, -0.4f, 0.4f, -0.2f, -0.4f, 0.0f, 0.0f, -0.8f, 0.2f, -0.2f, -0.4f, 0.0f, 0.2f, -0.4f, 0.0f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

   
    static const GLfloat g_color_buffer_data[] = {
       1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
       1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
       1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
       1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
       1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
       1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  1.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
       1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f
    };

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    glm::vec3 CameraVec;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glUseProgram(programID);

        if (z > 2 || z < -2) {
            step_z *= -1;
        }
        z += step_z;
        t += step_t;
        CameraVec[0] = get_x(t);
        CameraVec[1] = get_y(t);
        CameraVec[2] = get_z(z);
        View = glm::lookAt(
            CameraVec,
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
        );
        MVP = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );


        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );


        glDrawArrays(GL_TRIANGLES, 0, 36 * 3);

        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}

