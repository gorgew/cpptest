#include "Graphics.hpp"
#include "GLWrappers/glHelpers.hpp"
#include "Scenes/StartMenuScene.hpp"


Graphics::Graphics() {}

Graphics::Graphics(std::shared_ptr<SceneManager> s) {
    //Check if Glad loads
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cerr << "OpenGL error";
        exit(1);
    }
    //Set private variable
    sceneManagerPtr = s;
    
    //Add a scene
    auto startMenu = std::make_shared<StartMenuScene>();
    sceneManagerPtr->openScene(startMenu);

    glEnable(GL_DEPTH_TEST);
    std::cout << "new Graphics \n";
    offscreenBuffer.reset(new FrameBufferObject());
    offscreenBuffer->addTexture2D();
    offscreenBuffer->addDepthStencilRenderBuffer();
    offscreenBuffer->checkCompiled();
    
    defaultBufferProgram.reset(new ShaderProgram("Shaders/vcoord_texcoord.vert", 
            "Shaders/texcoord.frag"));
    
    std::vector<float> screenVerts;
    glHelpers::rectArray(screenVerts, 2.0f, 2.0f);
    
    defaultBufferQuadArray.reset(new VertexArrayObject());
    defaultBufferQuadArray->addBuffer32(screenVerts.data(), 
            screenVerts.size() * sizeof(float), GL_STATIC_DRAW);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    offscreenBuffer->activate();
}

void Graphics::draw() {
    
    offscreenBuffer->activate();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    sceneManagerPtr->drawScene();
    //std::cout<<"drawing\n";
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(defaultBufferQuadArray->getVAOID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, offscreenBuffer->texID);
    defaultBufferProgram->use();
    defaultBufferProgram->setUniform("iTexture", 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
}

void Graphics::close() {
    
}

Graphics::~Graphics() {
    std::cout<<"Graphics dying \n";
}