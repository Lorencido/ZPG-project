#include "ShaderProgram.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const std::vector<Shader>& shaders) : camera(nullptr){
    programID = glCreateProgram();

    for (const auto& shader : shaders) {
        shader.attachToProgram(programID);
    }

    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(programID, 512, nullptr, log);
        std::cerr << "❌ Program linking failed:\n" << log << std::endl;
    }
}

void ShaderProgram::UseShader() const {
    glUseProgram(programID);
}

void ShaderProgram::attachCamera(Camera* cam) {
    camera = cam;
}


void ShaderProgram::setViewMatrix(const glm::mat4& view) const {
    GLint loc = glGetUniformLocation(programID, "viewMatrix");
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
}

void ShaderProgram::setProjectionMatrix(const glm::mat4& projection) const {
    GLint loc = glGetUniformLocation(programID, "projectionMatrix");
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
}


void ShaderProgram::setUniformMatrix4fv(const char* name, const GLfloat* value) const {
    GLint loc = glGetUniformLocation(programID, name);
    if (loc == -1) {
        std::cerr << "⚠️ Warning: uniform \"" << name << "\" not found in shader.\n";
        return;
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void ShaderProgram::onCameraChanged(const glm::mat4& view, const glm::mat4& proj) {
    UseShader();
    //std::cout << "werks" << std::endl;
    GLint viewLoc = glGetUniformLocation(programID, "viewMatrix");
    GLint projLoc = glGetUniformLocation(programID, "projectionMatrix");
    if (viewLoc >= 0) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (projLoc >= 0) glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    if (loc >= 0) glUniform3fv(loc, 1, &value[0]);

    //std::cout << "Setting " << name << " = " << value.x << ", " << value.y << ", " << value.z << std::endl;
}

void ShaderProgram::setFloat(const std::string& name, float value) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    if (loc >= 0) glUniform1f(loc, value);
}

void ShaderProgram::setInt(const std::string& name, int value) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    if (loc >= 0) glUniform1i(loc, value);
}

void ShaderProgram::setUniformBool(const std::string& name, bool value) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    if (loc >= 0) glUniform1i(loc, value);
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = Shader::loadSourceFromFile(vertexPath);
    std::string fragmentSource = Shader::loadSourceFromFile(fragmentPath);
    Shader vShader(GL_VERTEX_SHADER, vertexSource);
    Shader fShader(GL_FRAGMENT_SHADER, fragmentSource);
    *this = ShaderProgram({vShader, fShader});
}