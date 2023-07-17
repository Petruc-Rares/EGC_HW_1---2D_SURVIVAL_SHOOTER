#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

#define PLAYER_LIVES 4
#define TIME_BEFORE_NEW_SPAWN 3
#define TIME_SHOW_POINTS 1
#define MAX_RANGE_PROJECTILE 3

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

double maxDouble(double a, double b) {
    return (a > b) ? a : b;
}

double minDouble(double a, double b) {
    return (a < b) ? a : b;
}

bool collisionEnemyProjectile(Enemy& enemy, Proiectile& projectile) {
    double x = maxDouble(enemy.getOx(), minDouble(projectile.getCrtOxHitbox(), enemy.getOx() + enemy.getHitboxWidth()));
    double y = maxDouble(enemy.getOy(), minDouble(projectile.getCrtOyHitbox(), enemy.getOy() + enemy.getHitboxHeight()));

    double distance = sqrt((x - projectile.getCrtOxHitbox()) * (x - projectile.getCrtOxHitbox()) +
        (y - projectile.getCrtOyHitbox()) * (y - projectile.getCrtOyHitbox()));

    if (distance < projectile.getRadius()) {
        return true;
    }

    return false;
}

int checkHitEnemies(std::vector<Proiectile> &projectiles, std::vector<Enemy>& enemies) {
    int no_hit = 0;

    for (int i = 0; i < projectiles.size(); i++) {
        Proiectile crtProjectile = projectiles[i];

        for (int j = 0; j < enemies.size(); j++) {
            Enemy crtEnemy = enemies[j];

            if (collisionEnemyProjectile(crtEnemy, crtProjectile)) {
                enemies.erase(std::next(enemies.begin(), j));
                projectiles.erase(std::next(projectiles.begin(), i));
                no_hit += 1;
                i--;
                break;
            }
        }
    }

    return no_hit;
}

bool collisionProjectileMap(Proiectile& projectile) {

    if ((projectile.getCrtOxHitbox() - projectile.getRadius() < -2 ) || (projectile.getCrtOxHitbox() + projectile.getRadius() > 6) ||
        (projectile.getCrtOyHitbox() - projectile.getRadius() < -2) || (projectile.getCrtOyHitbox() +  projectile.getRadius() > 6)) {
        return true;
    }

    return false;
}


bool collisionProjectileObstacles(Proiectile& projectile, const std::vector<Obstacle>& obstacles) {

    for (int i = 0; i < obstacles.size(); i++) {
        Obstacle crtObstacle = obstacles[i];

        double x = maxDouble(crtObstacle.getCrtOxHitbox(), minDouble(projectile.getCrtOxHitbox(), crtObstacle.getCrtOxHitbox() + crtObstacle.getHitboxWidth()));
        double y = maxDouble(crtObstacle.getCrtOyHitbox(), minDouble(projectile.getCrtOyHitbox(), crtObstacle.getCrtOyHitbox() + crtObstacle.getHitboxHeight()));

        double distance = sqrt((x - projectile.getCrtOxHitbox()) * (x - projectile.getCrtOxHitbox()) +
            (y - projectile.getCrtOyHitbox()) * (y - projectile.getCrtOyHitbox()));

        if (distance < projectile.getRadius()) {
            return true;
        }
    }
    return false;
}

bool collisionPlayerEnemy(Player_tema& player,  Enemy& enemy) {

    double x = maxDouble(enemy.getOx(), minDouble(player.getOx(), enemy.getOx() + enemy.getHitboxWidth()));
    double y = maxDouble(enemy.getOy(), minDouble(player.getOy(), enemy.getOy() + enemy.getHitboxHeight()));

    double distance = sqrt((x - player.getOx()) * (x - player.getOx()) +
        (y - player.getOy()) * (y - player.getOy()));

    if (distance < player.getRadius()) {
        return true;
    }

    return false;
}

