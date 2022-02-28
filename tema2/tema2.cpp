
#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

#define SIZE 20
#define ENEMIES 25

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::InitProjectile()
{
    Mesh* projectile = new Mesh("projectile");
    projectile->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[projectile->GetMeshID()] = projectile;
}

void Tema2::InitBox()
{
    Mesh* box = new Mesh("box");
    box->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[box->GetMeshID()] = box;
}

void Tema2::InitSphere()
{
    Mesh* sphere = new Mesh("sphere");
    sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[sphere->GetMeshID()] = sphere;
}


void Tema2::Init()
{
    // initialize maze
    maze = new Maze(SIZE);
    maze->generateMaze(SIZE);

    mazeStartPosition = 0;

    // initialize player
    Tema2::InitBox();
    pair<int, int> position = Tema2::GeneratePlayerPosition();
    player = new Player(position.first , position.second);

    Tema2::InitSphere();
    enemyOffset = 0.2f;
    Tema2::GenerateEnemies(enemies);


    top = 10;
    bottom = 0;
    left = 0;
    right = 5;
    near = 0.1;
    far = 200;
    fov = 60;
    projType = 1;

    renderCameraTarget = false;
    thirdPerson = true;


    camera = new implemented::CameraHomework();
    cameraBar = new implemented::CameraHomework();

    player->setOffsetCameraX(camera->distanceToTarget * sin(player->getRotatingAngle()));
    player->setOffsetCameraZ(camera->distanceToTarget * cos(player->getRotatingAngle()));

    camera->Set(glm::vec3(player->getPosX() + 0.5f - player->getOffsetCameraX(), 2, player->getPosZ() + 0.5f - player->getOffsetCameraZ()),
         glm::vec3(player->getPosX() + 0.5f, 2, player->getPosZ() + 0.5f), glm::vec3(0, 1, 0));

    cameraBar->Set(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, near, far);
    projType = 0;
    
    {
        Shader* shader = new Shader("SimpleShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
       
    }

    {
        Shader* shader = new Shader("AnimatedShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShaderAnimated.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;

    }

    Tema2::InitProjectile();
    projectileLife = 3.0f;

    healthBar = new HealthBar(1.0f, 1.0f, 1.0f, 1.0f, 6.0f, 1.0f);
    AddMeshToList(healthBar->getHealthBar());
    AddMeshToList(healthBar->getHealthBarBorder());

    timeBar = new TimeBar(1.0f, 1.0f, 1.0f, 1.0f, 6.0f, 1.0f);
    AddMeshToList(timeBar->getTimeBar());
    AddMeshToList(timeBar->getTimeBarBorder());

    // initialize starting time and elapsed time
    startTime = clock();
    time = 60;

    health = 1000;

    enemyCollision = false;
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.38f, 0.25f, 0.35f, 1);
    //glClearColor(0.53f, 0.33f, 0.48f, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

}

glm::mat3 Tema2::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
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


void Tema2::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
    projType = 1;


}


void Tema2::Update(float deltaTimeSeconds)
{

    glm::ivec2 resolution = window->GetResolution();

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, near, far);
    projType = 0;

    Tema2::RenderEnemies(deltaTimeSeconds);

    Tema2::RenderProjectiles(deltaTimeSeconds);

    if (thirdPerson && (!GameOver())) {
        Tema2::RenderPlayer();
    }

    Tema2::RenderMaze();

    SetViewportArea(ViewportSpace(resolution.x - 320, resolution.y - 60, 300, 50), glm::vec3(0.98f, 0.87f, 0.98f), true);
    SetViewportArea(ViewportSpace(resolution.x - 320, resolution.y - 120, 300, 50), glm::vec3(0.54f, 0.78f, 0.83f), true);
    
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix;

    if (projType == 0) {
        viewMatrix = camera->GetViewMatrix();
    }
    else {
        viewMatrix = cameraBar->GetViewMatrix();
    }

    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    
}


