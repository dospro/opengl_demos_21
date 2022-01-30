#include "shader_utils.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"

char *load_shader(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);    
    char *text = (char *)malloc(size);
    fread(text, size, 1, file);
    return text;
}

int load_shaders(const char *vertex_shader_filename, const char *fragment_shader_filename) {
    char *vert_shader_source = load_shader(vertex_shader_filename);
    char *frag_shader_source = load_shader(fragment_shader_filename);
    if (vert_shader_source == NULL || frag_shader_source == NULL) {
        printf("Error loading shader files\n");
        exit(1);
    }


    GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert_shader_id, 1, &vert_shader_source, NULL);
    glShaderSource(frag_shader_id, 1, &frag_shader_source, NULL);

    glCompileShader(vert_shader_id);
    checkOpenGLError();
    GLint status = 0;
    glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &status);
    if(status != 1) {
        printf("Failed to compile vertex shader\n");
        printShaderLog(vert_shader_id);
    }

    glCompileShader(frag_shader_id);
    glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &status);
    if(status != 1) {
        printf("Failed to compile fragment shader\n");
        printShaderLog(frag_shader_id);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader_id);
    glAttachShader(program, frag_shader_id);
    glLinkProgram(program);
    checkOpenGLError();
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status != 1) {
        printf("Failed to link shaders\n");
        printProgramLog(program);
    }

    free(vert_shader_source);
    free(frag_shader_source);

    return program;
}