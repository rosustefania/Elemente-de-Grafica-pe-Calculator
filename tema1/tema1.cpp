#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/tema1.h"


using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

/* initializes the map */
void Tema1::InitMap(float mapLength)
{
    map = Objects::CreateSquare("map", glm::vec3(0.f, 0.f, 0.f), mapLength, 
        glm::vec3(0.19f, 0.1f, 0.15f), true);
    AddMeshToList(map);
}

/* initializes the player */
void Tema1::InitPlayer()
{
    player = new Player(centrePlayerX, centrePlayerY, playerBodyHeadRadius, playerArmRadius);

    AddMeshToList(player->getPlayerHead());
    AddMeshToList(player->getPlayerFirstArm());
    AddMeshToList(player->getPlayerSecondArm());

}

/* initializes the obstacles */
void Tema1::InitObstacles()
{
    obstacle1 = Objects::CreateRectangle("obstacle1", glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 1.0f,
        glm::vec3(0.65f, 0.43f, 0.73f), true);
    AddMeshToList(obstacle1);

    obstacle2 = Objects::CreateSquare("obstacle2", glm::vec3(0.0f, 0.0f, 0.0f), 0.5f, 
        glm::vec3(0.79f, 0.66f, 0.90f), true);
    AddMeshToList(obstacle2);
}

/* initialize enemy */
void Tema1::InitEnemy()
{
    Enemy* enemy = new Enemy(enemyBodyLength, enemyArmLength);
    AddMeshToList(enemy->getEnemyBody());
    AddMeshToList(enemy->getEnemyFirstArm());
    AddMeshToList(enemy->getEnemySecondArm());
}

/* initializes the projectile */
void Tema1::InitProjectile(float projectileLength, float projectileMaxDist)
{
    projectile = new Projectile(0.125f, 1.25f);
    AddMeshToList(projectile->getProjectile());
}

/* initializes the health bar */
void Tema1::InitHealthBar()
{
    healthBar = new HealthBar(1.0f, 1.0f, 1.0f, 1.0f, 6.0f, 1.0f);
    AddMeshToList(healthBar->getHealthBar());
    AddMeshToList(healthBar->getHealthBarBorder());
}

/* initializes the score bar */
void Tema1::InitScoreBar()
{
    scoreBar = new ScoreBar(1.0f, 1.0f, 1.0f, 1.0f, 6.0f, 1.0f);
    AddMeshToList(scoreBar->getScoreBar());
    AddMeshToList(scoreBar->getScoreBarBorder());
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 1;       // logic x
    logicSpace.y = 1;       // logic y
    logicSpace.width = 2;   // logic width
    logicSpace.height = 2;  // logic height

    // initialize map
    mapLength = 4.0f;
    InitMap(mapLength);

    // initialize obstacles
    InitObstacles();

    // player's center coordinates
    centrePlayerX = 2.0f;
    centrePlayerY = 2.0f;
    
    // player's radius
    playerBodyHeadRadius = 0.1;
    playerArmRadius = 0.05;

    // initialize translation steps
    translateX = 0;
    translateY = 0;

    // distance between player head's centre and body's center
    distanceBodyX = 0.0f;
    distanceBodyY = 0.1f;

    // distance between player head's centre and arm's center
    distanceArmX = 0.075f;
    distanceArmY = 0.075f;

    // angle for rotating after mouse
    angular = 0;

    // enemy's arms center coordinates
    centreEnemyArmX = 0.05f;
    centreEnemyArmY = 0.05f;

    // enemy's body center coordinates
    centreEnemyBodyX = 0.125f;
    centreEnemyBodyY = 0.125f;

    // enemy's body dimensions
    enemyBodyLength = 0.25f;
    enemyArmLength = 0.1f;

    // initialize player
    InitPlayer();

    //initialize enemy
    InitEnemy();

    //initialize projectile
    projectileMaxDist = 1.25f;
    projectileLength = 0.125f;
    InitProjectile(projectileLength, projectileMaxDist);

    // initialize starting time and elapsed time
    startTime = clock();
    elapsedTime = 0;

    // initialize starting time of each spawning enemies mode  
    // first mode will start sometime in the first 3 seconds
    startModeOne = (double)rand() / (RAND_MAX / 3) + 1 * 1000; 
    // second mode will start sometime between the fourth and the sixth second
    startModeTwo = (double)rand() / (RAND_MAX / 3) + 4 * 1000; 

    // add obstacles
    Tema1::AddObstacles();

    // initialize health bar
    Tema1::InitHealthBar();

    // initialize score bar
    Tema1::InitScoreBar();
    score = 0.0f;

    gameOver = false;
}