pair<int, int> Tema2::GeneratePlayerPosition()
{
    // get random position somewhere in the middle of the maze
    int randomI = SIZE / 4 + rand() % SIZE / 2;
    int randomJ = SIZE / 4 + rand() % SIZE / 2;

    while ((maze->getGrid())[randomI][randomJ] != 0 ||
        maze->pathExits(SIZE, randomI, randomJ) == false) {
        randomI = SIZE / 4 + rand() % SIZE / 2;
        randomJ = SIZE / 4 + rand() % SIZE / 2;
    }

    (maze->getGrid())[randomI][randomJ] = 3;
    return (make_pair(randomI, randomJ));
}




void Tema2::RenderPlayer()
{

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0.95f, camera->GetTargetPosition().z));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.94f, 0.78f, 0.68f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 0.255f, 0.475f, camera->GetTargetPosition().z));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.255f, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.255f, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f, 0.10f, 0.12f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.94f, 0.78f, 0.68f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x - 0.255f, 0.475f, camera->GetTargetPosition().z ));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.255f, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.255f, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f, 0.10f, 0.12f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.94f, 0.78f, 0.68f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x - 0.255f , 0.675f, camera->GetTargetPosition().z));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.255f, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.255f, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f, 0.30f, 0.12f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.61f, 0.67f, 0.06f));


    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 0.255f, 0.675f, camera->GetTargetPosition().z));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.255f, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.255f, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f, 0.30f, 0.12f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.61f, 0.67f, 0.06f));


    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x, 0.625f, camera->GetTargetPosition().z));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f, 0.4f, 0.2f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.61f, 0.67f, 0.06f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x - 0.10f, 0.2f, camera->GetTargetPosition().z ));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.10f, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.10f, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.43f, 0.2f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.19f, 0.62f, 0.70f));


    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(camera->GetTargetPosition().x + 0.10f, 0.2f, camera->GetTargetPosition().z));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.10f, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, player->getRotatingAngle(), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.10f, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.43f, 0.2f));
    RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.19f, 0.62f, 0.70f));
}


void Tema2::RenderMaze()
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((maze->getGrid())[i][j] == 1) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(mazeStartPosition + i + 0.5f, 0.6f, mazeStartPosition + j + 0.5f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.2f, 1.0f));
                RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.75f, 0.21f, 0.56f));
            } else {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(mazeStartPosition + i + 0.5f, 0, mazeStartPosition + j + 0.5f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 0.0f, 1.0f));
                RenderMesh(meshes["box"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.35f, 0.02f, 0.20f));
            }
        }
    }
}


float Tema2::CalculateProjectileDistance(glm::vec3 initialPos, glm::vec3 currentPos)
{
    return sqrt(pow(currentPos.x - initialPos.x, 2) + pow(currentPos.y - initialPos.y, 2) +
        pow(currentPos.z - initialPos.z, 2) * 1.0);
}


void Tema2::AddProjectile(glm::vec3 startPos, list<Projectile> &projectiles, glm::vec3 forward)
{
    Projectile newProjectile;
    newProjectile.initialPos = startPos;
    newProjectile.currentPos = startPos;
    newProjectile.speed = 2.0f;
    newProjectile.direction = forward;
    projectiles.push_back(newProjectile);
}

void Tema2::RenderProjectiles(float deltaTimeSeconds)
{
    list<Projectile>::iterator it = projectiles.begin();
 
    while (it != projectiles.end()) {

        if (CalculateProjectileDistance(it->initialPos, it->currentPos) < projectileLife && 
                CheckProjectileWallCollision(*it) == false) {

            bool collision = false;

            list<Enemy>::iterator it2 = enemies.begin();
            while (it2 != enemies.end()) {
                if (CheckProjectileEnemyCollision(*it, *it2)){
                    collision = true;
                 }
                it2++;
            }

            if (collision == false) {
                (*it).currentPos.x = it->currentPos.x + it->speed * deltaTimeSeconds * it->direction.x;
                (*it).currentPos.y = it->currentPos.y + it->speed * deltaTimeSeconds * it->direction.y;
                (*it).currentPos.z = it->currentPos.z + it->speed * deltaTimeSeconds * it->direction.z;

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos.x + 0.5f, it->currentPos.y + 0.5f, it->currentPos.z + 0.5f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
                RenderMesh(meshes["projectile"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.30f, 0.33f, 0.07f));
                ++it;
            }
            else {
                it = projectiles.erase(it);
            }
        }
        else {
            it = projectiles.erase(it);
        }
    }
}


