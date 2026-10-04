#include "../../include/OpenGL/spriteRenderer.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void SpriteRenderer::init() {
    float verts[] = {
    -0.5f,  0.0f,  0.0f, 1.0f,   
     0.5f,  0.0f,  1.0f, 1.0f,   
     0.5f, -1.0f,  1.0f, 0.0f,   
    -0.5f, -1.0f,  0.0f, 0.0f,   
    };
    unsigned int idx[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void SpriteRenderer::draw(GLuint shaderProgram, const Sprite& sprite, const glm::mat4& viewProj) {

    float uvW = 1.0f / sprite.sheetCols;
    float uvH = 1.0f / sprite.sheetRows;
    float uvX = sprite.sheetCol * uvW;
    float uvY = sprite.sheetRow * uvH;

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProj"), 1, GL_FALSE, &viewProj[0][0]);
    glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), sprite.x, sprite.y);
    glUniform2f(glGetUniformLocation(shaderProgram, "uScale"), sprite.width * sprite.scale, sprite.height * sprite.scale);
    glUniform2f(glGetUniformLocation(shaderProgram, "uUVOffset"), uvX, uvY);
    glUniform2f(glGetUniformLocation(shaderProgram, "uUVSize"), uvW, uvH);
    glUniform2f(glGetUniformLocation(shaderProgram, "uFirePos"), 850.f, 800.f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uAmbient"), 0.05f, 0.05f, 0.06f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

GLuint SpriteRenderer::loadTexture(const std::string& path) {
    sf::Image img;
    if (!img.loadFromFile(path)) {
        std::cerr << "FAILED TO LOAD: " << path << std::endl;
        return 0;
    }

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        img.getSize().x, img.getSize().y, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texId;


}