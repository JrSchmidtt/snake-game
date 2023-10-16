#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 54, 5, 255};

int cellSize = 30;  // 30 x 30 pixels
int cellCount = 25; // 25 x 25 cells
int screenSize = cellSize * cellCount;

double LastUpdateTime = 0.0;

// Returns the event triggered by the interval.
bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - LastUpdateTime >= interval)
    {
        LastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Food
{
public:
    Vector2 position;
    Texture2D foodTexture;
    Food()
    {
        Image foodImage = LoadImage("assets/food.png");
        foodTexture = LoadTextureFromImage(foodImage);
        UnloadImage(foodImage); // unload image from memory (RAM) after texture was created.
        position = RandomPosition();
    }
    ~Food()
    {
        UnloadTexture(foodTexture);
    }
    void Render()
    {
        DrawTexture(foodTexture, position.x * cellSize, position.y * cellSize, darkGreen);
    }
    Vector2 RandomPosition()
    {
        Vector2 position;
        position.x = GetRandomValue(0, cellCount - 1);
        position.y = GetRandomValue(0, cellCount - 1);
        return position;
    }
};

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = Vector2{1, 0};
    float updateInterval = 0.2f; // interval in seconds between each update of the snake.
    void Render()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x * static_cast<float>(cellSize);
            float y = body[i].y * static_cast<float>(cellSize);
            Rectangle segment = Rectangle{x, y, static_cast<float>(cellSize), static_cast<float>(cellSize)};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);
        }
    }
    void Update()
    {
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }
    void Move()
    {

    }
};

int main()
{
    InitWindow(screenSize, screenSize, "Snake Game");
    SetTargetFPS(60);
    Food food = Food();
    Snake snake = Snake();
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(green);
        food.Render();
        snake.Render();
        if (eventTriggered(snake.updateInterval))
        {
            snake.Update();
        }
        if (IsKeyPressed(KEY_UP) && snake.direction.y != 1)
        {
            snake.direction = Vector2{0, -1};
        }
        if (IsKeyPressed(KEY_DOWN) && snake.direction.y != -1)
        {
            snake.direction = Vector2{0, 1};
        }
        if (IsKeyPressed(KEY_LEFT) && snake.direction.x != 1)
        {
            snake.direction = Vector2{-1, 0};
        }
        if (IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1)
        {
            snake.direction = Vector2{1, 0};
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}