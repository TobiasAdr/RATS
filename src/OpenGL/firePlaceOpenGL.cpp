#include "../../include/OpenGL/firePlaceOpenGL.h"

GLuint loadComputeShader(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Kunde inte öppna: " << path << std::endl;
        return 0;
    }
    std::string src((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    const char *c_src = src.c_str();

    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &c_src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Kompileringsfel i " << path << ": " << log << std::endl;
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);

    GLint linkSuccess;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Linkfel i " << path << ": " << log << std::endl;
    }

    glDetachShader(program, shader);
    glDeleteShader(shader);

    return program;
}

static GLuint loadRenderShader(const std::string &vertPath, const std::string &fragPath)
{
    auto compile = [](const std::string &path, GLenum type) -> GLuint
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Kunde inte öppna: " << path << std::endl;
            return 0;
        }
        std::string src((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        const char *c_src = src.c_str();

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &c_src, nullptr);
        glCompileShader(shader);

        GLint ok;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            std::cerr << "Kompileringsfel " << path << ": " << log << std::endl;
        }
        return shader;
    };

    GLuint vert = compile(vertPath, GL_VERTEX_SHADER);
    GLuint frag = compile(fragPath, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDetachShader(program, vert);
    glDetachShader(program, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

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

firePlaceOpenGL::firePlaceOpenGL(float originX, float originY)
{

    this->originX = originX;
    this->originY = originY;
}

void firePlaceOpenGL::init()
{

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);

    std::vector<FirePlaceParticle> initialData(NUM_PARTICLES);
    for (auto &p : initialData)
    {
        p.posX = 0.0f;
        p.posY = 0.0f;
        p.dirX = 0.0f;
        p.dirY = 0.0f;
        p.life = 0.0f;
    }

    glGenBuffers(1, &firePlaceSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, firePlaceSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 sizeof(FirePlaceParticle) * NUM_PARTICLES,
                 initialData.data(),
                 GL_DYNAMIC_DRAW);

    spawnFireProgram = loadComputeShader("shaders/firePlace.comp");

    renderProgram = loadRenderShader("shaders/firePlacePart.vert", "shaders/firePlacePart.frag");

    glGenVertexArrays(1, &particleVao);

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void firePlaceOpenGL::update(float deltaTime)
{
    step(deltaTime);
}

void firePlaceOpenGL::step(float deltaTime)
{
    elapsedTime += deltaTime;

    glUseProgram(spawnFireProgram);

    glUniform1f(glGetUniformLocation(spawnFireProgram, "deltaTime"), deltaTime);
    glUniform1f(glGetUniformLocation(spawnFireProgram, "time"), elapsedTime);
    glUniform1f(glGetUniformLocation(spawnFireProgram, "originX"), originX);
    glUniform1f(glGetUniformLocation(spawnFireProgram, "originY"), originY);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, firePlaceSSBO);
    glDispatchCompute((NUM_PARTICLES + 63) / 64, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void firePlaceOpenGL::render(const sf::View &view)
{

    glUseProgram(renderProgram);

    glm::mat4 viewProj = viewProjFromSFML(view);
    glUniformMatrix4fv(
        glGetUniformLocation(renderProgram, "uViewProj"),
        1, GL_FALSE, &viewProj[0][0]);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, firePlaceSSBO);

    glBindVertexArray(particleVao);
    glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
    glBindVertexArray(0);
}