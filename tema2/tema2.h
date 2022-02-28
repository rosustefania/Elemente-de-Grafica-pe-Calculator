#pragma once

#include "components/simple_scene.h"
#include "homework_camera.h"
#include <lab_m1/tema2/player.h>
#include <lab_m1/tema2/maze.h>
#include <lab_m1/tema2/healthBar.h>
#include <lab_m1/tema2/healthBar.h>
#include <lab_m1/tema2/timeBar.h>
#include <lab_m1/tema2/transform2D.h>


#include <stdbool.h>

#include <list>

typedef struct Collision {
    bool isCollision;
    int timeSinceCollision;
}collision;

typedef struct Projectile {
    glm::vec3 initialPos;
    glm::vec3 currentPos;
    float speed;
    glm::vec3 direction;
}Projectile;

typedef struct Enemy {
    glm::vec3 initialPos;
    glm::vec3 currentPos;
    int directionX;
    int directionZ;
    Collision collision;
}Enemy;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {

    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Tema2();
        ~Tema2();

        void Init() override;
        void InitBox();
        void InitSphere();
        void InitProjectile();

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;


        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void RenderPlayer();
        void RenderMaze();

        float CalculateProjectileDistance(glm::vec3 initialPos, glm::vec3 currentPos);
        void AddProjectile(glm::vec3 startPos, list<Projectile>& projectiles, glm::vec3 forward);
        void RenderProjectiles(float deltaTimeSeconds);

        pair<int, int> GeneratePlayerPosition();
        void UpdateDistanceToTarget(float newDistance);

        void GenerateEnemies(list<Enemy> &enemies);
        void RenderEnemies(float deltaTimeSeconds);

        void RenderHealthBar(glm::mat3 visMatrix);

        bool CheckPlayerWallCollision(float nextStepX, float nextStepZ);
        bool CheckPlayerEnemyCollision(float nextStepX, float nextStepZ);
        bool CheckProjectileEnemyCollision(Projectile projectile, Enemy enemy);
        bool CheckProjectileWallCollision(Projectile projectile);

        bool GameOver();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::CameraHomework* camera, *cameraBar;
        glm::mat4 projectionMatrix, orthographicMatrix;
        bool renderCameraTarget;
        bool thirdPerson;

        glm::mat3 visMatrix;

        float left, right, bottom, top, near, far;
        float fov;
        int projType;

        Player* player;

        Maze* maze;
        int mazeStartPosition;

        list<Projectile> projectiles;
        float projectileLife;

        list<Enemy> enemies;
        float enemyOffset;

        HealthBar* healthBar;
        float health;
        bool enemyCollision;

        TimeBar* timeBar;

        clock_t startTime;
        int time;

        clock_t enemyDisappear;


    };
}   // namespace m1