void Tema2::GenerateEnemies(list<Enemy> &enemies)
{
    int count = 0;

    int randomI = rand() % SIZE;
    int randomJ = rand() % SIZE;

    while (count < ENEMIES) {
        while ((maze->getGrid())[randomI][randomJ] != 0) {
            randomI = rand() % SIZE;
            randomJ = rand() % SIZE;
        }
     
        Collision newCollision;
        newCollision.isCollision = false;
        newCollision.timeSinceCollision = 0;

        Enemy newEnemy;
        newEnemy.initialPos = newEnemy.currentPos = glm::vec3(randomI + enemyOffset, 0.2f, randomJ + enemyOffset);
        newEnemy.directionX = 0;
        newEnemy.directionZ = 1;
        newEnemy.collision = newCollision;
        enemies.push_back(newEnemy);

        (maze->getGrid())[randomI][randomJ] = 2;
        count++;
        }

}

void Tema2::RenderEnemies(float deltaTimeSeconds)
{
    list<Enemy>::iterator it = enemies.begin();
    while (it != enemies.end()) {
        
        if (it->directionX == 1 && it->directionZ == 0) {
            if (it->currentPos[0] < it->initialPos[0] + 1.0f - enemyOffset * 2) {
                it->currentPos[0] += deltaTimeSeconds * 0.5f;
            }
            else {
                it->directionX = 0;
                it->directionZ = 1;
            }
        }

        
        if (it->directionX == 0 && it->directionZ == 1) {
            if (it->currentPos[2] < it->initialPos[2] + 1.0f - enemyOffset * 2) {
                it->currentPos[2] += deltaTimeSeconds * 0.5f;
            }
            else {
                it->directionX = -1;
                it->directionZ = 0;
            }
        }

        if (it->directionX == -1 && it->directionZ == 0) {
            if (it->currentPos[0] > it->initialPos[0]) {
                it->currentPos[0] -= deltaTimeSeconds * 0.5f;
            }
            else {
                it->directionX = 0;
                it->directionZ = -1;
            }
        }

        if (it->directionX == 0 && it->directionZ == -1) {
            if (it->currentPos[2] > it->initialPos[2]) {
                it->currentPos[2] -= deltaTimeSeconds * 0.5f;
            }
            else {
                it->directionX = 1;
                it->directionZ = 0;
            }
        }
       
        
        list<Projectile>::iterator it2 = projectiles.begin();
        while (it2 != projectiles.end()) {
            if (CheckProjectileEnemyCollision(*it2, *it)) {
                (*it).collision.isCollision = true;
                (*it).collision.timeSinceCollision = clock();
            }

            it2++;
        }

        if (it->collision.isCollision == false) {

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos[0], 0.20f, it->currentPos[2]));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
            RenderMesh(meshes["sphere"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.22f, 0.35f, 0.45f));

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos[0], 0.60f, it->currentPos[2]));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
            RenderMesh(meshes["sphere"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.64f, 0.90f, 0.57f));

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos[0], 1.0f, it->currentPos[2]));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
            RenderMesh(meshes["sphere"], shaders["SimpleShader"], modelMatrix, glm::vec3(0.52f, 0.70f, 0.56f));

            it++;
        }
        else {
            if ((clock() - it->collision.timeSinceCollision) / 1000 < 0.25) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos[0], 0.20f, it->currentPos[2]));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
                RenderMesh(meshes["sphere"], shaders["AnimatedShader"], modelMatrix, glm::vec3(0.22f, 0.35f, 0.45f));

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos[0], 0.60f, it->currentPos[2]));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
                RenderMesh(meshes["sphere"], shaders["AnimatedShader"], modelMatrix, glm::vec3(0.64f, 0.90f, 0.57f));

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(it->currentPos[0], 1.0f, it->currentPos[2]));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
                RenderMesh(meshes["sphere"], shaders["AnimatedShader"], modelMatrix, glm::vec3(0.52f, 0.70f, 0.56f));

                it++;
            }
            else {
                it = enemies.erase(it);
            }

        }
    }

}



