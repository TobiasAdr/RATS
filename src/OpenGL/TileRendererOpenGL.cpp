#include "../../include/OpenGL/TileRendererOpenGL.h"
#include <iostream>
#include <random>

TileRendererOpenGL::TileRendererOpenGL() {
};

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

static GLuint loadTexture(const std::string &path)
{
    sf::Image img;
    if (!img.loadFromFile(path))
    {
        std::cerr << "FAILED TO LOAD: " << path << std::endl;
        return 0;
    }
    std::cout << "Loaded: " << path
              << " (" << img.getSize().x << "x" << img.getSize().y << ")" << std::endl;

    img.flipVertically();

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 img.getSize().x, img.getSize().y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texId;
}

void TileRendererOpenGL::init()
{

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    floorTexture = loadTexture("resources/soilgemini4.png");
    mossTexture = loadTexture("resources/wallGemini2.png");
    windowTexture = loadTexture("resources/windowgem4.png");

    float floorVerts[] = {

        0.5f,
        0.0f,
        0.5f,
        1.0f,
        1.0f,
        0.5f,
        1.0f,
        0.5f,
        0.5f,
        1.0f,
        0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.0f,
        0.5f,
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVerts), floorVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    float wallVerts[] = {

        0.0f,
        0.5f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        -0.5f,
        0.0f,
        0.0f,
    };

    glGenVertexArrays(1, &wallVao);
    glBindVertexArray(wallVao);

    glGenBuffers(1, &wallVbo);
    glBindBuffer(GL_ARRAY_BUFFER, wallVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVerts), wallVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &wallEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    float wallNorthVerts[] = {
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.5f,
        1.0f,
        1.0f,
        1.0f,
        -0.5f,
        1.0f,
        0.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,
    };

    glGenVertexArrays(1, &wallNorthVao);
    glBindVertexArray(wallNorthVao);

    glGenBuffers(1, &wallNorthVbo);
    glBindBuffer(GL_ARRAY_BUFFER, wallNorthVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallNorthVerts), wallNorthVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &wallNorthEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallNorthEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void TileRendererOpenGL::render(GLuint shaderProgram, GLuint windowProgram, const sf::View &view)
{

    glUseProgram(shaderProgram);

    viewProj = viewProjFromSFML(view);
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "uViewProj"),
        1, GL_FALSE, &viewProj[0][0]);

    const int COLS = 12;
    const int ROWS = 12;

    const float startX = 800.f;
    const float startY = 750.f;

    float shade = 1.f;
    float shadeFloor = 1.f;

    glBindVertexArray(vao);

    glBindVertexArray(vao);

    glEnable(GL_BLEND);

    float windowX, windowY;
    float floorX, floorY;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            float worldX = (col - row) * (TILE_W * 0.5f);
            float worldY = (col + row) * (TILE_H * 0.5f);

            if ((row == 7) && col == 3)
            {

                floorX = worldX + startX;
                floorY = worldY + startY;
            }

            drawFloorTile(shaderProgram, worldX + startX, worldY + startY, shadeFloor, shadeFloor, shadeFloor);
        }
    }
    glEnable(GL_BLEND);

    for (int height = 64; height >= 0; height -= 16)
    {

        glBindVertexArray(wallVao);

        for (int row = 1; row < ROWS; row++)
        {

            float tileX = (0 - row) * (TILE_W * 0.5f) + startX;
            float tileY = (0 + row) * (TILE_H * 0.5f) + startY - height;

            if ((row == 9) && height == 32)
            {

                windowLight(shaderProgram, tileX, tileY - TILE_H / 2, shade - 0.5f, shade - 0.5f, shade - 0.5f);
                windowX = tileX + 24;
                windowY = tileY - TILE_H * 1.5;
                continue;
            }

            drawWallTile(shaderProgram, tileX, tileY - TILE_H / 2, shade, shade, shade);
        }

        glBindVertexArray(wallNorthVao);

        for (int col = 1; col < COLS; col++)
        {

            float tileX = col * (TILE_W * 0.5f) + startX;
            float tileY = col * (TILE_H * 0.5f) + startY - height;

            drawWallTile(shaderProgram, tileX, tileY - TILE_H / 2, shade, shade, shade);
        }
    }

    drawWindowRay(windowProgram, windowX, windowY, floorX, floorY);
    drawRays(windowProgram, windowX, windowY, floorX, floorY);

    glBindVertexArray(0);
}

void TileRendererOpenGL::drawMossFloor(GLuint shaderProgram, float screenX, float screenY, float r, float g, float b)
{

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mossTexture);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), screenX, screenY);
    glUniform2f(glGetUniformLocation(shaderProgram, "uScale"), TILE_W, TILE_H);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);
    glUniform1f(glGetUniformLocation(shaderProgram, "uTextureScale"), 32.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};

