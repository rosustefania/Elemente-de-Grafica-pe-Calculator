#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/objects.h"
#include "lab_m1/tema1/player.h"
#include "lab_m1/tema1/enemy.h"
#include "lab_m1/tema1/projectile.h"
#include "lab_m1/tema1/healthBar.h"
#include "lab_m1/tema1/scoreBar.h"


#include <chrono>


using std::list;
using std::pair;


typedef struct Dimension {
    float height;
    float width;
} dimension;

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
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
        Tema1();
        ~Tema1();

        void InitMap(float mapLength);
        void InitObstacles();
        void InitPlayer();
        void InitEnemy();
        void InitProjectile(float projectileLength, float projectileMaxDist);
        void InitHealthBar();
        void InitScoreBar();
        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMap(glm::mat3 visMatrix);
        void RenderPlayer(glm::mat3 visMatrix);
        void RenderObstacles(glm::mat3 visMatrix);
        void RenderEnemies(glm::mat3 visMatrix, list<pair<EnemyPosition, float>> &enemies, float deltaTimeSeconds);
        void RenderProjectiles(glm::mat3 visMatrix, list<pair<ProjectilePosition, TraveledDistance>> &projectiles, list<pair<Position, Dimension>> obstacles, float deltaTimeSeconds);
        void RenderHealthBar(glm::mat3 visMatrix);
        void RenderScoreBar(glm::mat3 visMatrix);

        void AddEnemy(float posX, float posY, float speed);
        void AddProjectile(float posX, float posY, float distX, float distY, float angle, float destX, float destY);
        void AddObstacles();
        void AddObstacle(float posX, float posY, float height, float width);
        void GenerateEnemies(int elapsedTime);

        bool CheckPlayerObstacleCollision(float nextStepX, float nextStepY, list<pair<Position, Dimension>> obstacles);
        bool CheckPlayerEnemyCollision(pair<EnemyPosition, float> enemy);
        bool CheckPlayerWallCollision(float nextStepX, float nextStepY);
        bool CheckProjectilEnemyCollision(pair<ProjectilePosition, TraveledDistance> projectile, list<pair<EnemyPosition, float>> &enemies);
        bool CheckProjectilWallCollision(pair<ProjectilePosition, TraveledDistance> projectile);
        bool CheckProjectilObstacleCollision(pair<ProjectilePosition, TraveledDistance> projectile, list<pair<Position, Dimension>> obstacles);
        bool CheckEnemyWallCollision(pair<EnemyPosition, float> enemy);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        Player* player;
        float playerBodyHeadRadius, playerArmRadius;
        float translateX, translateY;
        float centrePlayerX, centrePlayerY;
        float distanceBodyX, distanceBodyY, distanceArmX, distanceArmY;
        float angular;

        list<pair<EnemyPosition, float>> enemies;
        float enemyBodyLength, enemyArmLength;
        float translateEnemyX, translateEnemyY;
        float centreEnemyBodyX, centreEnemyBodyY, centreEnemyArmX, centreEnemyArmY;

        Mesh *map, *obstacle1, *obstacle2;
        float mapLength;

        Projectile* projectile;
        list<pair<ProjectilePosition, TraveledDistance>> projectiles;
        float projectileMaxDist, projectileLength;
        int fireRate;

        list<pair<Position, Dimension>> obstacles;

        HealthBar* healthBar;
        ScoreBar* scoreBar;
        float score;
        bool gameOver;

        double startModeOne, startModeTwo;
        clock_t startTime;
        int elapsedTime;
        time_t now;

    };
}   // namespace m1
