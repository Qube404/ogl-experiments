#include "shader.h"

Shader::Shader(const char *vertexPath, const char* fragmentPath): ID(0) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED : " << vertexPath << '\n' << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : " << fragmentPath << '\n' << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char *vertexPath, const char *geometryPath, const char* fragmentPath): ID(0) {
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        gShaderFile.open(geometryPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream;
        std::stringstream gShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        geometryCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, geometry, fragment;

    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED : " << vertexPath << '\n' << infoLog << std::endl;
    }

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);

    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED : " << geometryPath << '\n' << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED : " << fragmentPath << '\n' << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::clean() {
    glDeleteProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, glm::vec2 value) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string &name, glm::vec4 value) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
