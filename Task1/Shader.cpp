#include "Shader.h"

Shader::Shader(GLenum type, const std::string& source) {
    shaderID = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, log);
        std::cerr << "❌ Shader compilation failed:\n" << log << std::endl;
    }
}

void Shader::attachToProgram(GLuint programID) const {
    glAttachShader(programID, shaderID);
}

std::string Shader::loadSourceFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}