/* 2D visualization matrix */
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


/* Uniform 2D visualization matrix(same scale factor on x and y axes) */
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), 
        (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 200);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    elapsedTime = clock() - startTime;

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw 
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);
    
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    if (healthBar->getHealthBarPosX() == 7) {
        gameOver = true;
        cout << "GAME OVER!" << endl;
    }

    if (gameOver == false) {
        Tema1::RenderPlayer(visMatrix);
        Tema1::RenderProjectiles(visMatrix, projectiles, obstacles, deltaTimeSeconds);
        Tema1::GenerateEnemies(elapsedTime);
        Tema1::RenderEnemies(visMatrix, enemies, deltaTimeSeconds);
        Tema1::RenderPlayer(visMatrix);
    }
    Tema1::RenderObstacles(visMatrix);
    Tema1::RenderMap(visMatrix);

    // Sets the screen area where to draw the health bar
    SetViewportArea(ViewportSpace(resolution.x - 320, resolution.y - 60, 300, 50), glm::vec3(0.98f, 0.87f, 0.98f), true);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(LogicSpace(1, 1, 6, 1), ViewportSpace(resolution.x - 320, resolution.y - 60, 300, 50));
    Tema1::RenderHealthBar(visMatrix);

    // Sets the screen area where to draw the score bar
    SetViewportArea(ViewportSpace(resolution.x - 320, resolution.y - 120, 300, 50), glm::vec3(0.98f, 0.87f, 0.98f), true);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(LogicSpace(1, 1, 6, 1), ViewportSpace(resolution.x - 320, resolution.y - 120, 300, 50));
    Tema1::RenderScoreBar(visMatrix);
}

void Tema1::FrameEnd()
{
}

/* adds new enemy's position and speed */
void Tema1::AddEnemy(float posX, float posY, float speed)
{
    EnemyPosition position;
    position.posX = posX;
    position.posY = posY;
    position.initialPosX = posX;
    position.initialPosY = posY;

    pair<EnemyPosition, float> data = make_pair(position, speed);
    enemies.push_back(data);
}

/* adds new projectile */
void Tema1::AddProjectile(float posX, float posY, float distX, float distY, float angle, float destX, float destY)
{
    ProjectilePosition position;
    position.currentPosX = posX;
    position.currentPosY = posY;
    position.intialPosX = posX;
    position.intialPosY = posY;

    TraveledDistance traveledDistance;
    traveledDistance.distX = distX;
    traveledDistance.distY = distY;
    traveledDistance.angle = angle;
    traveledDistance.destX = destX;
    traveledDistance.destY = destY;

    projectiles.push_back(make_pair(position, traveledDistance));
    fireRate = time(&now);
}

/* adds a new obstacle */
void Tema1::AddObstacle(float posX, float posY, float height, float width) {
    Position position;
    position.posX = posX;
    position.posY = posY;

    Dimension dimension;
    dimension.height = height;
    dimension.width = width;

    pair<Position, Dimension> obstacle = make_pair(position, dimension);

    obstacles.push_back(obstacle);
}

/* add all obstacles */
void Tema1::AddObstacles() {
    AddObstacle(2.5f, 2.5f, 1.0f, 0.3f);
    AddObstacle(0.75f, 0.6f, 0.5f, 0.5f);
    AddObstacle(2.5f, 0.75f, 0.5f, 0.75f);
    AddObstacle(3.6f, 3.6f, 0.4f, 0.4f);
    AddObstacle(1.75f, 2.5f, 0.3f, 0.75f);
    AddObstacle(0.25f, 3.25f, 0.4f, 0.4f);
    AddObstacle(0.0f, 1.5f, 0.55f, 0.3f);
}


