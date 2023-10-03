#include "raylib.h"
int main() {
    // Initialize a window
    int width{900}, height{600};
    InitWindow(width, height, "Chase Game");

    // Circle Coordinates
    int circle_x{450}, circle_y{300}, circle_radius{25};
    // Circle Edges
    int l_circle_x{circle_x - circle_radius};
    int r_circle_x{circle_x + circle_radius};
    int u_circle_y{circle_y - circle_radius};
    int d_circle_y{circle_y + circle_radius};

    // Axe Coordinates
    int axe_x{0}, axe_y{0}, axe_length{50};
    // Axe Edges
    int l_axe_x{axe_x};
    int r_axe_x{axe_x + axe_length};
    int u_axe_y{axe_y};
    int d_axe_y{axe_y + axe_length};

    int direction{10};

    bool collision_with_axe =
        (d_axe_y >= u_circle_y) &&
        (u_axe_y <= d_circle_y) &&
        (l_axe_x <= r_circle_x) &&
        (r_axe_x >= l_circle_x);

    // WindowShouldClose returns true if esc is clicked
    SetTargetFPS(60);                    // Sets the highest FPS
    while(WindowShouldClose() == false) // Keep the Window open
    {
        // Setup Canvas
        BeginDrawing();
        // Clear canvas to a specific color to avoid flicker
        ClearBackground(WHITE);

        if (collision_with_axe) {
            DrawText("Game Over", 400, 200, 20, RED);
            if(IsKeyDown(KEY_R))
            {
                DrawText("Lose", 400, 200, 20, RED);
                axe_x = 0;
                axe_y = 0;
                collision_with_axe = false;
            }

        }
        else {
            // ****Game Logic Begin
            // Update Edges
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            d_circle_y = circle_y + circle_radius;
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            d_axe_y = axe_y + axe_length;

            // Update Collition Condition
            collision_with_axe =
                (d_axe_y >= u_circle_y) &&
                (u_axe_y <= d_circle_y) &&
                (l_axe_x <= r_circle_x) &&
                (r_axe_x >= l_circle_x);

            // Draws a Circle in the Canvas(X, Y, R, C)
            DrawCircle(circle_x, circle_y, circle_radius, BLUE);
            // Draws a Rectangle in the Canvas(X, Y, W, H, C)
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);

            // Circle Movement
            if (IsKeyDown(KEY_D) && circle_x < width - circle_radius)
                circle_x += 10;
            if (IsKeyDown(KEY_A) && circle_x > circle_radius)
                circle_x -= 10;
            if (IsKeyDown(KEY_W) && circle_y > circle_radius)
                circle_y -= 10;
            if (IsKeyDown(KEY_S) && circle_y < height - circle_radius)
                circle_y += 10;

            // Move the Axe
            if (circle_x > axe_x)
                axe_x += 5;
            if (circle_x < axe_x)
                axe_x -= 5;
            if (circle_y < axe_y)
                axe_y -= 5;
            if (circle_y > axe_y)
                axe_y += 5;
        }
        EndDrawing();
    };
}

