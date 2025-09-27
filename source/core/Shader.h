#pragma once
#include <QOpenGLFunctions>
#include <string>
#include <glm/glm.hpp>

class Shader : protected QOpenGLFunctions{
public:
    Shader(const std::string& vsPath, const std::string& fsPath);
    ~Shader();

    void load(const std::string& vsPath, const std::string& fsPath);
    void use();

    // uniform setters
    void setMat4(const std::string &name, const glm::mat4 &mat);
    void setVec3(const std::string &name, const glm::vec3 &vec);
    void setInt(const std::string &name, const int value);
    void setFloat(const std::string& name, const float value);
    void setFloat2(const std::string& name, const float value1, const float value2);

private:
    GLuint programID;
    GLuint vertShaderID, fragShaderID;

    void cleanUp();
    static std::string loadText(const std::string& filename);

    void checkCompileErrors(GLuint shader, const std::string& type);
    void checkLinkErrors(GLuint program);
};