void Tema2::RenderHealthBar(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(healthBar->getHealthBarPosX(), healthBar->getHealthBarPosY());
    modelMatrix *= transform2D::Scale(0.4f, 0.5f);
    RenderMesh2D(meshes[healthBar->getHealthBar()->GetMeshID()], shaders["VertexColor"], modelMatrix);
    
    modelMatrix *= transform2D::Translate(healthBar->getHealthBarBorderPosX(), healthBar->getHealthBarBorderPosY());
    modelMatrix *= transform2D::Scale(0.5f, 0.5f);
    RenderMesh2D(meshes[healthBar->getHealthBarBorder()->GetMeshID()], shaders["VertexColor"], modelMatrix);
    

}


bool Tema2::CheckPlayerWallCollision(float nextStepX, float nextStepZ)
{
    camera->TranslateRight(nextStepX);
    camera->MoveForward(nextStepZ);

    player->setPosX(camera->GetTargetPosition().x - 0.5f);
    player->setPosZ(camera->GetTargetPosition().z - 0.5f);

    bool collision = false;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((maze->getGrid())[i][j] == 1) {
                if (((mazeStartPosition + i) <= (player->getPosX() + 0.5f + 0.3f) &&
                    (mazeStartPosition + i + 1.0f) >= (player->getPosX() + 0.5f - 0.3f)) &&
                    ((mazeStartPosition + j) <= (player->getPosZ() + 0.5f + 0.3f) &&
                        (mazeStartPosition + j + 1.0f) >= (player->getPosZ() + 0.5f - 0.3f))) {

                    collision = true;

                }
               
            }
        }
    }


    return collision;
}

bool Tema2::CheckPlayerEnemyCollision(float nextStepX, float nextStepZ)
{
    bool collision = false;
    list<Enemy>::iterator it = enemies.begin();

    while (it != enemies.end()) {

        float x = max(player->getPosX() + 0.5f - 0.3f, min(it->currentPos.x, player->getPosX() + 0.5f + 0.3f));
        float z = max(player->getPosZ() + 0.5f - 0.3f, min(it->currentPos.z, player->getPosZ() + 0.05f + 0.3f));

        float distance = sqrt(pow((x - it->currentPos.x), 2) +
            pow((z - it->currentPos.z), 2) * 1.0f);

        if (distance < 0.4f) {
            collision = true;
            enemyCollision = true;
        }

        it++;
    }

    return collision;
}

bool Tema2::CheckProjectileEnemyCollision(Projectile projectile, Enemy enemy)
{
    bool collision = false;

    float distance1 = sqrt(pow((projectile.currentPos.x + 0.5f - enemy.currentPos.x), 2) + pow((projectile.currentPos.y + 0.5f - enemy.currentPos.y), 2) +
        pow((projectile.currentPos.z + 0.5f - enemy.currentPos.z), 2) * 1.0f);

    float distance2 = sqrt(pow((projectile.currentPos.x + 0.5f - enemy.currentPos.x), 2) + pow((projectile.currentPos.y + 0.5f - 2 * enemy.currentPos.y), 2) +
        pow((projectile.currentPos.z + 0.5f - enemy.currentPos.z), 2) * 1.0f);

    float distance3 = sqrt(pow((projectile.currentPos.x + 0.5f - enemy.currentPos.x), 2) + pow((projectile.currentPos.y - 3 * enemy.currentPos.y), 2) +
        pow((projectile.currentPos.z + 0.5f - enemy.currentPos.z), 2) * 1.0f);


    if (distance1 < (0.2f + 0.125f) || distance2 < (0.2f + 0.125f) || distance3 < (0.2f + 0.125f)) {
        collision = true;
    }
        

    return collision;
}

