#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ocean.hpp"
#include "util.hpp"
#include "plane.hpp"

const size_t N = 128;

// World goes from [-width, width] in all axis
const size_t WORLD_WIDTH = N;

void Ocean::init() {
    try {
        auto vert_shader = compile_shader("../src/shader.vert", GL_VERTEX_SHADER);
        auto frag_shader = compile_shader("../src/shader.frag", GL_FRAGMENT_SHADER);
        program = link_program({vert_shader, frag_shader});

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    glUseProgram(program);

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

    GLuint norm_attrib = 1;
    glEnableVertexAttribArray(norm_attrib);
    glVertexAttribPointer(norm_attrib, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), (char *)0 + sizeof(vec3));

    // Put the plane into world coordinates
    model = plane.get_model_matrix(WORLD_WIDTH);

    GLuint model_attrib = 2;
    glUniformMatrix4fv(model_attrib, 1, GL_FALSE, value_ptr(model));

    // Put the world into camera/view coordinates
    camera.position = vec3(0.0f, 16.0f, 0.0f);
    camera.orientation = angleAxis(0.0f, vec3(0.0f, 0.0f, -1.0f));

    view = make_view_matrix();

    GLuint view_attrib = 3;
    glUniformMatrix4fv(view_attrib, 1, GL_FALSE, value_ptr(view));

    // Perspective transformation
    int viewport_dimensions[4];
    glGetIntegerv(GL_VIEWPORT, viewport_dimensions);
    float aspect_ratio = viewport_dimensions[2] / (float)viewport_dimensions[3];

    perspective = make_perspective_matrix(aspect_ratio);

    GLuint persp_attrib = 4;
    glUniformMatrix4fv(persp_attrib, 1, GL_FALSE, value_ptr(perspective));

    // Compute the model matrix for fixing the normal vectors
    // mat4 model_inv_transp = glm::inverse(model);
    // model_inv_transp = glm::transpose(model_inv_transp);
    // GLint model_inv_transp_attrib = 3;
    // glUniformMatrix4fv(model_inv_transp_attrib, 1,
    //                    GL_FALSE, value_ptr(model_inv_transp));

    index_buffer = 0;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    {
        GLsizeiptr size = indices.size() * sizeof(unsigned int);
        const GLvoid *data = &(indices[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
}

mat4 Ocean::make_view_matrix() {
    float scale = (float)1.0f / (float)WORLD_WIDTH;
    mat4 orient_mat = mat4_cast(camera.orientation);

    // What we want to do is translate, then rotate, then scale --
    // this is expressed backwards in matrix multiplication
    mat4 view_mat = glm::scale(mat4(1.0f), vec3(scale));
    view_mat = orient_mat * view_mat;
    view_mat = glm::translate(view_mat, -camera.position);

    return view_mat;
}

mat4 Ocean::make_perspective_matrix(float aspect_ratio) {
    return glm::perspective(
        radians(45.0f), aspect_ratio, 0.1f, (float)WORLD_WIDTH);
}

void Ocean::cleanup() {
    glDeleteBuffers(1, &index_buffer);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vao);
}

void Ocean::update(double dt) {
    static const float turn_speed = 4.0;

    if (pressed_keys.find(GLFW_KEY_W) != pressed_keys.end()) {
        camera.orientation = rotate(camera.orientation,
                                    -turn_speed * (float)dt,
                                    vec3(1.0f, 0.0f, 0.0f));
    }

    if (pressed_keys.find(GLFW_KEY_S) != pressed_keys.end()) {
        camera.orientation = rotate(camera.orientation,
                                    turn_speed * (float)dt,
                                    vec3(1.0f, 0.0f, 0.0f));
    }

    if (pressed_keys.find(GLFW_KEY_A) != pressed_keys.end()) {
        camera.orientation = rotate(camera.orientation,
                                    turn_speed * (float)dt,
                                    vec3(0.0f, 0.0f, -1.0f));
    }

    if (pressed_keys.find(GLFW_KEY_D) != pressed_keys.end()) {
        camera.orientation = rotate(camera.orientation,
                                    -turn_speed * (float)dt,
                                    vec3(0.0f, 0.0f, -1.0f));
    }

    camera.orientation = normalize(camera.orientation);

    view = make_view_matrix();

    GLuint view_attrib = 3;
    glUniformMatrix4fv(view_attrib, 1, GL_FALSE, value_ptr(view));
}

void Ocean::draw(double dt) {
    (void)dt;

    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (char *)nullptr + 0);
}

void Ocean::on_window_resize(GLFWwindow *window, int width, int height) {
    (void)window;

    float aspect_ratio = width / (float)height;
    perspective = make_perspective_matrix(aspect_ratio);

    GLuint persp_attrib = 4;
    glUniformMatrix4fv(persp_attrib, 1, GL_FALSE, value_ptr(perspective));
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