void TileRendererOpenGL::drawFloorTile(GLuint shaderProgram, float screenX, float screenY,
                                       float r, float g, float b)
{

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glUniform1i(glGetUniformLocation(shaderProgram, "isWindow"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), screenX, screenY);
    glUniform2f(glGetUniformLocation(shaderProgram, "uScale"), TILE_W, TILE_H);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);
    glUniform1f(glGetUniformLocation(shaderProgram, "uTextureScale"), 4.0f);
    glUniform1i(glGetUniformLocation(shaderProgram, "uUseWorldUV"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "uTileW"), TILE_W);
    glUniform1f(glGetUniformLocation(shaderProgram, "uTileH"), TILE_H);

    glUniform2f(glGetUniformLocation(shaderProgram, "uFirePos"), 875.0f, 825.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uAmbient"), 0.05f, 0.05f, 0.06f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TileRendererOpenGL::drawWallTile(GLuint shaderProgram, float screenX, float screenY,
                                      float r, float g, float b)
{

    glBindTexture(GL_TEXTURE_2D, mossTexture);

    glUniform1i(glGetUniformLocation(shaderProgram, "isWindow"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), screenX, screenY);
    glUniform2f(glGetUniformLocation(shaderProgram, "uScale"), TILE_W, TILE_W);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);
    glUniform1f(glGetUniformLocation(shaderProgram, "uTextureScale"), 1.0f);
    glUniform1i(glGetUniformLocation(shaderProgram, "uUseWorldUV"), 0);

    glUniform2f(glGetUniformLocation(shaderProgram, "uFirePos"), 875.0f, 825.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uAmbient"), 0.05f, 0.05f, 0.06f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TileRendererOpenGL::windowLight(GLuint shaderProgram, float screenX, float screenY, float r, float g, float b)
{

    glBindTexture(GL_TEXTURE_2D, windowTexture);

    glUniform1i(glGetUniformLocation(shaderProgram, "isWindow"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), screenX, screenY);
    glUniform2f(glGetUniformLocation(shaderProgram, "uScale"), TILE_W, TILE_W);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);
    glUniform1f(glGetUniformLocation(shaderProgram, "uTextureScale"), 2.0f);
    glUniform1i(glGetUniformLocation(shaderProgram, "uUseWorldUV"), 0);

    glUniform2f(glGetUniformLocation(shaderProgram, "uFirePos"), 875.0f, 825.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "uAmbient"), 0.05f, 0.05f, 0.06f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TileRendererOpenGL::drawWindowRay(GLuint shaderProgram, float windowX, float windowY, float floorX, float floorY)
{

    float w = 20.0f;
    float h = w * 0.5f;

    float verts[] = {

        floorX,
        floorY - h,
        0.0f,
        floorX + w,
        floorY,
        0.0f,
        floorX,
        floorY + h,
        0.0f,
        floorX - w,
        floorY,
        0.0f,
        floorX,
        floorY,
        1.0f,
    };

    unsigned int idx[] = {
        4,
        0,
        1,
        4,
        1,
        2,
        4,
        2,
        3,
        4,
        3,
        0,
    };

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProj"), 1, GL_FALSE, &viewProj[0][0]);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void TileRendererOpenGL::drawRays(GLuint shaderProgram, float windowX, float windowY,
                                  float floorX, float floorY)
{

    float w = 20.0f;
    float h = w * 0.5f;

    float cx[4] = {floorX, floorX + w, floorX, floorX - w};
    float cy[4] = {floorY - h, floorY, floorY + h, floorY};

    const int steps = 200;
    const int totalRays = 4 * (steps + 1);

    static std::vector<std::pair<float, float>> cachedOffsets;
    if (cachedOffsets.empty())
    {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(-8.0f, 8.0f);
        cachedOffsets.reserve(totalRays);
        for (int i = 0; i < totalRays; i++)
            cachedOffsets.push_back({dist(rng), dist(rng)});
    }

    std::vector<float> verts;
    int rayIndex = 0;

    for (int edge = 0; edge < 4; edge++)
    {
        int next = (edge + 1) % 4;

        for (int i = 0; i <= steps; i++)
        {
            float t = (float)i / (float)steps;

            float px = cx[edge] + t * (cx[next] - cx[edge]);
            float py = cy[edge] + t * (cy[next] - cy[edge]);

            float sx = windowX + cachedOffsets[rayIndex].first;
            float sy = windowY + cachedOffsets[rayIndex].second;
            rayIndex++;

            verts.insert(verts.end(), {sx, sy, 0.0f});
            verts.insert(verts.end(), {px, py, 1.0f});
        }
    }

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uViewProj"), 1, GL_FALSE, &viewProj[0][0]);

    glDrawArrays(GL_LINES, 0, (GLsizei)(verts.size() / 3));

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
