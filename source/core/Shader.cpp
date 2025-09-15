#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// #ifdef WIN32
// typedef wchar_t CHAR_T;
// #include <windows.h>
// inline std::wstring utf82Unicode(const std::string& s) {
// 	int len = MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,NULL,0);
// 	wchar_t* wstr = new wchar_t[len+1];
// 	len = MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,wstr,len);
// 	wstr[len]=L'\0';
// 	std::wstring ws(L""); ws+=wstr;
// 	delete wstr;
// 	return ws;
// }
// #else
// typedef char CHAR_T;
// #define utf82Unicode(X) (X)
// #endif

Shader::Shader(const std::string& vsPath, const std::string& fsPath) 
    : programID(0), vertShaderID(0), fragShaderID(0) 
{ 
    initializeOpenGLFunctions();
    load(vsPath, fsPath); 
}

Shader::~Shader() { cleanUp(); }

void Shader::load(const std::string& vsPath, const std::string& fsPath){
    cleanUp();
    
    programID = glCreateProgram();
    vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    std::string vertCode = loadText( vsPath );
    std::string fragCode = loadText( fsPath );
    
    if( vertCode.empty() || fragCode.empty() ) {
        std::cerr<<"Shader code is not loaded properly"<<std::endl;
        return;
    }
    
    const GLchar* vshaderCode = vertCode.c_str();
    glShaderSource	( vertShaderID, 1, &vshaderCode, nullptr );
    glCompileShader	( vertShaderID );
    checkCompileErrors(vertShaderID, "VERTEX");
    glAttachShader  ( programID, vertShaderID );
    
    const GLchar* fshaderCode = fragCode.c_str();
    glShaderSource	( fragShaderID, 1, &fshaderCode, nullptr );
    glCompileShader	( fragShaderID );
    checkCompileErrors(fragShaderID, "FRAGMENT");
    glAttachShader  ( programID, fragShaderID );
    
    glLinkProgram( programID );
    checkLinkErrors(programID);
}

void Shader::use(){
    glUseProgram(programID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "[ERROR] Shader Compilation (" << type << "):\n" 
                  << infoLog << std::endl;
    }
}

void Shader::checkLinkErrors(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "[ERROR] Program Linking:\n" 
                  << infoLog << std::endl;
    }
}

std::string Shader::loadText(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open shader file: " << filename << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Shader::cleanUp(){
    if (programID) glDeleteProgram(programID);
    if (vertShaderID) glDeleteShader(vertShaderID);
    if (fragShaderID) glDeleteShader(fragShaderID);
    programID = vertShaderID = fragShaderID = 0;
}

