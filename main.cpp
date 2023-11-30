#include "raylib.h"
#include "health.h"
#include "Score.h"
#include "Enums.h"
#include <raymath.h>
#define MAX_ENEMIES 100
#define MAX_BULLETS 100
#define MAX_PARTICLES 200
#define MAX_POWERUPS 1


typedef struct {
    Vector2 position;
    bool active;
    Vector2 speed;
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
    Texture2D playerTexture[8];
    float textureFloat = 0.0f;
    Vector2 playerMiddle = {playerPosition.x - playerTexture[0].width / 2, playerPosition.y - playerTexture[0].height / 2};
    playerTexture[0] = LoadTexture("R1.png");
    playerTexture[1] = LoadTexture("R2.png");
    playerTexture[2] = LoadTexture("U1.png");
    playerTexture[3] = LoadTexture("U2.png");
    playerTexture[4] = LoadTexture("L1.png");
    playerTexture[5] = LoadTexture("L2.png");
    playerTexture[6] = LoadTexture("D1.png");
    playerTexture[7] = LoadTexture("D2.png");
    Anim animSprite = Right;

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
    const int powerupSpawnInterval = 10000;

    bool gameOver = false;

    State state = Menu;
    Difficulty diff = Easy;

    Particle particles[MAX_PARTICLES] = { 0 };
    Color bloodColor = { 200, 0, 0, 255 };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        if (!gameOver) {
            playerMiddle = {playerPosition.x - playerTexture[0].width / 2, playerPosition.y - playerTexture[0].height / 2};
            backgroundPosition.y += backgroundScrollSpeed;
            if (backgroundPosition.y >= screenHeight) {
                backgroundPosition.y = 0;
            }

            speedTimer += deltaTime;
            textureFloat += deltaTime;
            {
                if(textureFloat > 1)
                {
                    textureFloat = 0;
                }
            }
            if(speedTimer >= enemySpeedIncrease)
            {
                enemySpeed += 0.5;
                speedTimer = 0.0f;
            }

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            {
                animSprite = Right;
                playerPosition.x += 5;
            } 
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))           
            {
                animSprite = Left;
                playerPosition.x -= 5;
            } 
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            {
                animSprite = Down;
                playerPosition.y += 5;
            }
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            {
                animSprite = Up;
                playerPosition.y -= 5;
            }

                Vector2 mousePosition = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].active = true;
                        bullets[i].position = playerPosition;
                        bullets[i].speed = Vector2Normalize(Vector2Subtract(mousePosition, bullets[i].position));
                        bullets[i].speed = Vector2Scale(bullets[i].speed, bulletSpeed);
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    bullets[i].position = Vector2Add(bullets[i].position, bullets[i].speed);
                    if (bullets[i].position.x < 0 || bullets[i].position.x > screenWidth || bullets[i].position.y < 0 || bullets[i].position.y > screenHeight) {
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
            
            switch (diff)
            {
                case Easy:
                    if (GetRandomValue(0, 100) < 5) {
                        for (int i = 0; i < MAX_ENEMIES; i++) {
                            if (!enemies[i].active) {
                                enemies[i].position.x = GetRandomValue(0, screenWidth);
                                enemies[i].position.y = GetRandomValue(0, screenHeight);
                                //enemies[i].position.y = screenHeight + 10;
                                enemies[i].active = true;
                                break;
                            }
                        }
                    }
                    break;

                case Medium:
                    if (GetRandomValue(0, 100) < 7) {
                        for (int i = 0; i < MAX_ENEMIES; i++) {
                            if (!enemies[i].active) {
                                enemies[i].position.x = GetRandomValue(0, screenWidth);
                                enemies[i].position.y = screenHeight + 10;
                                enemies[i].active = true;
                                break;
                            }
                        }
                    }
                    break;

                case Hard:
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
                    break;
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
                    //enemies[i].position.y -= enemySpeed;

                    Vector2 direction = Vector2Normalize(Vector2Subtract(playerPosition, enemies[i].position));
                    enemies[i].position = Vector2Add(enemies[i].position, Vector2Scale(direction, enemySpeed));
                    if (CheckCollisionCircles(playerPosition, playerRadius, enemies[i].position, 12)) {
                        DecreasePlayerHealth(&playerHealth, 10);
                        enemies[i].active = false;
                    }

                    if (enemies[i].position.y < -10) {
                        enemies[i].active = false;
                    }
                }
            }

            if (GetPlayerHealth(&playerHealth) <= 0) {
                state = Over;
                gameOver = true;
            }
        } else {
            if (IsKeyPressed(KEY_R)) {
                InitPlayerHealth(&playerHealth, 100);
                InitScore(&pScore, 0);
                gameOver = false;
                state = Game;
                playerPosition = { screenWidth / 2, screenHeight / 2 };
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(backgroundTexture, backgroundPosition.x, backgroundPosition.y, WHITE);
        DrawTexture(backgroundTexture, backgroundPosition.x, backgroundPosition.y - screenHeight, WHITE);

        switch (state) {
            case Menu:
                DrawText("Press Enter to start", screenWidth / 2 - MeasureText("Press Enter to start", 20) / 2, screenHeight / 2 - 50, 20, BLACK);


                DrawText("Select Difficulty:", screenWidth / 2 - MeasureText("Select Difficulty:", 20) / 2, screenHeight / 2 + 100, 20, BLACK);

                DrawText("Easy (1)", screenWidth / 2 - MeasureText("Easy (1)", 20) / 2, screenHeight / 2 + 150, 20, BLACK);
                DrawText("Medium (2)", screenWidth / 2 - MeasureText("Medium (2)", 20) / 2, screenHeight / 2 + 180, 20, BLACK);
                DrawText("Hard (3)", screenWidth / 2 - MeasureText("Hard ()", 20) / 2, screenHeight / 2 + 210, 20, BLACK);

                if (IsKeyPressed(KEY_ONE)) {
                    diff = Easy;
                } 
                else if (IsKeyPressed(KEY_TWO)) {
                    diff = Medium;
                } 
                else if (IsKeyPressed(KEY_THREE)) {
                    diff = Hard;
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    state = Game;
                    InitPlayerHealth(&playerHealth, 100);
                }
                break;

                case Game:
                    if (!gameOver) {
                        switch (animSprite)
                        {
                            case Right:
                                if(speedTimer <= 0.5)
                                {
                                    DrawTextureEx(playerTexture[0], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                if(speedTimer >= 0.51)
                                {
                                    DrawTextureEx(playerTexture[1], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                break;
                            
                            case Up:
                                if(speedTimer <= 0.5)
                                {
                                    DrawTextureEx(playerTexture[2], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                if(speedTimer >= 0.51)
                                {
                                    DrawTextureEx(playerTexture[3], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                break;
                            
                            case Left:
                                if(speedTimer <= 0.5)
                                {
                                    DrawTextureEx(playerTexture[4], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                if(speedTimer >= 0.51)
                                {
                                    DrawTextureEx(playerTexture[5], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                break;
                                                            
                            case Down:
                                if(speedTimer <= 0.5)
                                {
                                    DrawTextureEx(playerTexture[6], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                if(speedTimer >= 0.51)
                                {
                                    DrawTextureEx(playerTexture[7], playerMiddle, 0.0f, 1.0f,WHITE);
                                }
                                break;
                        }
                        // DrawCircleV(playerPosition, playerRadius, RED);
                        // if(textureFloat <= 0.5)
                        // {
                        //     DrawTexture(playerTexture[0], playerPosition.x - playerTexture[0].width / 2, playerPosition.y - playerTexture[0].height / 2, WHITE);
                        // }
                        // if(speedTimer >= 0.5)
                        // {
                        //     DrawTextureEx(playerTexture[1], playerMiddle, -90.0f, 1.0f,WHITE);
                        // }

                        for (int i = 0; i < MAX_BULLETS; i++) {
                            if (bullets[i].active) {
                                DrawTextureEx(bulletTexture, bullets[i].position, atan2f(bullets[i].speed.y, bullets[i].speed.x) * RAD2DEG, 1.0f, WHITE);
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
                break;

                case Over:
                    DrawText("Game Over", screenWidth / 2 - MeasureText("Game Over", 40) / 2, screenHeight / 2 - 20, 40, RED);
                    DrawText(TextFormat("Final Score: %d",GetPlayerScore(&pScore)), screenWidth / 2 - MeasureText("Final Score: XXX", 20) / 2, screenHeight / 2 + 20, 20, DARKGRAY);
                    DrawText("Press 'R' to restart", screenWidth / 2 - MeasureText("Press 'R' to restart", 20) / 2, screenHeight / 2 + 60, 20, DARKGRAY);
                    enemySpeed = 2; 
                    break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}