bool Tema2::CheckProjectileWallCollision(Projectile projectile)
{
    bool collision = false;
   
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((maze->getGrid())[i][j] == 1) {

                float x = max(mazeStartPosition + i * 1.0f, min(projectile.currentPos.x + 0.5f, mazeStartPosition + i + 1.0f));
                float y = max(0.0f , min(projectile.currentPos.y, 1.0f));
                float z = max(mazeStartPosition + j * 1.0f, min(projectile.currentPos.z + 0.5f, mazeStartPosition + j + 1.0f));

                float distance = sqrt(pow((x - projectile.currentPos.x + 0.5f), 2) + pow((y - projectile.currentPos.y + 0.5f), 2) +
                    pow((z - projectile.currentPos.z + 0.5f), 2) * 1.0f);

                if (distance < 0.125f) {
                    collision = true;
                }
            }
        }
    }

    return collision;
}

bool Tema2::GameOver()
{  
    if (((clock() - startTime) / 1000 > time) || health <= 0 || player->getPosX() < mazeStartPosition ||
        player->getPosZ() < mazeStartPosition || player->getPosX() > SIZE - 1 || player->getPosZ() > SIZE - 1)  {
        return true;
    }

    return false;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;
    enemyCollision = false;

    if (window->KeyHold(GLFW_KEY_W) && (CheckPlayerWallCollision(0, cameraSpeed * deltaTime) || 
        CheckPlayerEnemyCollision(0, cameraSpeed * deltaTime))) {
        camera->MoveForward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A) && (CheckPlayerWallCollision(-cameraSpeed * deltaTime, 0) ||
        CheckPlayerEnemyCollision(-cameraSpeed * deltaTime, 0))) {
         camera->TranslateRight(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S) && (CheckPlayerWallCollision(0, -cameraSpeed * deltaTime) ||
        CheckPlayerEnemyCollision(0, -cameraSpeed * deltaTime))) {
        camera->MoveForward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D) && (CheckPlayerWallCollision(cameraSpeed * deltaTime, 0) ||
        CheckPlayerEnemyCollision(cameraSpeed * deltaTime, 0))) {
        camera->TranslateRight(-cameraSpeed * deltaTime);
    }

    if (enemyCollision == true) {
        health -= 1;
    }

    player->setPosX(camera->GetTargetPosition().x - 0.5f);
    player->setPosZ(camera->GetTargetPosition().z - 0.5f);

}

void Tema2::UpdateDistanceToTarget(float newDistance) 
{
    camera->distanceToTarget = newDistance;
    player->setOffsetCameraX(camera->distanceToTarget * sin(player->getRotatingAngle()));
    player->setOffsetCameraZ(camera->distanceToTarget * cos(player->getRotatingAngle()));
}

void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }

    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
        if (thirdPerson == false) {

            Tema2::UpdateDistanceToTarget(4);
            camera->Set(glm::vec3(player->getPosX() + 0.5f - player->getOffsetCameraX(), 2, player->getPosZ() + 0.5f - player->getOffsetCameraZ()),
                glm::vec3(player->getPosX() + 0.5f, 2, player->getPosZ() + 0.5f), glm::vec3(0, 1, 0));

            thirdPerson = true;

        } else {
            Tema2::UpdateDistanceToTarget(0.1);
            camera->Set(glm::vec3(player->getPosX() + 0.5f - player->getOffsetCameraX(), 1, player->getPosZ() + 0.5f - player->getOffsetCameraZ()),
                glm::vec3(player->getPosX() + 0.5f, 1, player->getPosZ() + 0.5f), glm::vec3(0, 1, 0));
            
            thirdPerson = false;

        }
    }

    if (key == GLFW_KEY_SPACE) {
        if (thirdPerson == false) {
            AddProjectile(glm::vec3(player->getPosX(), 0.5f, player->getPosZ()), projectiles, camera->forward);
        }
    }
 
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

        player->setRotatingAngle(player->getRotatingAngle() - deltaX * sensivityOY);

        if (!thirdPerson) {
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (thirdPerson) {
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
        
    }
    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