/* 
 *    Generates enemies based on two modes:
 *   - first mode generates one enemy every two seconds starting at a random time 
 *   bewteen the first and the third second
 *   - second mode generates four enemies every six seconds starting at a random
 *   time bewteen the fourth and the sixth second
*/
void Tema1::GenerateEnemies(int elapsedTime) {
    if (elapsedTime - startModeOne > 0 && elapsedTime - startModeOne < 100) {
        float posX = (float)rand() / (float)(RAND_MAX / 3.65f);
        float posY = (float)rand() / (float)(RAND_MAX / 3.70f);
        float speed = (float)rand() / (float)(RAND_MAX / 0.4f) + 0.2f;

        if ((posX <= player->getPositionX() - 0.5f || posX >= player->getPositionX() + 0.5f) &&
            (posY <= player->getPositionY() - 0.5f || posY >= player->getPositionY() + 0.5f)) {
            Tema1::AddEnemy(posX, posY, speed);
            startModeOne += 2000;
        }
    }

    if (elapsedTime - startModeTwo > 0 && elapsedTime - startModeTwo < 100) {
        float posX = (float)rand() / (float)(RAND_MAX / 3.65f);
        float posY = (float)rand() / (float)(RAND_MAX / 3.70f);
        float speed = (float)rand() / (float)(RAND_MAX / 0.4f) + 0.2f;
         Tema1::AddEnemy(posX, posY, speed);
        

        posX = (float)rand() / (float)(RAND_MAX / 3.65f);
        posY = (float)rand() / (float)(RAND_MAX / 3.70f);
        speed = (float)rand() / (float)(RAND_MAX / 0.4f) + 0.2f;
        Tema1::AddEnemy(posX, posY, speed);
        

        posX = (float)rand() / (float)(RAND_MAX / 3.65f);
        posY = (float)rand() / (float)(RAND_MAX / 3.70f);
        speed = (float)rand() / (float)(RAND_MAX / 0.4f) + 0.2f;
        Tema1::AddEnemy(posX, posY, speed);
       
        posX = (float)rand() / (float)(RAND_MAX / 3.65f);
        posY = (float)rand() / (float)(RAND_MAX / 3.70f);
        speed = (float)rand() / (float)(RAND_MAX / 0.5f) + 0.1f;
        Tema1::AddEnemy(posX, posY, speed);
       

        startModeTwo += 6000;
    }

}


/* renders game's map */
void Tema1::RenderMap(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(0.0f, 0.0f);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);

}

