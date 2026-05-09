#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

struct Triangle {
    GLuint vao;
    glm::vec3 color;
};

std::vector<Triangle> triangles;       
std::vector<glm::vec3> tempVertices;   

GLuint createTriangleVAO(float x0, float y0, float x1, float y1, float x2, float y2) {
    GLfloat vertices[] = {
        x0, y0, 0.0f,
        x1, y1, 0.0f,
        x2, y2, 0.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        tempVertices.push_back(glm::vec3((float)xpos, (float)ypos, 0.0f));
        std::cout << "Vertice registrado: (" << xpos << ", " << ypos << ")" << std::endl;

        if (tempVertices.size() == 3) {
            GLuint vao = createTriangleVAO(
                tempVertices[0].x, tempVertices[0].y,
                tempVertices[1].x, tempVertices[1].y,
                tempVertices[2].x, tempVertices[2].y
            );

            glm::vec3 color = glm::vec3(
                (float)rand() / RAND_MAX,
                (float)rand() / RAND_MAX,
                (float)rand() / RAND_MAX
            );

            triangles.push_back({vao, color});
            tempVertices.clear(); 
            
            std::cout << "Triangulo criado com sucesso!" << std::endl;
        }
    }
}

int main() {
    if (!glfwInit()) return -1;
    srand(static_cast<unsigned int>(time(0))); 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Atividade Vivencial 1", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    const char* vertex_shader =
        "#version 410\n"
        "layout ( location = 0 ) in vec3 vPosition;"
        "uniform mat4 proj;"
        "void main() {"
        "    gl_Position = proj * vec4(vPosition, 1.0);"
        "    gl_PointSize = 8.0;" 
        "}";

    const char* fragment_shader =
        "#version 410\n"
        "uniform vec3 uColor;"
        "out vec4 frag_color;"
        "void main(){"
        "  frag_color = vec4(uColor, 1.0f);"
        "}";

    int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    
    int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    
    int shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);

    GLuint tempVAO, tempVBO;
    glGenVertexArrays(1, &tempVAO);
    glGenBuffers(1, &tempVBO);

    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_programme);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "proj"), 1, GL_FALSE, glm::value_ptr(proj));


        for (const auto& tri : triangles) {
            glUniform3f(glGetUniformLocation(shader_programme, "uColor"), tri.color.r, tri.color.g, tri.color.b);
            glBindVertexArray(tri.vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        if (!tempVertices.empty()) {
            glBindVertexArray(tempVAO);
            glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
            

            glBufferData(GL_ARRAY_BUFFER, tempVertices.size() * sizeof(glm::vec3), tempVertices.data(), GL_DYNAMIC_DRAW);
            
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glUniform3f(glGetUniformLocation(shader_programme, "uColor"), 1.0f, 0.0f, 0.0f); 
            glDrawArrays(GL_POINTS, 0, tempVertices.size());
            
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}