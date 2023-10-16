#include <iostream>
#include <raylib.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 54, 5, 255};

int cellSize = 30;  // 30 x 30 pixels
int cellCount = 25; // 25 x 25 cells
int screenSize = cellSize * cellCount;

class Food
{
public:
    Vector2 position;
    Texture2D foodTexture;
    // constructor (called when object is created)
    Food()
    {
        Image foodImage = LoadImage("assets/food.png");
        foodTexture = LoadTextureFromImage(foodImage);
        UnloadImage(foodImage); // unload image from memory (RAM) after texture was created.
        position = RandomPosition();
    }
    // destructor (called when object is destroyed)
    ~Food()
    {
        UnloadTexture(foodTexture);
    }
    // Render food on screen
    void Render()
    {
        DrawTexture(foodTexture, position.x * cellSize, position.y * cellSize, darkGreen);
    }
    // Generate random position for food in grid.
    Vector2 RandomPosition()
    {
        Vector2 position;
        position.x = GetRandomValue(0, cellCount - 1);
        position.y = GetRandomValue(0, cellCount - 1);
        return position;
    }
};

int main()
{
    InitWindow(screenSize, screenSize, "Snake Game");
    SetTargetFPS(60);
    Food food = Food();
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(green); // set background color
        food.Render();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}