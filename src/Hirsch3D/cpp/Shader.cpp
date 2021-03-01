#include "../Include/Hirsch3D/core/Shader.hpp"


#include <fstream>
#include <cstdio>
#include <iostream>
#include "../Include/Hirsch3D/error/Exception.hpp"

bool h3d::Shader::load(std::string vertexSrc, std::string fragmentSrc, std::string geometryShader) {
    if(geometryShader == "nogeometryshader") useGeometryShader = false;
    else useGeometryShader = true;
    this->shaderId = createShader(vertexSrc.data(), fragmentSrc.data(), geometryShader.data());
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
		throw h3d::Exception("Shader file could not be found");
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

GLuint h3d::Shader::createShader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile) {
    std::string vertexShaderSrc = parse(vertexShaderFile).data();
    std::string fragmentShaderSrc = parse(fragmentShaderFile).data();
    
    std::string geometryShaderSrc;
    if(useGeometryShader)
    geometryShaderSrc = parse(geometryShaderFile).data();
    GLuint program = glCreateProgram();
    GLuint vs = compile(vertexShaderSrc, GL_VERTEX_SHADER);
    GLuint fs = compile(fragmentShaderSrc, GL_FRAGMENT_SHADER);
    GLuint gs = 0; 
    if(useGeometryShader)
    gs = compile(geometryShaderSrc, GL_GEOMETRY_SHADER);

    glAttachShader(program, vs);
    
    if(useGeometryShader)
        glAttachShader(program, gs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    if(useGeometryShader)
    glDetachShader(program, gs);
    if(useGeometryShader)
    glDeleteShader(gs);
   
    

    return program;
}