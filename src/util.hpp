#pragma once

#include <fstream>
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::array;
using std::unique_ptr;
using std::vector;

unique_ptr<vector<char>>
read_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    std::streamsize file_size = file.tellg();
    auto buffer = std::make_unique<vector<char>>(file_size + 1);

    file.seekg(0);
    file.read(buffer->data(), file_size);
    file.close();

    buffer->push_back('\0');

    return buffer;
}

GLuint compile_shader(const std::string &filename, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);

    const auto source_vec = read_file(filename);
    const GLchar *shader_sources[] = {source_vec->data()};

    glShaderSource(shader, 1, shader_sources, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char info_log[512];
        GLsizei bytes_written;
        glGetShaderInfoLog(shader, 512, &bytes_written, info_log);
        info_log[bytes_written] = '\0';

        std::string err_msg = "Compilation of '";
        err_msg.append(filename);
        err_msg.append("' failed: ");
        err_msg.append(info_log);

        throw std::runtime_error(err_msg);
    }

    return shader;
}

GLuint link_program(const std::vector<GLuint> &shaders) {
    GLuint program = glCreateProgram();

    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char info_log[512];
        GLsizei bytes_written;
        glGetProgramInfoLog(program, 512, &bytes_written, info_log);
        info_log[bytes_written] = '\0';

        std::string err_msg = "Program linking failed: ";
        err_msg.append(info_log);

        throw std::runtime_error(err_msg);
    }

    return program;
}
