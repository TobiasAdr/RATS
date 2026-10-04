#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Sprite {
    GLuint texture;
    float x, y;
    float width, height;
    float scale = 1.0f;
    float sheetCol = 0.f;  
    float sheetRow = 0.f;
    int sheetCols = 1;
    int sheetRows = 1;
};

class SpriteRenderer {
public:
    void init();
    void draw(GLuint shaderProgram, const Sprite& sprite, const glm::mat4& viewProj);
    static GLuint loadTexture(const std::string& path);

private:
    GLuint vao, vbo, ebo;
};