bool collisionPlayerObstacles(Player_tema& player, const std::vector<Obstacle>& obstacles) {

    for (int i = 0; i < obstacles.size(); i++) {
        Obstacle crtObstacle = obstacles[i];

        double x = maxDouble(crtObstacle.getCrtOxHitbox(), minDouble(player.getOx(), crtObstacle.getCrtOxHitbox() + crtObstacle.getHitboxWidth()));
        double y = maxDouble(crtObstacle.getCrtOyHitbox(), minDouble(player.getOy(), crtObstacle.getCrtOyHitbox() + crtObstacle.getHitboxHeight()));

        double distance = sqrt((x - player.getOx()) * (x - player.getOx()) +
            (y - player.getOy()) * (y - player.getOy()));
        
        if (distance < player.getRadius()) {
            return true;
        }
    }
    return false;
}


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    srand(time(NULL));

    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 4;   // logic width
    logicSpace.height = 4;  // logic height

    oxCenterPlayer = (logicSpace.x + logicSpace.width) / 2;
    oyCenterPlayer = (logicSpace.y + logicSpace.height) / 2;

    glm::vec3 corner = glm::vec3(0, 0, 0);
    length = 1;

    // typical mesh square which can be easily reused by scaling
    // is used for creating the map
    Mesh* square1 = object2D_tema::CreateSquare("square1", corner, length, glm::vec3(1, 1, 0), true);
    AddMeshToList(square1);

    glm::vec3 center = glm::vec3(0, 0, 0);
    radius = 0.3f;

    // meshes used for the player
    Mesh* circleBody = object2D_tema::CreateCircle("circleBody", center, radius, glm::vec3(0, 0, 1), true);
    AddMeshToList(circleBody);

    Mesh* circleLeftEye = object2D_tema::CreateCircle("circleLeftEye", center + glm::vec3(-radius / 2, -5 * radius/6, 0), radius / 6, glm::vec3(1, 0, 0), true);
    AddMeshToList(circleLeftEye);

    Mesh* circleRightEye = object2D_tema::CreateCircle("circleRightEye", center + glm::vec3(radius / 2, -5 * radius / 6, 0), radius / 6, glm::vec3(1, 0, 0), true);
    AddMeshToList(circleRightEye);

    // meshes used for the enemy
    Mesh* enemyBody = object2D_tema::CreateRectangle("enemyBody", corner - glm::vec3(0.4, 0.3, 0), 0.8, 0.6, glm::vec3(1, 0, 0), true);
    AddMeshToList(enemyBody);

    Mesh* enemyLeftEye = object2D_tema::CreateRectangle("enemyLeftEye", corner + glm::vec3(-0.35, -0.25, 0), 0.1, 0.05, glm::vec3(0, 0, 1), true);
    AddMeshToList(enemyLeftEye);

    Mesh* enemyRightEye = object2D_tema::CreateRectangle("enemyRightEye", corner + glm::vec3(0.25, -0.25, 0), 0.1, 0.05, glm::vec3(0, 0, 1), true);
    AddMeshToList(enemyRightEye);

    // initialize default enemy
    enemy.setCrtOxHitbox(-1);
    enemy.setCrtOyHitbox(1);
    enemy.setOxHitbox(-0.4);
    enemy.setOyHitbox(-0.3);
    enemy.setHitboxWidth(0.8);
    enemy.setHitboxHeight(0.6);
    enemy.setOx(-1);
    enemy.setOy(1);
    enemy.setThetaEnemy(0);

    // initialize player
    player.setCenter(glm::vec3(2, 2, 0));
    player.setHitboxHeight(0.6f);
    player.setHitboxWidth(0.6f);
    player.setRadius(0.3f);
    player.setOxHitbox(1.7);
    player.setOyHitbox(1.7);
    player.setCrtOxHitbox(1.7);
    player.setCrtOyHitbox(1.7);
    player.setOxTranslate(0);
    player.setOyTranslate(0);
    player.setOx(2);
    player.setOy(2);

    oxTranslatePlayer = 0;
    oyTranslatePlayer = 0;
    thetaCursor = 0;

    // meshes and objects used for the obstacles
    obstacles = {};

    Mesh* obstacle_0 = object2D_tema::CreateSquare("obstacle_0", glm::vec3(-1, -1, 0),
        2, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(obstacle_0);

    Obstacle obstacle0;
    obstacle0.setCrtOxHitbox(-1);
    obstacle0.setCrtOyHitbox(-1);
    obstacle0.setHitboxWidth(2);
    obstacle0.setHitboxHeight(2);

    obstacles.push_back(obstacle0);

    Mesh* obstacle_1 = object2D_tema::CreateRectangle("obstacle_1", glm::vec3(1, -1, 0),
        2, 1, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(obstacle_1);

    Obstacle obstacle1;
    obstacle1.setCrtOxHitbox(1);
    obstacle1.setCrtOyHitbox(-1);
    obstacle1.setHitboxWidth(2);
    obstacle1.setHitboxHeight(1);

    obstacles.push_back(obstacle1);

    Mesh* obstacle_2 = object2D_tema::CreateRectangle("obstacle_2", glm::vec3(0, 3, 0),
        0.5, 2, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(obstacle_2);

    Obstacle obstacle2;
    obstacle2.setCrtOxHitbox(0);
    obstacle2.setCrtOyHitbox(3);
    obstacle2.setHitboxWidth(0.5);
    obstacle2.setHitboxHeight(2);

    obstacles.push_back(obstacle2);

    Mesh* obstacle_3 = object2D_tema::CreateRectangle("obstacle_3", glm::vec3(3, 3, 0),
        2, 1.5, glm::vec3(0.5, 0.5, 0.5), true);
    AddMeshToList(obstacle_3);

    Obstacle obstacle3;
    obstacle3.setCrtOxHitbox(3);
    obstacle3.setCrtOyHitbox(3);
    obstacle3.setHitboxWidth(2);
    obstacle3.setHitboxHeight(1.5);

    obstacles.push_back(obstacle3);

    // mesh used for the projectile
    Mesh* projectile = object2D_tema::CreateSquare("projectile", glm::vec3(-0.1, -0.1, 0),
        0.2, glm::vec3(0, 0, 0), true);
    AddMeshToList(projectile);

    projectiles = {};
    enemies = {};

    // initial number of lives and no of points
    player_lives = PLAYER_LIVES;
    no_points = 0;

    Mesh* healthbar = object2D_tema::CreateRectangle("healthbar", corner, 0.8, 0.2, glm::vec3(0, 1, 0), false);
    AddMeshToList(healthbar);

    Mesh* healthbar_life = object2D_tema::CreateRectangle("healthbar_life", corner, 0.8, 0.2, glm::vec3(0, 1, 0), true);
    AddMeshToList(healthbar_life);

    deltaTimeEnemySpawn = 0;
    deltaTimeLastShoot = 0;
    deltaTimeShowPoints = 0;
}


// 2D visualization matrix
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


// Uniform 2D visualization matrix (same scale factor on x and y axes)
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

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c/9324796
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the window
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    // check if game is not over
    if (player_lives > 0) {

        deltaTimeEnemySpawn += deltaTimeSeconds;

        // check if a new enemy can be spawned
        if (deltaTimeEnemySpawn > TIME_BEFORE_NEW_SPAWN) {
            int iSecret = rand() % obstacles.size();

            double secretOx = fRand(obstacles[iSecret].getCrtOxHitbox(), obstacles[iSecret].getCrtOxHitbox() + obstacles[iSecret].getHitboxWidth());
            double secretOy = fRand(obstacles[iSecret].getCrtOyHitbox(), obstacles[iSecret].getCrtOyHitbox() + obstacles[iSecret].getHitboxHeight());

            enemy.setCrtOxHitbox(secretOx);
            enemy.setCrtOyHitbox(secretOy);

            enemy.setOx(secretOx);
            enemy.setOy(secretOy);

            // range of speed that a enemy can have
            double secretSpeed = fRand(0.5, 1);
            enemy.setSpeed(secretSpeed);

            enemies.push_back(enemy);

            // reset it back to 0
            deltaTimeEnemySpawn = 0;
        }

        // check for collision player-enemy
        for (int i = 0; i < enemies.size(); i++) {
            Enemy crtEnemy = enemies[i];
    
            if (collisionPlayerEnemy(player, crtEnemy)) {
                enemies.erase(std::next(enemies.begin(), i));
                i--;
                player_lives--;
                continue;
            }
        }

        centerPlayer = glm::vec3(resolution.x / 2, resolution.y / 2, 0);

        oxCoordPlayer = centerPlayer[0];
        oyCoordPlayer = centerPlayer[1];

        currLogicOx = oxCenterPlayer + oxTranslatePlayer;
        currLogicOy = oyCenterPlayer + oyTranslatePlayer;

        deltaTimeShooting = deltaTimeSeconds;
        deltaTimeLastShoot -= deltaTimeSeconds;
        deltaTimeShowPoints -= deltaTimeSeconds;

        no_points += checkHitEnemies(projectiles, enemies);

        // show points periodically
        if (deltaTimeShowPoints < 0) {
            deltaTimeShowPoints = TIME_SHOW_POINTS;
            printf("Current score is %d\n", no_points);
        }

        // make enemies follow the player
        for (int i = 0; i < enemies.size(); i++) {
            double diffOx = player.getOx() - (enemies[i].getOx() + enemies[i].getHitboxWidth() / 2);
            double diffOy = player.getOy() - (enemies[i].getOy() + enemies[i].getHitboxHeight() / 2);

            double thetaEnemy = enemies[i].getThetaEnemy();

            if ((diffOy > 0) && (diffOx > 0)) {
                // player is above and at the right of the current enemy
                thetaEnemy = M_PI_2 + atan(abs((double)diffOy / diffOx));

                // collision enemy - margin map right
                if (enemies[i].getOx() + enemies[i].getHitboxWidth() + deltaTimeSeconds * enemies[i].getSpeed() * sin(thetaEnemy) > 6) {
                    thetaEnemy = M_PI;
                }
                // collision enemy - margin map up
                else if (enemies[i].getOy() + enemies[i].getHitboxHeight() - deltaTimeSeconds * enemies[i].getSpeed() * cos(thetaEnemy) > 5.9f) {
                    thetaEnemy = M_PI_2;
                }
            }
            else if ((diffOy > 0) && (diffOx < 0)) {
                // player is above and at the left of the current enemy

                thetaEnemy = -M_PI_2 - atan(abs((double)diffOy / diffOx));

                // collision enemy - margin map left
                if (enemies[i].getOx() + deltaTimeSeconds * enemies[i].getSpeed() * sin(thetaEnemy) < -2) {
                    thetaEnemy = -M_PI;
                }
                // collision enemy - margin map up
                else if (enemies[i].getOy() + enemies[i].getHitboxHeight() - deltaTimeSeconds * enemies[i].getSpeed() * cos(thetaEnemy) > 5.9f) {
                    thetaEnemy = -M_PI_2;
                }
            }
            else if ((diffOy < 0) && (diffOx)) {
                // player is below the current enemy

                thetaEnemy = atan((double)diffOx / abs(diffOy));

                // collision enemy - margin map left
                if ((diffOx < 0) && (enemies[i].getOx() + deltaTimeSeconds * enemies[i].getSpeed() * sin(thetaEnemy) < -2)) {
                    thetaEnemy = 0;
                }
                // collision enemy - margin map right
                else if ((diffOx > 0) && (enemies[i].getOx() + enemies[i].getHitboxWidth() + deltaTimeSeconds * enemies[i].getSpeed() * sin(thetaEnemy) > 6)) {
                    thetaEnemy = 0;
                }
                // collision enemy - margin map down
                else if ((diffOx > 0) && (enemies[i].getOy() + enemies[i].getHitboxHeight() - deltaTimeSeconds * enemies[i].getSpeed() * cos(thetaEnemy) < -2)) {
                    thetaEnemy = M_PI_2;
                }
                // collision enemy - margin map down
                else if ((diffOx < 0) && (enemies[i].getOy() + enemies[i].getHitboxHeight() - deltaTimeSeconds * enemies[i].getSpeed() * cos(thetaEnemy) < -2)) {
                    thetaEnemy = -M_PI_2;
                }
            }
            else if (diffOx == 0) {
                thetaEnemy = (diffOy < 0) ? -M_PI : 0;
            }
            else if (diffOy == 0) {
                thetaEnemy = (diffOx < 0) ? -M_PI / 2 : M_PI / 2;
            }
            enemies[i].setOx(enemies[i].getOx() + deltaTimeSeconds * enemies[i].getSpeed() * sin(thetaEnemy));
            enemies[i].setOy(enemies[i].getOy() - deltaTimeSeconds * enemies[i].getSpeed() * cos(thetaEnemy));
            enemies[i].setThetaEnemy(thetaEnemy);
        }

        DrawScene(visMatrix);
    } else {
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
 }


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix *
        transform2D::Translate(player.getOx() + logicSpace.width / 1.5, player.getOy() +  logicSpace.height / 2.35);

    RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);


    modelMatrix *= transform2D::Scale((float) player_lives/4, 1);

    RenderMesh2D(meshes["healthbar_life"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(player.getOx(), player.getOy())
                            * transform2D::Rotate(thetaCursor);

    // player
    RenderMesh2D(meshes["circleLeftEye"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["circleRightEye"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["circleBody"], shaders["VertexColor"], modelMatrix);


    // obstacles
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["obstacle_0"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle_1"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle_2"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["obstacle_3"], shaders["VertexColor"], modelMatrix);


    // projectiles
    for (int i = 0; i < projectiles.size(); i++) {

        Proiectile crtProjectile = projectiles[i];
        double crtProjectileOx = crtProjectile.getCrtOxHitbox();
        double crtProjectileOy = crtProjectile.getCrtOyHitbox();

        // check if projectile intersects with the obstacles or margins of the map
        // to know if it is needed to render the projectile or not
        if ((collisionProjectileObstacles(crtProjectile, obstacles)) ||
            (collisionProjectileMap(crtProjectile))) {
            projectiles.erase(std::next(projectiles.begin(), i));
            i--;
            continue;
        }

        double nextOx = (crtProjectileOx + crtProjectile.getRadius() * sin(crtProjectile.getThetaCursor()) * deltaTimeShooting * 10);
        double nextOy = (crtProjectileOy - crtProjectile.getRadius() * cos(crtProjectile.getThetaCursor()) * deltaTimeShooting * 10);

        projectiles[i].setDistance(
            projectiles[i].getDistance() +
            sqrt(((crtProjectileOx - nextOx) * (crtProjectileOx - nextOx)) + ((crtProjectileOy - nextOy) * (crtProjectileOy - nextOy))));

        if (projectiles[i].getDistance() > MAX_RANGE_PROJECTILE) {
            projectiles.erase(std::next(projectiles.begin(), i));
            i--;
            continue;
        }

        modelMatrix = visMatrix * transform2D::Translate(crtProjectile.getRadius() * sin(crtProjectile.getThetaCursor()),
                                                         -crtProjectile.getRadius() * cos(crtProjectile.getThetaCursor()))      
                                * transform2D::Translate(crtProjectile.getCrtOxHitbox(),
                                    crtProjectile.getCrtOyHitbox())
                                * transform2D::Rotate(crtProjectile.getThetaCursor());

        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

        projectiles[i].setCrtOxHitbox(nextOx);
        projectiles[i].setCrtOyHitbox(nextOy);
    }

    // ENEMY
    for (int i = 0; i < enemies.size(); i++) {
        modelMatrix = visMatrix * transform2D::Translate(enemies[i].getOx(), enemies[i].getOy())
            * transform2D::Translate(0.4, 0.3)
            * transform2D::Rotate(enemies[i].getThetaEnemy());

        RenderMesh2D(meshes["enemyLeftEye"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemyRightEye"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);
    }

    // MAP
    modelMatrix = visMatrix * transform2D::Translate(-2, -2)
                            * transform2D::Scale(8, 8);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

    Player_tema copy_player;

    copy_player.setOx(player.getOx());
    copy_player.setOy(player.getOy());
    copy_player.setRadius(player.getRadius());

    if (window->KeyHold(GLFW_KEY_W)) {
        copy_player.setOy(copy_player.getOy() + deltaTime * 2);

        if (!collisionPlayerObstacles(copy_player, obstacles)) {
            if (currLogicOy + radius + radius / 6 + deltaTime * 2 < 6) {
                logicSpace.y += deltaTime * 2;
                    oyTranslatePlayer += deltaTime * 2;

                    player.setOy(player.getCenter()[1] + oyTranslatePlayer);
                    player.setCrtOyHitbox(player.getOyHitbox() + oyTranslatePlayer);
                    player.setOyTranslate(oyTranslatePlayer);
            }
        }
    }
    else if (window->KeyHold(GLFW_KEY_A)) {
        copy_player.setOx(copy_player.getOx() - deltaTime * 2);

        if (!collisionPlayerObstacles(copy_player, obstacles)) {
            if (currLogicOx - radius - radius / 6 - deltaTime * 2 > -2) {
                logicSpace.x -= deltaTime * 2;
                    oxTranslatePlayer -= deltaTime * 2;

                    player.setOx(player.getCenter()[0] + oxTranslatePlayer);
                    player.setCrtOxHitbox(player.getOxHitbox() + oxTranslatePlayer);
                    player.setOxTranslate(oxTranslatePlayer);
            }
        }
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        copy_player.setOy(copy_player.getOy() - deltaTime * 2);

        if (!collisionPlayerObstacles(copy_player, obstacles)) {
            if (currLogicOy - radius - radius / 6 - deltaTime * 2 > -2) {
                logicSpace.y -= deltaTime * 2;
                oyTranslatePlayer -= deltaTime * 2;

                player.setOy(player.getCenter()[1] + oyTranslatePlayer);
                player.setCrtOyHitbox(player.getOyHitbox() + oyTranslatePlayer);
                player.setOyTranslate(oyTranslatePlayer);
            }
        }
    }
    else if (window->KeyHold(GLFW_KEY_D)) {
        copy_player.setOx(copy_player.getOx() + deltaTime * 2);

        if (!collisionPlayerObstacles(copy_player, obstacles)) {
            if (currLogicOx + radius + radius / 6 + deltaTime * 2 < 6) {
                logicSpace.x += deltaTime * 2;
                oxTranslatePlayer += deltaTime * 2;

                player.setOx(player.getCenter()[0] + oxTranslatePlayer);
                player.setCrtOxHitbox(player.getOxHitbox() + oxTranslatePlayer);
                player.setOxTranslate(oxTranslatePlayer);
            }
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    //glm::vec3 mouseCoords = visMatrixAux * glm::vec3(mouseX, mouseY, 0);

    int diffOx = mouseX - oxCoordPlayer;
    int diffOy = mouseY - oyCoordPlayer;

    //cout << "coord mouse: " << mouseX << ", " << mouseY << endl;
    //cout << "coord player:" << oxCoordPlayer << ", " << oyCoordPlayer << endl;
    //cout << diffOx << ", " << diffOy << endl;

    if ((diffOy < 0) && (diffOx > 0)) {
     //   cout << "Cadran Dreapta sus" << endl;
        thetaCursor = M_PI_2 + atan(abs((double)diffOy / diffOx));
    }
    else if ((diffOy < 0) && (diffOx < 0)) {
      //  cout << "Cadran Stanga sus" << endl;
        thetaCursor = -M_PI_2 - atan(abs((double)diffOy / diffOx));
    }
    else if ((diffOy > 0) && (diffOx)){
        thetaCursor = atan((double)diffOx / diffOy);
    }
    else if (diffOx == 0) {
        thetaCursor = (diffOy < 0) ? -M_PI : 0;
    }
    else if (diffOy == 0) {
        thetaCursor = (diffOx < 0) ? -M_PI / 2 : M_PI / 2;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        if (deltaTimeLastShoot <= 0) {
            deltaTimeLastShoot = 0.5;
            Proiectile new_projectile;

            new_projectile.setCrtOxHitbox(player.getOx());
            new_projectile.setCrtOyHitbox(player.getOy());

            new_projectile.setThetaCursor(thetaCursor);

            new_projectile.setRadius(0.2 * sqrt(2) / 2);
            new_projectile.setDistance(0);

            projectiles.push_back(new_projectile);
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
