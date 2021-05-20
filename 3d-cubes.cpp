#include <glad/glad.h>
#include "shaderprogram.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
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
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /*
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // int imgWidth, imgHeight, numChannels;

    // stbi_set_flip_vertically_on_load(true);
    
    // unsigned int texture1, texture2;
    // glGenTextures(1, &texture1);
    // glBindTexture(GL_TEXTURE_2D, texture1);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // unsigned char *data1 = stbi_load("resources/container.jpg", &imgWidth, &imgHeight, &numChannels, 0);
    // if (data1) {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    //     std::cout<<"TEXTURE1 LOADED\n";
    // } else {
    //     std::cout<<"TEXTURE1 LOADING FAILURE\n";
    //     return 1;
    // }

    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    // unsigned char *data2 = stbi_load("resources/awesomeface.png", &imgWidth, &imgHeight, &numChannels, 0);
    // if (data2) {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    //     std::cout<<"TEXTURE2 LOADED\n";
    // } else {
    //     std::cout<<"TEXTURE2 LOADING FAILURE\n";
    //     return 1;
    // }
    
    // stbi_image_free(data1);
    // stbi_image_free(data2);

    auto tex_man = std::make_unique<TextureManager>();
    tex_man->add_texture("../resources/awesomeface.png", "awesome-face");
    std::cout << "TexID: " << tex_man->get_id("awesome-face") << "\n"; 

    tex_man->add_texture("../resources/blank.png", "blank");

    auto shader_man = std::make_unique<ShaderManager>();
    shader_man->add_shader("mvp.vert", "../resources/mvp.vert", GL_VERTEX_SHADER);
    shader_man->add_shader("mix.frag", "../resources/mix.frag", GL_FRAGMENT_SHADER);
    shader_man->add_program("my_program", {"mvp.vert", "mix.frag"});

    std::cout << "shadere_program: " << shader_man->get_program_id("my_program") << "\n";
    
    ShaderProgram shaderProgram("resources/mvp.vert", "resources/mix.frag");
    shaderProgram.use();
    shaderProgram.setUniform("texture1", 0);
    //shaderProgram.setUniform("texture2", 1);
    glUniform1i(glGetUniformLocation(shaderProgram.getID(), "texture2"), 1);
    
    shader_man->use("my_program");
    glUniform1i(glGetUniformLocation(shader_man->get_program_id("my_program"), "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader_man->get_program_id("my_program"), "texture2"), 1);
    
    //bool x = glIsProgram(shaderProgram.getID());
    bool x = glIsProgram(shader_man->get_program_id("my_program"));
    std::cout << "is program: " << x << "\n";

    //int loc = glGetUniformLocation(shader_man->get_program_id("my_program"), "texture2");
    int loc = glGetUniformLocation(shaderProgram.getID(), "texture2");
    std::cout << loc << "\n";
    int a[1] {69};
    glGetUniformiv(shader_man->get_program_id("my_program"), glGetUniformLocation(shader_man->get_program_id("my_program"), "texture2"), a);
    //glGetUniformiv(shaderProgram.getID(), glGetUniformLocation(shaderProgram.getID(), "texture2"), a);
    std::cout <<"uniform value " << a[0];
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
        float time = SDL_GetTicks() / 100000.0f;
        //std::cout << "TIME (S): " << time << std::endl;
        //model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        projection = glm::perspective(glm::radians(60.0f), (float) WINDOW_HEIGHT / (float) WINDOW_WIDTH, 0.1f, 10.0f);

        shaderProgram.setUniformMatrix4("model", GL_FALSE, glm::value_ptr(model));
        shaderProgram.setUniformMatrix4("view", GL_FALSE, glm::value_ptr(view)); 
        shaderProgram.setUniformMatrix4("projection", GL_FALSE, glm::value_ptr(projection)); 
        glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("my_program"), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("my_program"), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("my_program"), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_man->get_id("blank"));
        glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //glBindTexture(GL_TEXTURE_2D, texture2);
        glBindTexture(GL_TEXTURE_2D, tex_man->get_id("awesome-face"));

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO);

        for (glm::vec3 cubePos : cubePositions) {
            
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePos);

            //float angle = ((rand() % 359) + 1.0f);
            float angle = glm::radians(90.0f);
            model = glm::rotate(model, angle, glm::vec3(0.5f, 0.5f, 1.0f));
            shaderProgram.setUniformMatrix4("model", GL_FALSE, glm::value_ptr(model));

            glUniformMatrix4fv(glGetUniformLocation(shader_man->get_program_id("my_program"), "model"), 1, GL_FALSE, glm::value_ptr(model));
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