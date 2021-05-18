/*
 * Draw 2 triangles next to each other
 */
#include <memory>
#include <iostream>
#include <glad/glad.h>

#include <KeyEventHandler.hpp>
#include <MouseEventHandler.hpp>

#include <SDL.h>
#include <SDL_opengl.h>

int WINDOW_HEIGHT = 800;
int WINDOW_WIDTH = 600;

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main() 
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Opengl", 
                                        100, 
                                        100, 
                                        WINDOW_HEIGHT, 
                                        WINDOW_WIDTH, 
                                        SDL_WINDOW_OPENGL | 
                                        SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cerr << "OpenGL error";
        exit(1);
    }
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        
         0.0f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.5f,  -0.5f, 0.0f
    };
    
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int shaderSuccess;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderSuccess);

    if(!shaderSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr<<"VERTEX SHADER COMPILATION FAILURE\n"<<infoLog<<std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    auto KeyEventHandler_ptr = std::make_unique<KeyEventHandler>();

    std::function<bool(SDL_Event)> escape = 
        [](SDL_Event e) { 
            if (e.key.keysym.sym == SDLK_a) {
                std::cout << "a was pressed!\n";
            }
            
            return true;
        };

    const std::function<void()> a_down = 
        []() { 
            std::cout << " a was pressed !\n";
        };
    //KeyEventHandler_ptr->add_handler(escape);
    const std::function<void()> b_hold_down = 
        []() { 
            std::cout << " b was pressed !\n";
        };

    const std::function<void()> b_hold_up = 
        []() { 
            std::cout << " b was released !\n";
        };

    KeyEventHandler_ptr->add_keydown_handler(SDLK_a, a_down);
    KeyEventHandler_ptr->add_held_key_handler(SDLK_b, b_hold_down, b_hold_up);

    auto MouseEventHandler_ptr = std::make_unique<MouseEventHandler>();
    
    SDL_Event event;
    while(true)
    {   
        KeyEventHandler_ptr->handle_event(event);
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) break;
            else if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                MouseEventHandler_ptr->handle(event);
            }
            //Resizing I guess
            else if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    WINDOW_HEIGHT = event.window.data1;
                    WINDOW_WIDTH = event.window.data2;
                    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
                }
            }
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        SDL_GL_SwapWindow(window);
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window); 
    SDL_Quit();
    return 0;
}