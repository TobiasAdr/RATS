#include "../../include/OpenGL/OpenGLRenderer.h"
#include <sstream>

static const char *VERT_SRC = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aUV;
    out vec2 vUV;

    uniform mat4 uProjection;

    void main() {
        vUV = aUV;
        gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
    }
)";

static const char *FRAG_SRC = R"(
    #version 330 core
    in vec2 vUV;
    out vec4 FragColor;
    uniform sampler2D uGrid;
    void main() {
        FragColor = texture(uGrid, vUV);
    }
)";

static glm::mat4 viewProjFromSFML(const sf::View &view)
{

    sf::Vector2f c = view.getCenter();
    sf::Vector2f s = view.getSize();

    float left = c.x - s.x / 2.0f;
    float right = c.x + s.x / 2.0f;
    float top = c.y - s.y / 2.0f;
    float bottom = c.y + s.y / 2.0f;

    return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

std::string loadShaderSource(const std::string &path)
{
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

OpenGLRenderer::OpenGLRenderer(sf::RenderWindow &window)
    : VAO(0), VBO(0), shaderProgram(0), window(window)
{
}


void OpenGLRenderer::init(OpenGLElementSimulation &simulation1, OpenGLElementSimulation &simulation2, OpenGLElementSimulation &simulation3, EnemyManagerOpenGL &enemyManager, TileRendererOpenGL &tileRenderer, firePlaceOpenGL &firePlace)
{

    
    glewExperimental = GL_TRUE;
    glewInit();

    
    shaderProgram = buildProgram(VERT_SRC, FRAG_SRC);

    std::string vertSrc = loadShaderSource("shaders/enemy.vert");
    std::string fragSrc = loadShaderSource("shaders/enemy.frag");
    this->enemyShader = buildProgram(vertSrc, fragSrc);

    std::string tileVert = loadShaderSource("shaders/quad.vert");
    std::string tileFrag = loadShaderSource("shaders/quad.frag");

    std::string windowVert = loadShaderSource("shaders/window.vert");
    std::string windowFrag = loadShaderSource("shaders/window.frag");

    this->windowShader = buildProgram(windowVert, windowFrag);

    this->tileShader = buildProgram(tileVert, tileFrag);

    float mapW = this->window.getSize().x;
    float mapH = this->window.getSize().y;

    float vertices[] = {
        0.f, 0.f, 0.0f, 0.0f,
        0.f, mapH, 0.0f, 1.0f,
        mapW, mapH, 1.0f, 1.0f,

        0.f, 0.f, 0.0f, 0.0f,
        mapW, mapH, 1.0f, 1.0f,
        mapW, 0.f, 1.0f, 0.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    
    simulation1.init();
    simulation2.init();
    simulation3.init();

    tileRenderer.init();

    firePlace.init();

    std::string spriteVert = loadShaderSource("shaders/sprite.vert");
    std::string spriteFrag = loadShaderSource("shaders/sprite.frag");
    this->spriteShader = buildProgram(spriteVert, spriteFrag);

    spriteRenderer.init();

    sprites = {
        {SpriteRenderer::loadTexture("resources/skeletonGPT.png"), 830.f, 770.f, 16.f, 32.f},
    };
    GLuint sheet = SpriteRenderer::loadTexture("resources/props.png");

    sprites2 = {
        {sheet, 900.f, 800.f, 128.f, 128.f, 0.17f, 1.4f, 0, 10, 6},
        {sheet, 910.f, 810.f, 128.f, 128.f, 0.17f, 5.8f, 0.2, 10, 6},
        {sheet, 800.f, 770.f, 128.f, 128.f, 0.17f, 4.f, 5.f, 10, 6}, 
        {sheet, 795.f, 775.f, 128.f, 128.f, 0.17f, 4.f, 5.f, 10, 6}, 
        {sheet, 810.f, 770.f, 128.f, 128.f, 0.17f, 4.f, 5.f, 10, 6}, 
        {sheet, 805.f, 775.f, 128.f, 128.f, 0.17f, 4.f, 5.f, 10, 6}, 

        {sheet, 873.0f, 835.0f, 128.f, 128.f, 0.17f, 8.f, 1.4f, 10, 6},

        
        
    };
}

void OpenGLRenderer::render(OpenGLElementSimulation &simulation1, OpenGLElementSimulation &simulation2, OpenGLElementSimulation &simulation3, EnemyManagerOpenGL &enemyManager, TileRendererOpenGL &tileRenderer, firePlaceOpenGL &firePlace)
{

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tileRenderer.render(this->tileShader, this->windowShader, window.getView());

    glm::mat4 viewProj = viewProjFromSFML(window.getView());

    for (auto &sprite : sprites)
    {
        spriteRenderer.draw(spriteShader, sprite, viewProj);
    }
    for (auto &sprite : sprites2)
    {

        spriteRenderer.draw(spriteShader, sprite, viewProj);
    }

    enemyManager.render(this->enemyShader, window, firePlace.getPosX(), firePlace.getPosY());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    firePlace.render(window.getView()); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);
    
    sf::Transform sfTransform = window.getView().getTransform();
    const float *sfMatrix = sfTransform.getMatrix();

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "uProjection"),
        1, GL_FALSE, sfMatrix);

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, simulation1.getTexture());
    glUniform1i(glGetUniformLocation(shaderProgram, "uGrid"), 0);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, simulation2.getTexture());
    glUniform1i(glGetUniformLocation(shaderProgram, "uGrid"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, simulation3.getTexture());
    glUniform1i(glGetUniformLocation(shaderProgram, "uGrid"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

GLuint OpenGLRenderer::compileShader(GLenum type, const std::string &src)
{

    GLuint shader = glCreateShader(type);
    const char *c = src.c_str();
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);

    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "[OpenGLRenderer] Shader-fel: " << log << "\n";
    }

    return shader;
}

GLuint OpenGLRenderer::buildProgram(const std::string &vertSrc, const std::string &fragSrc)
{

    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}