#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ocean.hpp"
#include "util.hpp"
#include "plane.hpp"

const size_t N = 256;

// World goes from [-width, width] in all axis
const size_t WORLD_WIDTH = N;

void Ocean::init(GLFWwindow *window) {
    try {
        auto vert_shader = compile_shader("../src/shader.vert", GL_VERTEX_SHADER);
        auto frag_shader = compile_shader("../src/shader.frag", GL_FRAGMENT_SHADER);
        program = link_program({vert_shader, frag_shader});

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    glUseProgram(program);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    screen_size = vec2((float)viewport[2], (float)viewport[3]);
    screen_center = screen_size / 2.0f;

    // Move the cursor to the center of the screen
    mouse_pos = vec2(screen_center.x, screen_center.y);
    glfwSetCursorPos(window, mouse_pos.x, mouse_pos.y);

    Plane<N> plane;
    auto vertices = plane.vertices;
    auto indices = plane.indices;
    num_elements = indices.size();

    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vertex_buffer = 0;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    {
        GLsizeiptr size = vertices.size() * sizeof(Vertex);
        const GLvoid *data = &(vertices[0]);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    GLuint pos_attrib = 0;
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), (char *)nullptr + 0);

    // TODO: Fix the normal vectors
    GLuint norm_attrib = 1;
    glEnableVertexAttribArray(norm_attrib);
    glVertexAttribPointer(norm_attrib, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), (char *)0 + sizeof(vec3));

    // Put the plane into world coordinates
    model = plane.get_model_matrix(WORLD_WIDTH);
    GLint model_attrib = 2;
    glUniformMatrix4fv(model_attrib, 1, GL_FALSE, value_ptr(model));

    // Put the world into camera/view coordinates
    camera = Camera(vec3(0.0f, 8.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
    update_view_matrix();

    update_perspective_matrix();

    index_buffer = 0;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    {
        GLsizeiptr size = indices.size() * sizeof(unsigned int);
        const GLvoid *data = &(indices[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
}

void Ocean::cleanup() {
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vao);
}

void Ocean::update(double dt) {
    static const float move_speed = 24.0;
    float move_amt = move_speed * (float)dt;

    if (pressed_keys.find(GLFW_KEY_W) != pressed_keys.end()) {
        camera.move(Camera::Direction::FORWARD, move_amt);
    }
    if (pressed_keys.find(GLFW_KEY_S) != pressed_keys.end()) {
        camera.move(Camera::Direction::BACKWARD, move_amt);
    }
    if (pressed_keys.find(GLFW_KEY_A) != pressed_keys.end()) {
        camera.move(Camera::Direction::LEFT, move_amt);
    }
    if (pressed_keys.find(GLFW_KEY_D) != pressed_keys.end()) {
        camera.move(Camera::Direction::RIGHT, move_amt);
    }

    update_view_matrix();
}

void Ocean::draw() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (char *)nullptr + 0);
}

void Ocean::on_key_event(GLFWwindow *window, int key,
                         int scancode, int action, int mods) {
    (void)window;
    (void)mods;

    if (action == GLFW_PRESS) {
        const char *key_name = glfwGetKeyName(key, scancode);
        if (!key_name) {
            return;
        }

        this->pressed_keys[key] = key_name;

        // TODO: Rebind this or require CTRL to be held
        switch (key_name[0]) {
        case 'w':
        case 'W':
            if (!wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframe = true;
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframe = false;
            }
            break;
        }

    } else if (action == GLFW_RELEASE) {
        this->pressed_keys.erase(key);
    }
}

void Ocean::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
    (void)window;

    const float sensitivity = 0.05f;

    // TODO: Can the positions overflow, since they're unbounded?
    vec2 delta = vec2(xpos - mouse_pos.x, mouse_pos.y - ypos);
    delta *= sensitivity;

    camera.rotate2d(delta);
    update_view_matrix();

    // Update state variables
    mouse_pos = vec2(xpos, ypos);
}

void Ocean::on_window_resize(GLFWwindow *window, int width, int height) {
    (void)window;

    screen_size = vec2(width, height);
    screen_center = vec2(width, height) / 2.0f;
    update_perspective_matrix();
}

void Ocean::update_view_matrix() {
    view = camera.get_view_matrix();
    GLint view_attrib = 3;
    glUniformMatrix4fv(view_attrib, 1, GL_FALSE, value_ptr(view));
}

void Ocean::update_perspective_matrix() {
    float aspect_ratio = (float)screen_size.x / (float)screen_size.y;
    perspective = glm::perspective(
        glm::radians(45.0f), aspect_ratio, 0.1f, (float)WORLD_WIDTH);

    GLint persp_attrib = 4;
    glUniformMatrix4fv(persp_attrib, 1, GL_FALSE, value_ptr(perspective));
}
