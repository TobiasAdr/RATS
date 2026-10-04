#include "OpenGLElementSimulation.h"
#include "EnemyManagerOpenGL.h"
#pragma once
#include "TileRendererOpenGL.h"
#include "firePlaceOpenGL.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "SpriteRenderer.h"


class OpenGLRenderer {
public:


    void init(OpenGLElementSimulation& simulation1, OpenGLElementSimulation& simulation2, OpenGLElementSimulation& simulation3, EnemyManagerOpenGL& enemyManager, TileRendererOpenGL& tileRenderer, firePlaceOpenGL& firePlace);

    void render(OpenGLElementSimulation& simulation1, OpenGLElementSimulation& simulation2, OpenGLElementSimulation& simulation3, EnemyManagerOpenGL& enemyManager, TileRendererOpenGL& tileRenderer, firePlaceOpenGL& firePlace);

    OpenGLRenderer(sf::RenderWindow& window);

private:
    GLuint VAO, VBO, shaderProgram, enemyShader;
    GLuint tileShader;

    
    SpriteRenderer spriteRenderer;
    GLuint spriteShader;
    std::vector<Sprite> sprites;
    std::vector<Sprite> sprites2;

    GLuint windowShader;

    sf::RenderWindow& window;

    GLuint compileShader(GLenum type, const std::string& src);
    GLuint buildProgram(const std::string& vertSrc, const std::string& fragSrc);
};

