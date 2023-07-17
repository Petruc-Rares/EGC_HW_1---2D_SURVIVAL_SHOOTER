#pragma once

#include "components/simple_scene.h"
//#include "GameElement.h"
#include "Player_tema.h"
#include "Obstacle.h"
#include "Proiectile.h"
#include "Enemy.h"

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

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float length;
        float radius;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        glm::vec3 centerPlayer;
        // coord of player in gate (always in center of viewport)
        int oxCoordPlayer, oyCoordPlayer;
        double thetaCursor;
        // translation of player in logic coords
        double oxTranslatePlayer, oyTranslatePlayer;
        // logic coords of player at initialization
        double oxCenterPlayer, oyCenterPlayer;
        // logic coords of player at time accessed
        double currLogicOx, currLogicOy;
        // speed of a projectile relative to the deltaTimeSeconds
        float deltaTimeShooting;
        // used for fire rate
        float deltaTimeLastShoot;
        // used for spawning a new enemy
        float deltaTimeEnemySpawn;
        // used for showing the points regularly
        float deltaTimeShowPoints;

        // the player of the game
        Player_tema player;

        // used for initialization of each new enemy
        Enemy enemy;

        std::vector<Obstacle> obstacles;
        std::vector<Proiectile> projectiles;
        std::vector<Enemy> enemies;

        int player_lives;
        int no_points;
    };
}
