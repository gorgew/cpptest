#include <glad/glad.h>
#include "shaderprogram.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/format.h>

#include <memory>
#include <VertexArrayManager.hpp>
#include <TextureManager.cpp>
#include <ShaderManager.hpp>

int WINDOW_HEIGHT = 800;
int WINDOW_WIDTH = 600;

/*
float vertices[] = {
        //vec coords              color           tex coords
        -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.0f,  0.5f, 0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.0f,  0.5f, 0.0f, 0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
    };
*/

//Copy and pasted
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f
};

int main(void) {
    fmt::print("");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetSwapInterval(1);

    SDL_Window* window = SDL_CreateWindow("Opengl", 100, 100, WINDOW_HEIGHT, WINDOW_WIDTH, 
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cerr << "OpenGL error";
        exit(1);
    }
    
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    glEnable(GL_DEPTH_TEST);

    /*
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    */
    auto va_manager = std::make_unique<VertexArrayManager>();

    va_manager->add_array("cube", vertices, sizeof(vertices), {3, 3, 2});
    GLuint VAO = va_manager->get_array_id("cube");

    auto tex_man = std::make_unique<TextureManager>();
    //tex_man->add_texture("../resources/awesomeface.png", "awesome-face");
    //std::cout << "TexID: " << tex_man->get_id("awesome-face") << "\n"; 

    tex_man->add_texture("../resources/blank.png", "blank");
    //tex_man->add_2d_array_texture("array-tex-test", "../resources/array-tex-test.png", 1, 1, 4);
    //tex_man->add_2d_array_texture("array-tex-test", "../resources/nums-h.png", 32, 32, 6);
    tex_man->add_2d_array_texture("array-tex-test", "../resources/NumsPacked.png", 32, 32, 6);
    //tex_man->add_2d_array_texture("array-tex-test", "../resources/single-px-h.png", 1, 1, 8);
    //tex_man->add_2d_array_texture("array-tex-test", "../resources/2-2-px-pack.png", 2, 2, 9);
    //tex_man->add_2d_array_texture("array-tex-test", "../resources/px.png", 1, 1, 1);
    auto shader_man = std::make_unique<ShaderManager>();
    shader_man->add_shader("mvp.vert", "../resources/mvp.vert", GL_VERTEX_SHADER);

    shader_man->add_shader("array-tex.frag", "../resources/array-tex.frag", GL_FRAGMENT_SHADER);
    shader_man->add_program("array-tex", {"mvp.vert", "array-tex.frag"});

    std::cout << "shadere_program: " << shader_man->get_program_id("array-tex") << "\n";

    shader_man->use("array-tex");
    glUniform1i(glGetUniformLocation(shader_man->get_program_id("array-tex"), "iTexture"), 0);
    glUniform1f(glGetUniformLocation(shader_man->get_program_id("array-tex"), "layer"), 5.0);
    
    //bool x = glIsProgram(shaderProgram.getID());
    bool x = glIsProgram(shader_man->get_program_id("array-tex"));
    std::cout << "is program: " << x << "\n";

    int loc = glGetUniformLocation(shader_man->get_program_id("array-tex"), "layer");
    //int loc = glGetUniformLocation(shaderProgram.getID(), "texture2");
    std::cout << "loc " << loc << "\n";
    int a[1] {69};
    glGetUniformiv(shader_man->get_program_id("array-tex"), glGetUniformLocation(shader_man->get_program_id("array-tex"), "iTexture"), a);
    //glGetUniformiv(shaderProgram.getID(), glGetUniformLocation(shaderProgram.getID(), "texture2"), a);
    std::cout <<"uniform value " << a[0] << "\n";

    float b[1] {69.0};
    glGetUniformfv(shader_man->get_program_id("array-tex"), glGetUniformLocation(shader_man->get_program_id("array-tex"), "layer"), b);
    //glGetUniformiv(shaderProgram.getID(), glGetUniformLocation(shaderProgram.getID(), "texture2"), a);
    std::cout <<"b value" << b[0] << "\n";
    //Model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    //View
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    //Projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    //Matrices to translate the cube
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    srand(1);

    SDL_Event windowEvent;
    while (true) {
        
        if (SDL_PollEvent(&windowEvent)) {

            if (windowEvent.type == SDL_QUIT) {
                break;

            } else if (windowEvent.type == SDL_KEYUP && 
                    windowEvent.key.keysym.sym == SDLK_ESCAPE) {
                break;

            } else if (windowEvent.type == SDL_WINDOWEVENT) {

                if (windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    WINDOW_HEIGHT = windowEvent.window.data1;
                    WINDOW_WIDTH = windowEvent.window.data2;
                    //std::cout << "WINDOW_HEIGHT: " << WINDOW_HEIGHT << "\n";
                    //std::cout << "WINDOW_WIDTH: " << WINDOW_WIDTH << "\n";
                    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
                }
            }
        }
        //float time = SDL_GetTicks() / 100000.0f;
        //std::cout << "TIME (S): " << time << std::endl;
        //model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        projection = glm::perspective(glm::radians(60.0f), (float) WINDOW_HEIGHT / (float) WINDOW_WIDTH, 0.1f, 10.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("array-tex"), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("array-tex"), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("array-tex"), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
 
        int id = tex_man->get_id("array-tex-test");
        glBindTexture(GL_TEXTURE_2D_ARRAY, id);

        glBindVertexArray(VAO);

        for (glm::vec3 cubePos : cubePositions) {
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePos);

            //float angle = ((rand() % 359) + 1.0f);
            //float angle = glm::radians(90.0f);
            
            glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("array-tex"), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        SDL_GL_SwapWindow(window);

        SDL_Delay(16);
    }

    //shaderProgram.close();
    //glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window); 
    SDL_Quit();

    return 0;
}