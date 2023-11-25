#include "raylib.h"
#include "health.h"
#include "Score.h"
#define MAX_ENEMIES 100
#define MAX_BULLETS 100

typedef struct {
    Vector2 position;
    bool active;
} Bullet;

typedef struct {
    Vector2 position;
    bool active;
} Enemy;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Asrtoworld");

    Vector2 playerPosition = { screenWidth / 2, screenHeight / 2 };
    int playerRadius = 20;
    PlayerHealth playerHealth;
    PlayerScore pScore;
    InitPlayerHealth(&playerHealth, 100);
    InitScore(&pScore, 0);

    Bullet bullets[MAX_BULLETS] = { 0 };
    int bulletSpeed = 5;
    
    Enemy enemies[MAX_ENEMIES] = { 0 };
    int enemySpeed = 2;
    float speedTimer = 0.0f;
    const float enemySpeedIncrease = 2.0f;

    bool gameOver = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!gameOver) {

            speedTimer += deltaTime;
            if(speedTimer >= enemySpeedIncrease)
            {
                enemySpeed += 0.5;
                speedTimer = 0.0f;
            }

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerPosition.x += 5;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) playerPosition.x -= 5;
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) playerPosition.y += 5;
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) playerPosition.y -= 5;

            if (IsKeyPressed(KEY_SPACE)) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].position = playerPosition;
                        bullets[i].active = true;
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    bullets[i].position.y += bulletSpeed;

                    if (bullets[i].position.y > 450) {
                        bullets[i].active = false;
                    }
                }
            }

            if (GetRandomValue(0, 100) < 5) {
                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (!enemies[i].active) {
                        enemies[i].position.x = GetRandomValue(0, screenWidth);
                        enemies[i].position.y = screenHeight + 10;
                        enemies[i].active = true;
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    for (int j = 0; j < MAX_ENEMIES; j++) {
                        if (enemies[j].active && CheckCollisionCircles(bullets[i].position, 5, enemies[j].position, 10)) {
                            bullets[i].active = false;
                            enemies[j].active = false;
                            IncreaseScore(&pScore, 10);
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    enemies[i].position.y -= enemySpeed;

                    if (CheckCollisionCircles(playerPosition, playerRadius, enemies[i].position, 10)) {
                        DecreasePlayerHealth(&playerHealth, 10);
                        enemies[i].active = false;
                    }

                    if (enemies[i].position.y < 0) {
                        enemies[i].active = false;
                    }
                }
            }

            if (GetPlayerHealth(&playerHealth) <= 0) {
                gameOver = true;
            }
        } else {
            if (IsKeyPressed(KEY_R)) {
                InitPlayerHealth(&playerHealth, 100);
                InitScore(&pScore, 0);
                gameOver = false;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameOver) {
            DrawCircleV(playerPosition, playerRadius, RED);
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    DrawCircleV(bullets[i].position, 5, BLUE);
                }
            }

            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    DrawCircleV(enemies[i].position, 10, GREEN);
                }
            }

            DrawText(TextFormat("Health: %d", GetPlayerHealth(&playerHealth)), 10, 10, 20, RED);
            DrawText(TextFormat("Score: %d", GetPlayerScore(&pScore)), 10, 30, 20, RED);
        } else {
            DrawText("Game Over", screenWidth / 2 - MeasureText("Game Over", 40) / 2, screenHeight / 2 - 20, 40, RED);
            DrawText(TextFormat("Final Score: %d",GetPlayerScore(&pScore)), screenWidth / 2 - MeasureText("Final Score: XXX", 20) / 2, screenHeight / 2 + 20, 20, DARKGRAY);
            DrawText("Press 'R' to restart", screenWidth / 2 - MeasureText("Press 'R' to restart", 20) / 2, screenHeight / 2 + 60, 20, DARKGRAY);
            enemySpeed = 2;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}