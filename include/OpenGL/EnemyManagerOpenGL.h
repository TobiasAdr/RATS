#pragma once
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <string>
#include "../Types.h"

class EnemyManagerOpenGL {
public:
    EnemyManagerOpenGL();
    ~EnemyManagerOpenGL();

    void init(const std::string& texturePath);  
    void spawnEnemy(float x, float y, float hp = 100.f);
    void update(float deltaTime, float playerX, float playerY);
    void render(GLuint shaderProgram, sf::RenderWindow& window, float firePosX, float firePosY);          

    
    int checkHit(float px, float py, float radius = 16.f);
    void dealDamage(int enemyIndex, float damage);

    int getScore() const { return score; }

    bool processPlayerHit(float playerX, float playerY);

    int getAliveCount() {
        int count = 0;
        for (auto& e : enemies) if (e.alive) count++;
        return count;
    };

    

    void uploadEnemyPosition();
    void processHits();

    int getScore() { return this->score; }

private:
    
    void rebuildInstanceBuffer();   

    void rayHit(GLuint shaderProgram, float x, float y);

    std::vector<EnemyOpenGL> enemies;

    GLuint enemySSBO = 0;   
    GLuint hitSSBO = 0;     
    GLuint VAO = 0;
    GLuint quadVBO = 0;         
    GLuint instanceVBO = 0;     
    GLuint texture = 0;

    int score = 0;
    float floorX = 736.f;
    float floorY = 830.f;

};