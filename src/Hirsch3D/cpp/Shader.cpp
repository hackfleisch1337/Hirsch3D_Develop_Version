#include "../Include/Hirsch3D/core/Shader.hpp"


#include <fstream>
#include <cstdio>
#include <iostream>

bool h3d::Shader::load(std::string vertexSrc, std::string fragmentSrc) {
    this->shaderId = createShader(vertexSrc.data(), fragmentSrc.data());
    this->hasLoaded = true;
    return true;
}

h3d::Shader::~Shader() {
    glDeleteProgram(shaderId);
}

void h3d::Shader::bind() const{
    glUseProgram(shaderId);
}

void h3d::Shader::unbind() const {
    glUseProgram(0);
}

GLuint h3d::Shader::compile(std::string shaderSrc, GLenum type) {
    GLuint id = glCreateShader(type);
    const char* srcCode = shaderSrc.c_str();
    glShaderSource(id, 1, &srcCode, 0);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
        int lenght = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* msg = new char[lenght];

        glGetShaderInfoLog(id, lenght, &lenght, msg);

        std::cout << "\033[1;31m[FAILED] Shader compilation error: " << msg << "!\033[0m" << std::endl;
        delete[] msg;
        return 0;
    }
    return id;
}

#ifndef GREEN
#define YELLOW "\033[93m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define RESET_CLR "\x1B[0m"
#endif

std::string h3d::Shader::parse(const char* filename) {
    FILE* file;
    file = fopen(filename, "rb");
	if (file == nullptr) {
		std::cout << RED << "[FAILED] File " << filename << " could not be found" << RESET_CLR << std::endl;
		return "";
	}


    std::string contents;
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);
    contents.resize(filesize);

    fread(&contents[0], 1, filesize, file);
    fclose(file);
    //std::cout << contents << std::endl;
    return contents;
}

GLuint h3d::Shader::createShader(const char* vertexShaderFile, const char* fragmentShaderFile) {
    std::string vertexShaderSrc = parse(vertexShaderFile).data();
    std::string fragmentShaderSrc = parse(fragmentShaderFile).data();
    GLuint program = glCreateProgram();
    GLuint vs = compile(vertexShaderSrc, GL_VERTEX_SHADER);
    GLuint fs = compile(fragmentShaderSrc, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}