#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 382;
    windowDimensions[1] = 382;
    InitWindow(windowDimensions[0], windowDimensions[1], "Classy Clash!");
    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.f};
    Character knight{windowDimensions[0], windowDimensions[1]};
    Prop props[2]{
        Prop(Vector2{600, 400}, LoadTexture("nature_tileset/Rock.png")),
        Prop(Vector2{300, 500}, LoadTexture("nature_tileset/Log.png"))};
    Enemy goblin{
        Vector2{700, 200},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};
    Enemy slime {
        Vector2{400, 600},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};
    Enemy *enemies[]{
        &goblin,
        &slime};
    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while(!WindowShouldClose())   // one passage per frame
    {
        // delta time
        const float dT {GetFrameTime()};
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);   

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);
        // draw the props
        for(Prop prop : props)
            prop.Render(knight.getWorldPos());

        if (!knight.getAlive()) {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else{
            std::string knightHealth{"Health: "};
            knightHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        knight.tick(dT);
        // check map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowDimensions[0] > map.width * mapScale ||
            knight.getWorldPos().y + windowDimensions[1] > map.height * mapScale)
        {
            knight.undoMovement();
        }
        // check collisions with props
        for (Prop prop : props) {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
                knight.undoMovement();
        }

        for(Enemy * enemy : enemies) {
            enemy->tick(dT);
        }
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies){
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }
        
        EndDrawing();
    }

    // free resources
    UnloadTexture(map);
    CloseWindow();
}