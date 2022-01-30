#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "common/error_handler.h"
#include "common/shader_utils.h"

int load_texture(const char *filename, GLuint *id);

int main(int argc, char *argv[]) {
    float vertices1[] = {
            -0.9, -0.9, 0.01,
            -0.9, 0.9, 0.01,
            0.9, 0.9, 0.01,
            0.9, -0.9, 0.01,
    };

    float vertices2[] = {
            0.2, 0.2, 0.05,
            0.2, 0.8, 0.05,
            0.8, 0.8, 0.05,

            0.8, 0.8, 0.05,
            0.8, 0.2, 0.05,
            0.2, 0.2, 0.05,
    };

    float tex_coords[] = {
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
    };

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error iniciando SDL");
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    auto *screen = SDL_CreateWindow(
            "OpenGL PNG Demo",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800, 600,
            SDL_WINDOW_OPENGL);
    auto *gl_context = SDL_GL_CreateContext(screen);
    glewInit();

    GLuint shader = load_shaders("../vert_shader.glsl", "../frag_shader.glsl");

    glViewport(0, 0, 800, 600);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Load texture
    glActiveTexture(GL_TEXTURE0); // Make sure we are using texture unit 0
    GLuint texture1_id, texture2_id;
    load_texture("bg_1.png", &texture1_id);
    load_texture("bbg1.png", &texture2_id);

    bool is_running = true;
    SDL_Event event;
    while (is_running) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    is_running = false;
                }
                break;

            default:
                break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices1);

        // Now load the texture
        glEnableClientState(GL_TEXTURE_COORD_ARRAY); // When drawing arrays of vertex we also send texture coords
        glBindTexture(GL_TEXTURE_2D, texture1_id);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);

        //glUniform1i(glGetUniformLocation(shader, "samp"), 1);
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        SDL_GL_SwapWindow(screen);
    }
    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(1, &texture2_id);
    glDeleteTextures(1, &texture1_id);
    glDeleteProgram(shader);

    SDL_GL_DeleteContext(gl_context);
    SDL_Quit();
    return 0;
}

int load_texture(const char *filename, GLuint *id) {
    SDL_Surface *image = IMG_Load(filename);
    if (!image) {
        printf("No se pudo cargar imagen1");
        return 1;
    }

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(
            GL_TEXTURE_2D,
            0, GL_RGBA,
            image->w,
            image->h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    *id = texture_id;
    SDL_FreeSurface(image);
    return 0;
}