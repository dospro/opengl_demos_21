#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>

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


int load_shaders(const char *vertex_shader_filename, const char *fragment_shader_filename);
int load_texture(const char *filename, GLuint *id);

int main(int argc, char *argv[])
{
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

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
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

    //GLuint vao[1];
    GLuint vbo[2];
    //glGenVertexArrays(1, vao);
    //glBindVertexArray(vao[0]);
    //glGenBuffers(2, vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);

    glViewport(0, 0, 800, 600);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glEnable(GL_DEPTH_TEST);

    // Load texture
    //glEnable(GL_TEXTURE_2D); // Should no be needed?
    //glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0); // MAke sure we are using texture unit 0    
    GLuint texture1_id, texture2_id;
    load_texture("bg_1.png", &texture1_id);
    load_texture("bbg1.png", &texture2_id);

    bool is_running = true;
    SDL_Event event;
    while (is_running)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                is_running = false;
            }
            break;

        default:
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);
        //glBindBuffer(GL_ARRAY_BUFFER, 0); // Disable buffer

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices1);

        // Now load the texture
        glEnableClientState(GL_TEXTURE_COORD_ARRAY); // When drawing arrays of vertex we also send texture coords
        glBindTexture(GL_TEXTURE_2D, texture1_id);
        glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);

        //glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //glEnableVertexAttribArray(0);        
        
        glUniform1i(glGetUniformLocation(shader, "samp"), 0);
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        SDL_GL_SwapWindow(screen);
    }
    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(1, &texture2_id);
    glDeleteTextures(1, &texture1_id);
    glDeleteBuffers(2, vbo);
    glDeleteProgram(shader);

    SDL_GL_DeleteContext(gl_context);
    SDL_Quit();
    return 0;
}

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