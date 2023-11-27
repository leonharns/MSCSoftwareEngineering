#include "raylib.h"
#include "health.h"
#include "Score.h"
#define MAX_ENEMIES 100
#define MAX_BULLETS 100
#define MAX_PARTICLES 200
#define MAX_POWERUPS 1

typedef struct {
    Vector2 position;
    bool active;
} Bullet;

typedef struct {
    Vector2 position;
    bool active;
} Enemy;

typedef struct {
    Vector2 position;
    bool active;
} Powerup;

typedef struct {
    Vector2 position;
    Vector2 speed;
    Color color;
    float radius;
    bool active;
} Particle;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "ZambizeeNights");

    Vector2 playerPosition = { screenWidth / 2, screenHeight / 2 };
    int playerRadius = 20;
    PlayerHealth playerHealth;
    PlayerScore pScore;
    InitPlayerHealth(&playerHealth, 100);
    InitScore(&pScore, 0);

    Texture2D backgroundTexture = LoadTexture("Background.png");
    Vector2 backgroundPosition = { 0, 0 };
    float backgroundScrollSpeed = 2.0f;

    Texture2D bulletTexture = LoadTexture("bullet.png");
    Bullet bullets[MAX_BULLETS] = { 0 };
    int bulletSpeed = 5;
    
    Enemy enemies[MAX_ENEMIES] = { 0 };
    int enemySpeed = 2;
    float speedTimer = 0.0f;
    const float enemySpeedIncrease = 2.0f;
    Image enemyImage = LoadImage("zambie.png");
    ImageResize(&enemyImage, enemyImage.width / 8, enemyImage.height / 10);
    Texture2D enemyTexture = LoadTextureFromImage(enemyImage);
    UnloadImage(enemyImage);

    Texture2D healthTexture = LoadTexture("Health.png");
    Powerup powerups[MAX_POWERUPS] = { 0 };
    int powerupSpawnTimer = 0;
    const int powerupSpawnInterval = 30000;

    bool gameOver = false;

    Particle particles[MAX_PARTICLES] = { 0 };
    Color bloodColor = { 200, 0, 0, 255 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!gameOver) {
            
            backgroundPosition.y += backgroundScrollSpeed;
            if (backgroundPosition.y >= screenHeight) {
                backgroundPosition.y = 0;
            }

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
             
            powerupSpawnTimer += GetFrameTime() * 1000;

            if (powerupSpawnTimer >= powerupSpawnInterval) {
                powerupSpawnTimer = 0;

                for (int i = 0; i < MAX_POWERUPS; i++) {
                    if (!powerups[i].active) {
                        powerups[i].position.x = GetRandomValue(0, screenWidth);
                        powerups[i].position.y = GetRandomValue(0, screenHeight);
                        powerups[i].active = true;
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (powerups[i].active && CheckCollisionCircles(playerPosition, playerRadius, powerups[i].position, 10)) {
                    InitPlayerHealth(&playerHealth, 100);
                    powerups[i].active = false;
                }
            }

            if (GetRandomValue(0, 100) < 10) {
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
                            for (int k = 0; k < MAX_PARTICLES; k++) {
                                if (!particles[k].active) {
                                    particles[k].position = enemies[j].position;
                                    particles[k].speed = (Vector2){ GetRandomValue(-5, 2), GetRandomValue(-5, 2) };
                                    particles[k].color = bloodColor;
                                    particles[k].radius = GetRandomValue(1, 2);
                                    particles[k].active = true;
                                }
                            }
                        }
                    }
                }
            }
              for (int i = 0; i < MAX_PARTICLES; i++) {
                if (particles[i].active) {
                    particles[i].position.x += particles[i].speed.x;
                    particles[i].position.y += particles[i].speed.y;

                    if (particles[i].position.x < 0 || particles[i].position.x > screenWidth || particles[i].position.y < 0 || particles[i].position.y > screenHeight) {
                        particles[i].active = false;
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

                    if (enemies[i].position.y < -10) {
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

        DrawTexture(backgroundTexture, backgroundPosition.x, backgroundPosition.y, WHITE);
        DrawTexture(backgroundTexture, backgroundPosition.x, backgroundPosition.y - screenHeight, WHITE);

        if (!gameOver) {
            DrawCircleV(playerPosition, playerRadius, RED);
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    DrawTexture(bulletTexture, bullets[i].position.x - bulletTexture.width / 2, bullets[i].position.y - bulletTexture.height / 2, WHITE);
                }
            }

            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    DrawTexture(enemyTexture, enemies[i].position.x - enemyTexture.width / 2, enemies[i].position.y - enemyTexture.height / 2, WHITE);
                }
            }

            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (powerups[i].active) {
                    DrawTexture(healthTexture, powerups[i].position.x - healthTexture.width / 2, powerups[i].position.y - healthTexture.height / 2, WHITE);
                }   
            }

            for (int i = 0; i < MAX_PARTICLES; i++) {
                if (particles[i].active) {
                    DrawCircleV(particles[i].position, particles[i].radius, particles[i].color);
                }
            }

            DrawText(TextFormat("Health: %d", GetPlayerHealth(&playerHealth)), 10, 10, 20, RED);
            DrawText(TextFormat("Score: %d", GetPlayerScore(&pScore)), 10, 30, 20, RED);
        } 
        else {
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