/*******************************************************************************************
*
*   raylib [textures] example - Background scrolling
*
*   Example originally created with raylib 2.0, last time updated with raylib 2.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

// #include "raylib.h"

// #define MAX_COLORS_COUNT    23          // Number of colors available

// //------------------------------------------------------------------------------------
// // Program main entry point
// //------------------------------------------------------------------------------------
// int main(void)
// {
//     // Initialization
//     //--------------------------------------------------------------------------------------
//     const int screenWidth = 800;
//     const int screenHeight = 450;

//     InitWindow(screenWidth, screenHeight, "raylib [textures] example - mouse painting");

//     // Colors to choose from
//     Color colors[MAX_COLORS_COUNT] = {
//         RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
//         SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
//         LIGHTGRAY, GRAY, DARKGRAY, BLACK };

//     // Define colorsRecs data (for every rectangle)
//     Rectangle colorsRecs[MAX_COLORS_COUNT] = { 0 };

//     for (int i = 0; i < MAX_COLORS_COUNT; i++)
//     {
//         colorsRecs[i].x = 10 + 30.0f*i + 2*i;
//         colorsRecs[i].y = 10;
//         colorsRecs[i].width = 30;
//         colorsRecs[i].height = 30;
//     }

//     int colorSelected = 0;
//     int colorSelectedPrev = colorSelected;
//     int colorMouseHover = 0;
//     float brushSize = 20.0f;
//     bool mouseWasPressed = false;

//     Rectangle btnSaveRec = { 750, 10, 40, 30 };
//     bool btnSaveMouseHover = false;
//     bool showSaveMessage = false;
//     int saveMessageCounter = 0;

//     // Create a RenderTexture2D to use as a canvas
//     RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

//     // Clear render texture before entering the game loop
//     BeginTextureMode(target);
//     ClearBackground(colors[0]);
//     EndTextureMode();

//     SetTargetFPS(120);              // Set our game to run at 120 frames-per-second
//     //--------------------------------------------------------------------------------------

//     // Main game loop
//     while (!WindowShouldClose())    // Detect window close button or ESC key
//     {
//         // Update
//         //----------------------------------------------------------------------------------
//         Vector2 mousePos = GetMousePosition();

//         // Move between colors with keys
//         if (IsKeyPressed(KEY_RIGHT)) colorSelected++;
//         else if (IsKeyPressed(KEY_LEFT)) colorSelected--;

//         if (colorSelected >= MAX_COLORS_COUNT) colorSelected = MAX_COLORS_COUNT - 1;
//         else if (colorSelected < 0) colorSelected = 0;

//         // Choose color with mouse
//         for (int i = 0; i < MAX_COLORS_COUNT; i++)
//         {
//             if (CheckCollisionPointRec(mousePos, colorsRecs[i]))
//             {
//                 colorMouseHover = i;
//                 break;
//             }
//             else colorMouseHover = -1;
//         }

//         if ((colorMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
//         {
//             colorSelected = colorMouseHover;
//             colorSelectedPrev = colorSelected;
//         }

//         // Change brush size
//         brushSize += GetMouseWheelMove()*5;
//         if (brushSize < 2) brushSize = 2;
//         if (brushSize > 50) brushSize = 50;

//         if (IsKeyPressed(KEY_C))
//         {
//             // Clear render texture to clear color
//             BeginTextureMode(target);
//             ClearBackground(colors[0]);
//             EndTextureMode();
//         }

//         if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || (GetGestureDetected() == GESTURE_DRAG))
//         {
//             // Paint circle into render texture
//             // NOTE: To avoid discontinuous circles, we could store
//             // previous-next mouse points and just draw a line using brush size
//             BeginTextureMode(target);
//             if (mousePos.y > 50) DrawCircle((int)mousePos.x, (int)mousePos.y, brushSize, colors[colorSelected]);
//             EndTextureMode();
//         }

//         if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
//         {
//             if (!mouseWasPressed)
//             {
//                 colorSelectedPrev = colorSelected;
//                 colorSelected = 0;
//             }

//             mouseWasPressed = true;

//             // Erase circle from render texture
//             BeginTextureMode(target);
//             if (mousePos.y > 50) DrawCircle((int)mousePos.x, (int)mousePos.y, brushSize, colors[0]);
//             EndTextureMode();
//         }
//         else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && mouseWasPressed)
//         {
//             colorSelected = colorSelectedPrev;
//             mouseWasPressed = false;
//         }

//         // Check mouse hover save button
//         if (CheckCollisionPointRec(mousePos, btnSaveRec)) btnSaveMouseHover = true;
//         else btnSaveMouseHover = false;

//         // Image saving logic
//         // NOTE: Saving painted texture to a default named image
//         if ((btnSaveMouseHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_S))
//         {
//             Image image = LoadImageFromTexture(target.texture);
//             ImageFlipVertical(&image);
//             ExportImage(image, "my_amazing_texture_painting.png");
//             UnloadImage(image);
//             showSaveMessage = true;
//         }

//         if (showSaveMessage)
//         {
//             // On saving, show a full screen message for 2 seconds
//             saveMessageCounter++;
//             if (saveMessageCounter > 240)
//             {
//                 showSaveMessage = false;
//                 saveMessageCounter = 0;
//             }
//         }
//         //----------------------------------------------------------------------------------

//         // Draw
//         //----------------------------------------------------------------------------------
//         BeginDrawing();

//         ClearBackground(RAYWHITE);

//         // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
//         DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

//         // Draw drawing circle for reference
//         if (mousePos.y > 50)
//         {
//             if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) DrawCircleLines((int)mousePos.x, (int)mousePos.y, brushSize, GRAY);
//             else DrawCircle(GetMouseX(), GetMouseY(), brushSize, colors[colorSelected]);
//         }

//         // Draw top panel
//         DrawRectangle(0, 0, GetScreenWidth(), 50, RAYWHITE);
//         DrawLine(0, 50, GetScreenWidth(), 50, LIGHTGRAY);

//         // Draw color selection rectangles
//         for (int i = 0; i < MAX_COLORS_COUNT; i++) DrawRectangleRec(colorsRecs[i], colors[i]);
//         DrawRectangleLines(10, 10, 30, 30, LIGHTGRAY);

//         if (colorMouseHover >= 0) DrawRectangleRec(colorsRecs[colorMouseHover], Fade(WHITE, 0.6f));

//         DrawRectangleLinesEx((Rectangle){ colorsRecs[colorSelected].x - 2, colorsRecs[colorSelected].y - 2,
//                              colorsRecs[colorSelected].width + 4, colorsRecs[colorSelected].height + 4 }, 2, BLACK);

//         // Draw save image button
//         DrawRectangleLinesEx(btnSaveRec, 2, btnSaveMouseHover ? RED : BLACK);
//         DrawText("SAVE!", 755, 20, 10, btnSaveMouseHover ? RED : BLACK);

//         // Draw save image message
//         if (showSaveMessage)
//         {
//             DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
//             DrawRectangle(0, 150, GetScreenWidth(), 80, BLACK);
//             DrawText("IMAGE SAVED:  my_amazing_texture_painting.png", 150, 180, 20, RAYWHITE);
//         }

//         EndDrawing();
//         //----------------------------------------------------------------------------------
//     }

//     // De-Initialization
//     //--------------------------------------------------------------------------------------
//     UnloadRenderTexture(target);    // Unload render texture

//     CloseWindow();                  // Close window and OpenGL context
//     //--------------------------------------------------------------------------------------

//     return 0;
// }

// #include "raylib.h"

// //------------------------------------------------------------------------------------
// // Program main entry point
// //------------------------------------------------------------------------------------
// int main(void)
// {
//     // Initialization
//     //--------------------------------------------------------------------------------------
//     const int screenWidth = 800;
//     const int screenHeight = 450;

//     InitWindow(screenWidth, screenHeight, "raylib [textures] example - background scrolling");

//     // NOTE: Be careful, background width must be equal or bigger than screen width
//     // if not, texture should be draw more than two times for scrolling effect
//     Texture2D background = LoadTexture("my_amazing_texture_painting.png");
//     Texture2D midground = LoadTexture("my_amazing_texture_painting.png");
//     Texture2D foreground = LoadTexture("my_amazing_texture_painting.png");

//     float scrollingBack = 0.0f;
//     float scrollingMid = 0.0f;
//     float scrollingFore = 0.0f;

//     SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
//     //--------------------------------------------------------------------------------------

//     // Main game loop
//     while (!WindowShouldClose())    // Detect window close button or ESC key
//     {
//         // Update
//         //----------------------------------------------------------------------------------
//         scrollingBack -= 0.1f;
//         scrollingMid -= 0.5f;
//         scrollingFore -= 1.0f;

//         // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
//         if (scrollingBack <= -background.width*2) scrollingBack = 0;
//         if (scrollingMid <= -midground.width*2) scrollingMid = 0;
//         if (scrollingFore <= -foreground.width*2) scrollingFore = 0;
//         //----------------------------------------------------------------------------------

//         // Draw
//         //----------------------------------------------------------------------------------
//         BeginDrawing();

//             ClearBackground(GetColor(0x052c46ff));

//             // Draw background image twice
//             // NOTE: Texture is scaled twice its size
//             DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
//             DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);

//             // Draw midground image twice
//             DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
//             DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

//             // Draw foreground image twice
//             DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
//             DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

//             DrawText("BACKGROUND SCROLLING & PARALLAX", 10, 10, 20, RED);
//             DrawText("(c) Cyberpunk Street Environment by Luis Zuno (@ansimuz)", screenWidth - 330, screenHeight - 20, 10, RAYWHITE);

//         EndDrawing();
//         //----------------------------------------------------------------------------------
//     }

//     // De-Initialization
//     //--------------------------------------------------------------------------------------
//     UnloadTexture(background);  // Unload background texture
//     UnloadTexture(midground);   // Unload midground texture
//     UnloadTexture(foreground);  // Unload foreground texture

//     CloseWindow();              // Close window and OpenGL context
//     //--------------------------------------------------------------------------------------

//     return 0;
// }