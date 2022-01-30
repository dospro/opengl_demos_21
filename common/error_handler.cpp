#include "error_handler.h"

#include <stdio.h>
#include <stdlib.h>

void printShaderLog(GLuint shader) {
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        char *log = (char *)malloc(len);
        int written = 0;
        glGetShaderInfoLog(shader, len, &written, log);
        printf("Shader info log: %s\n", log);
        free(log);
    }
}

void printProgramLog(GLint program) {
    int len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        char *log = (char *)malloc(len);
        int written = 0;
        glGetProgramInfoLog(program, len, &written, log);
        printf("Program info log: %s\n", log);
        free(log);
    }
}

int checkOpenGLError() {
    int foundError = 0;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR) {
        printf("OpenGL error code: %d\n", glErr);
        foundError = 1;
        glErr = glGetError();
    }
    return foundError;
}