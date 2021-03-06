#include "ocean.hpp"

#include "plane.hpp"
#include "util.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const size_t N = 128;

// World goes from [-width, width] in all axis
const size_t WORLD_WIDTH = 256;

void Ocean::init(GLFWwindow *win) {
    try {
        // TODO: It would be super rad to be able to compile the
        // shaders INTO this executable, at compile time
        auto vert = compile_shader("../src/shader.vert", GL_VERTEX_SHADER);
        auto frag = compile_shader("../src/shader.frag", GL_FRAGMENT_SHADER);
        program = link_program({vert, frag});

        glDeleteShader(vert);
        glDeleteShader(frag);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    this->window = win;

    glUseProgram(program);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    screen_size = vec2((float)viewport[2], (float)viewport[3]);
    screen_center = screen_size / 2.0f;

    // Move the cursor to the center of the screen
    mouse_pos = vec2(screen_center.x, screen_center.y);
    glfwSetCursorPos(win, mouse_pos.x, mouse_pos.y);

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
    glVertexAttribPointer(
        pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char *)nullptr + 0);

    GLuint norm_attrib = 1;
    glEnableVertexAttribArray(norm_attrib);
    glVertexAttribPointer(
        norm_attrib,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (char *)0 + sizeof(vec3));

    // Put the plane into world coordinates
    model = plane.get_model_matrix(WORLD_WIDTH);
    GLint model_attrib = 2;
    glUniformMatrix4fv(model_attrib, 1, GL_FALSE, value_ptr(model));

    // Fix the normal vectors with the inverse transpose
    mat4 model_inv_transp = glm::transpose(glm::inverse(model));
    GLint model_inv_transp_attrib = 14;
    glUniformMatrix4fv(
        model_inv_transp_attrib, 1, GL_FALSE, value_ptr(model_inv_transp));

    // Put the world into camera/view coordinates
    camera = Camera(
        vec3(0.0f, 8.0f, 0.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 1.0f, 0.0f));
    update_view_matrix();

    update_eye_position();

    update_perspective_matrix();

    // LIGHTING
    GLint light_pos_attrib = 5;
    glUniform3fv(
        light_pos_attrib, 1, value_ptr(vec3(0.0f, (float)WORLD_WIDTH, 0.0f)));

    GLint ambient_light_color_attrib = 6;
    glUniform3fv(ambient_light_color_attrib, 1, value_ptr(vec3(0.05f)));

    GLint diffuse_light_color_attrib = 7;
    glUniform3fv(diffuse_light_color_attrib, 1, value_ptr(vec3(1.0f)));

    GLint specular_light_color_attrib = 8;
    glUniform3fv(specular_light_color_attrib, 1, value_ptr(vec3(1.0f)));

    GLint ambient_material_color_attrib = 9;
    vec3 material_color = vec3(155.0 / 255.0, 84.0 / 255.0, 21.0 / 255.0);
    glUniform3fv(ambient_material_color_attrib, 1, value_ptr(material_color));

    GLint diffuse_material_color_attrib = 10;
    glUniform3fv(diffuse_material_color_attrib, 1, value_ptr(material_color));

    GLint specular_material_color_attrib = 11;
    glUniform3fv(specular_material_color_attrib, 1, value_ptr(vec3(0.0f)));

    GLint material_shininess_attrib = 12;
    glUniform1f(material_shininess_attrib, 32.0f);

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
    // Some keys are not detected by the key event handler, so we poll
    // for them manually
    {
        int space = glfwGetKey(this->window, GLFW_KEY_SPACE);
        if (space == GLFW_PRESS) {
            this->pressed_keys[GLFW_KEY_SPACE] = " ";
        } else {
            this->pressed_keys.erase(GLFW_KEY_SPACE);
        }

        int lshift = glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT);
        if (lshift == GLFW_PRESS) {
            this->pressed_keys[GLFW_KEY_LEFT_SHIFT] = "LSHIFT";
        } else {
            this->pressed_keys.erase(GLFW_KEY_LEFT_SHIFT);
        }
    }

    static const float move_speed = (float)WORLD_WIDTH / 10.0f;
    float move_amt = move_speed * (float)dt;

    // TODO: It would be nice if pressing multiple keys didn't change
    // the speed you move at

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
    if (pressed_keys.find(GLFW_KEY_SPACE) != pressed_keys.end()) {
        camera.move(Camera::Direction::UP, move_amt);
    }
    if (pressed_keys.find(GLFW_KEY_LEFT_SHIFT) != pressed_keys.end()) {
        camera.move(Camera::Direction::DOWN, move_amt);
    }

    update_view_matrix();
    update_eye_position();
}

void Ocean::draw() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(
        GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (char *)nullptr + 0);
}

void Ocean::on_key_event(
    GLFWwindow *win,
    int key,
    int scancode,
    int action,
    int mods) {
    (void)win;
    (void)mods;

    if (action == GLFW_PRESS) {
        const char *key_name = glfwGetKeyName(key, scancode);
        if (!key_name) {
            return;
        }

        this->pressed_keys[key] = key_name;

        if (mods & GLFW_MOD_CONTROL) {
            // Holding down CTRL, check for bound key actions
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
        }

    } else if (action == GLFW_RELEASE) {
        this->pressed_keys.erase(key);
    }
}

void Ocean::on_mouse_move(GLFWwindow *win, double xpos, double ypos) {
    (void)win;

    if (is_first_mouse_movement) {
        is_first_mouse_movement = false;
        mouse_pos = vec2(xpos, ypos);
    }

    const float sensitivity = 0.05f;

    // TODO: Can the positions overflow, since they're unbounded?
    vec2 delta = vec2(xpos - mouse_pos.x, mouse_pos.y - ypos);
    delta *= sensitivity;

    camera.rotate2d(delta);
    update_view_matrix();

    // Update state variables
    mouse_pos = vec2(xpos, ypos);
}

void Ocean::on_window_resize(GLFWwindow *win, int width, int height) {
    (void)win;

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
        glm::radians(45.0f), aspect_ratio, 0.1f, (float)WORLD_WIDTH * 2);

    GLint persp_attrib = 4;
    glUniformMatrix4fv(persp_attrib, 1, GL_FALSE, value_ptr(perspective));
}

void Ocean::update_eye_position() {
    GLint eye_pos_attrib = 13;
    glUniformMatrix3fv(
        eye_pos_attrib, 1, GL_FALSE, value_ptr(camera.get_position()));
}
