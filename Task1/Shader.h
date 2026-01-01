#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
    Shader(GLenum type, const std::string& source);
    void attachToProgram(GLuint programID) const;
    static std::string loadSourceFromFile(const std::string& path);

private:
    GLuint shaderID;
};