/* renders game's player */
void Tema1::RenderPlayer(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(centrePlayerX, centrePlayerY);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(angular);
    modelMatrix *= transform2D::Translate(-distanceArmX, -distanceArmY);
    RenderMesh2D(meshes[player->getPlayerFirstArm()->GetMeshID()], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(centrePlayerX, centrePlayerY);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(angular);
    modelMatrix *= transform2D::Translate(distanceArmX, -distanceArmY);
    RenderMesh2D(meshes[player->getPlayerSecondArm()->GetMeshID()], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(centrePlayerX, centrePlayerY);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    RenderMesh2D(meshes[player->getPlayerHead()->GetMeshID()], shaders["VertexColor"], modelMatrix);
    
}

/* renders game's obstacles */
void Tema1::RenderObstacles(glm::mat3 visMatrix) 
{
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(2.5f, 2.5f);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(0.75f, 0.6f);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(2.5f, 0.75f);
    modelMatrix *= transform2D::Scale(2.5f, 0.5f);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(3.6f, 3.6f);
    modelMatrix *= transform2D::Scale(0.8f, 0.8f);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(1.75f, 2.5f);
    modelMatrix *= transform2D::Scale(2.5f, 0.3f);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(0.25f, 3.25f);
    modelMatrix *= transform2D::Scale(0.8f, 0.8f);
    RenderMesh2D(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(0.0f, 1.5f);
    modelMatrix *= transform2D::Scale(1.0f, 0.55f);
    RenderMesh2D(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);

}

/* renders game's enemies */
void Tema1::RenderEnemies(glm::mat3 visMatrix, list <pair<EnemyPosition, float>>& enemies, float deltaTimeSeconds)
{
    list<pair<EnemyPosition, float >>::iterator it =  enemies.begin();
    while (it != enemies.end()) {

        float initialX = player->getPositionX() - it->first.initialPosX;
        float initialY = player->getPositionY() - it->first.initialPosY;

        float factorX = player->getPositionX() - it->first.posX;
        float factorY = player->getPositionY() - it->first.posY;

        if (CheckPlayerEnemyCollision(*it) == false) {
            // the enemy has not reached the player yet

            if (CheckEnemyWallCollision(*it) == false) {
                if (it->first.posX != player->getPositionX()) {
                    // decide if it needs to go to the left or to the right
                    if (it->first.posX < player->getPositionX()) {
                        (*it).first.posX = it->first.posX + it->second * deltaTimeSeconds;
                    }
                    else {
                        (*it).first.posX = it->first.posX - it->second * deltaTimeSeconds;
                    }
                }


                if (it->first.posY != player->getPositionY()) {
                    // decide if it needs to go to the left or to the right
                    if (it->first.posY < player->getPositionY()) {
                        (*it).first.posY = it->first.posY + it->second * deltaTimeSeconds;
                    }
                    else {
                        (*it).first.posY = it->first.posY - it->second * deltaTimeSeconds;
                    }
                }
            }


            // rotates to face the player in order to go after him
            float deltaX = player->getPositionX() - (it->first.posX + enemyArmLength / 2 + enemyBodyLength / 2);
            float deltaY = player->getPositionY() - (it->first.posY + enemyArmLength / 2 + enemyBodyLength / 2);

            
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(it->first.posX, it->first.posY);
            modelMatrix *= transform2D::Translate(centreEnemyArmX + centreEnemyBodyX, centreEnemyArmY + centreEnemyBodyY);
            modelMatrix *= transform2D::Rotate(-atan2(deltaY, deltaX) + M_PI / 2 + M_PI);
            modelMatrix *= transform2D::Translate(-centreEnemyArmX - centreEnemyBodyX, -centreEnemyArmY - centreEnemyBodyY);
            RenderMesh2D(meshes["enemyFirstArm"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(it->first.posX + 2 * centreEnemyBodyX, it->first.posY);
            modelMatrix *= transform2D::Translate(centreEnemyArmX - centreEnemyBodyX, centreEnemyArmY + centreEnemyBodyY);
            modelMatrix *= transform2D::Rotate(-atan2(deltaY, deltaX) + M_PI / 2 + M_PI);
            modelMatrix *= transform2D::Translate(-centreEnemyArmX + centreEnemyBodyX, -centreEnemyArmY - centreEnemyBodyY);
            RenderMesh2D(meshes["enemySecondArm"], shaders["VertexColor"], modelMatrix);

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(it->first.posX + centreEnemyArmX, it->first.posY + centreEnemyArmY);
            modelMatrix *= transform2D::Translate(centreEnemyBodyX, centreEnemyBodyY);
            modelMatrix *= transform2D::Rotate(-atan2(deltaY, deltaX) + M_PI / 2);
            modelMatrix *= transform2D::Translate(-centreEnemyBodyX, -centreEnemyBodyY);
            RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);

            ++it;
        }
        else {
            it = enemies.erase(it);
        }
    }
}

/* renders game's projectiles */
void Tema1::RenderProjectiles(glm::mat3 visMatrix, list<pair<ProjectilePosition, TraveledDistance>>& projectiles,
    list<pair<Position, Dimension>> obstacles, float deltaTimeSeconds)
{
    list<pair<ProjectilePosition, TraveledDistance>>::iterator it = projectiles.begin();
    while (it != projectiles.end()) {

        float factorX = it->second.destX - it->first.intialPosX;
        float factorY = it->second.destY - it->first.intialPosY;

        float speed = 2.25f;
        float traveledDistance = sqrt(pow(it->second.distX, 2) + pow(it->second.distY, 2) * 1.0);

        if (traveledDistance < projectile->getProjectileMaxDist() &&
            CheckProjectilWallCollision(*it) == false &&
            CheckProjectilObstacleCollision(*it, obstacles) == false &&
            CheckProjectilEnemyCollision(*it, enemies) == false) {

            (*it).first.currentPosX = it->first.currentPosX + speed * factorX * deltaTimeSeconds;
            (*it).second.distX += speed * factorX * deltaTimeSeconds;

            (*it).first.currentPosY = it->first.currentPosY + speed * factorY * deltaTimeSeconds;
            (*it).second.distY += speed * factorY * deltaTimeSeconds;

            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(it->first.currentPosX, it->first.currentPosY);
            modelMatrix *= transform2D::Rotate(it->second.angle);
            modelMatrix *= transform2D::Translate(-projectile->getProjectileLength() / 2, 
                -projectile->getProjectileLength() / 2);
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

            it++;
        }
        else {
            it = projectiles.erase(it);
        }
    }
}

/* renders game's health bar */
void Tema1::RenderHealthBar(glm::mat3 visMatrix) 
{
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(healthBar->getHealthBarPosX(), healthBar->getHealthBarPosY());
    RenderMesh2D(meshes[healthBar->getHealthBar()->GetMeshID()], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(healthBar->getHealthBarBorderPosX(), healthBar->getHealthBarBorderPosY());
    RenderMesh2D(meshes[healthBar->getHealthBarBorder()->GetMeshID()], shaders["VertexColor"], modelMatrix);
    
}

/* renders game's score bar */
void Tema1::RenderScoreBar(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(scoreBar->getScoreBarPosX(), scoreBar->getScoreBarPosY());
    modelMatrix *= transform2D::Scale(score, 1.0f);
    RenderMesh2D(meshes[scoreBar->getScoreBar()->GetMeshID()], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(scoreBar->getScoreBarBorderPosX(), scoreBar->getScoreBarBorderPosY());
    RenderMesh2D(meshes[scoreBar->getScoreBarBorder()->GetMeshID()], shaders["VertexColor"], modelMatrix);

}


/* checks collision between player and obstacle */
bool Tema1::CheckPlayerObstacleCollision(float nextStepX, float nextStepY, 
    list<pair<Position, Dimension>> obstacles)
{
    bool collision = false;

    list<pair<Position, Dimension>>::iterator it;
    for (it = obstacles.begin(); it != obstacles.end(); ++it) {

        // calculate the distance between player centre and obstacle centre
        float distanceX = (player->getPositionX() + nextStepX) - (it->first.posX + it->second.width / 2);
        float distanceY = (player->getPositionY() + nextStepY) - (it->first.posY + it->second.height / 2);
        glm::vec2 difference(distanceX, distanceY);

        glm::vec2 clamped = glm::clamp(difference, 
            - glm::vec2(it->second.width / 2, it->second.height / 2), 
            glm::vec2(it->second.width / 2, it->second.height / 2));

        glm::vec2 closest = glm::vec2(it->first.posX + it->second.width / 2,
            it->first.posY + it->second.height / 2) + clamped;

        difference = closest - 
            glm::vec2((player->getPositionX() + nextStepX), (player->getPositionY() + nextStepY));

        if (glm::length(difference) < playerBodyHeadRadius) {
            collision = true;
        }    
    }
    return collision;
}

/* checks collision between player and enemy */
bool Tema1::CheckPlayerEnemyCollision(pair<EnemyPosition, float> enemy)
{
    bool collision = false;

    float distanceX = player->getPositionX() - (enemy.first.posX + enemyBodyLength / 2);
    float distanceY = player->getPositionY() -  (enemy.first.posY + enemyBodyLength / 2);
    glm::vec2 difference(distanceX, distanceY);

    glm::vec2 clamped = glm::clamp(difference,
        -glm::vec2(enemyBodyLength / 2, enemyBodyLength / 2),
        glm::vec2(enemyBodyLength / 2, enemyBodyLength / 2));

    glm::vec2 closest = glm::vec2(enemy.first.posX + enemyBodyLength / 2,
        enemy.first.posY + enemyBodyLength / 2) + clamped;

    difference = closest - glm::vec2(player->getPositionX(), player->getPositionY());

    if (glm::length(difference) < playerBodyHeadRadius) {
        collision = true;
        healthBar->setHealthBarPosX(healthBar->getHealthBarPosX() + 0.25f);
    }

    return collision;
}

/* checks collision between player and map's walls */
bool Tema1::CheckPlayerWallCollision(float nextStepX, float nextStepY)
{
    bool collision = false;

    if (player->getPositionX() + nextStepX < playerBodyHeadRadius ||
        player->getPositionY() + nextStepY < playerBodyHeadRadius ||
        player->getPositionX() + nextStepX > mapLength - playerBodyHeadRadius ||
        player->getPositionY() + nextStepY > mapLength - playerBodyHeadRadius) {

        collision = true;
    }

    return collision;
}

/* checks collision between projectil and enemy */
bool Tema1::CheckProjectilEnemyCollision(pair<ProjectilePosition, TraveledDistance> project,
    list<pair<EnemyPosition, float>> &enemies)
{
    bool collision = false;
    list<pair<EnemyPosition, float>>::iterator it = enemies.begin();
    while (it != enemies.end()) {
        bool collisionArmX = project.first.currentPosX + projectile->getProjectileLength() / 2 >=
            it->first.posX &&
            it->first.posX + enemyArmLength + enemyBodyLength >=
            project.first.currentPosX - projectile->getProjectileLength() / 2;
        bool collisionArmY = project.first.currentPosY + projectile->getProjectileLength() / 2 >=
            it->first.posY &&
            it->first.posY + enemyBodyLength + enemyArmLength / 2 >= 
            project.first.currentPosY - projectile->getProjectileLength() / 2;
        
        bool collisionBodyX = project.first.currentPosX + projectile->getProjectileLength() / 2 >= 
            it->first.posX + enemyArmLength / 2 &&
            it->first.posX + enemyArmLength / 2 + enemyBodyLength >= 
            project.first.currentPosX - projectile->getProjectileLength() / 2;
        bool collisionBodyY = project.first.currentPosY + projectile->getProjectileLength() / 2 >=
            it->first.posY + enemyArmLength / 2 &&
            it->first.posY + enemyArmLength / 2 + enemyBodyLength >=
            project.first.currentPosY - projectile->getProjectileLength() / 2;
        
        if ((collisionArmX && collisionArmY) == true || (collisionBodyX && collisionBodyY) == true) {
            collision = true;
            it = enemies.erase(it);
            score += 0.01f;
        }
        else {
            it++;
        }
    }

    return collision;
}

/* checks collision between projectil and map's walls */
bool Tema1::CheckProjectilWallCollision(pair<ProjectilePosition, TraveledDistance> project)
{
    bool collision = false;

    if (project.first.currentPosX < projectile->getProjectileLength() / 2 ||
        project.first.currentPosY < projectile->getProjectileLength() / 2 ||
        project.first.currentPosX > mapLength - projectile->getProjectileLength() / 2 ||
        project.first.currentPosY > mapLength - projectile->getProjectileLength() / 2) {

        collision = true;
    }

    return collision;

}

/* checks collision between projectil and obstacle */
bool Tema1::CheckProjectilObstacleCollision(pair<ProjectilePosition, TraveledDistance> project, 
    list<pair<Position, Dimension>> obstacles)
{
    bool collision = false;
    list<pair<Position, Dimension>>::iterator it;
    for (it = obstacles.begin(); it != obstacles.end(); ++it) {
        bool collisionX = project.first.currentPosX + projectile->getProjectileLength() / 2 >= it->first.posX && 
            it->first.posX + it->second.width >= project.first.currentPosX - projectile->getProjectileLength() / 2;
        bool collisionY = project.first.currentPosY + projectile->getProjectileLength() / 2 >= it->first.posY && 
            it->first.posY + it->second.height >= project.first.currentPosY - projectile->getProjectileLength() / 2;

        if ((collisionX && collisionY) == true) {
            collision = true;
        }
    }

    return collision;

}

/* checks collision between enemy and map's walls */
bool Tema1::CheckEnemyWallCollision(pair<EnemyPosition, float> enemy)
{
    bool collision = false;

    if (enemy.first.posX < 0 ||
        enemy.first.posY < 0 ||
        enemy.first.posX > mapLength - enemyArmLength - enemyBodyLength ||
        enemy.first.posY > mapLength - enemyArmLength / 2 - enemyBodyLength) {

        collision = true;
    }

    return collision;

}
        
/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        if (CheckPlayerObstacleCollision(0, 0.5 * deltaTime, obstacles) == false &&
                CheckPlayerWallCollision(0, 0.5 * deltaTime) == false) {
            logicSpace.y += 0.5 * deltaTime;
            translateY += 0.5 * deltaTime;
            player->setPositionY(player->getPositionY() + 0.5 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (CheckPlayerObstacleCollision(0, -0.5 * deltaTime, obstacles) == false &&
                CheckPlayerWallCollision(0, -0.5 * deltaTime) == false) {
            logicSpace.y -= 0.5 * deltaTime;
            translateY -= 0.5 * deltaTime;
            player->setPositionY(player->getPositionY() - 0.5 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        if (CheckPlayerObstacleCollision(-0.5 * deltaTime, 0, obstacles) == false &&
                CheckPlayerWallCollision(-0.5 * deltaTime, 0) == false) {
            logicSpace.x -= 0.5 * deltaTime;
            translateX -= 0.5 * deltaTime;
            player->setPositionX(player->getPositionX() - 0.5 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        if (CheckPlayerObstacleCollision(0.5 * deltaTime, 0, obstacles) == false &&
                CheckPlayerWallCollision(0.5 * deltaTime, 0) == false) {
            logicSpace.x += 0.5 * deltaTime;
            translateX += 0.5 * deltaTime;
            player->setPositionX(player->getPositionX() + 0.5 * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_Z)) {

        logicSpace.x += logicSpace.width / 2;
        logicSpace.y += logicSpace.height / 2;

        logicSpace.width -= 0.5 * deltaTime;
        logicSpace.height -= 0.5 * deltaTime;

        logicSpace.x -= logicSpace.width / 2;
        logicSpace.y -= logicSpace.height / 2;
    }
    
    if (window->KeyHold(GLFW_KEY_X)) {
        logicSpace.x += logicSpace.width / 2;
        logicSpace.y += logicSpace.height / 2;

        logicSpace.width += 0.5 * deltaTime;
        logicSpace.height += 0.5 * deltaTime;

        logicSpace.x -= logicSpace.width / 2;
        logicSpace.y -= logicSpace.height / 2;
    }

}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{ 
    glm::ivec2 resolution = window->GetResolution();

    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

     angular = atan2((mouseX - tx) / smin - player->getPositionX(), 
         (resolution.y - mouseY - ty) / smin - player->getPositionY()) + M_PI;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{   
    glm::ivec2 resolution = window->GetResolution();

    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    float angle = atan2((mouseX - tx) / smin - player->getPositionX(), 
        (resolution.y - mouseY - ty) / smin - player->getPositionY()) + M_PI;
    
    
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {

        if (time(&now) - fireRate >= 1) {
            Tema1::AddProjectile(player->getPositionX(), player->getPositionY(), 
                0.0f, 0.0f, angle, (mouseX - tx) / smin, (resolution.y - mouseY - ty) / smin);
        }
    }
}

