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
    Vector2 position = {5, 6}; // position of food in grid
    void draw()
    {
        DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
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
        food.draw();           // draw food
        EndDrawing();
    }
    CloseWindow();
    return 0;
}