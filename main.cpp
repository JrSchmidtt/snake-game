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

// Check if two vectors are equal. Returns true if they are equal, false otherwise.
bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(element, deque[i]))
        {
            return true;
        }
    }
    return false;
}

class Food
{
public:
    Vector2 position;
    Texture2D foodTexture;
    Food(deque<Vector2> snakeBody)
    {
        Image foodImage = LoadImage("assets/food.png");
        foodTexture = LoadTextureFromImage(foodImage);
        UnloadImage(foodImage); // unload image from memory (RAM) after texture was created.
        position = RandomPosition(snakeBody);
    }
    ~Food()
    {
        UnloadTexture(foodTexture);
    }
    void Render()
    {
        DrawTexture(foodTexture, position.x * cellSize, position.y * cellSize, darkGreen);
    }
    Vector2 RandomCell()
    {
        Vector2 position;
        position.x = GetRandomValue(0, cellCount - 1);
        position.y = GetRandomValue(0, cellCount - 1);
        return position;
    }
    Vector2 RandomPosition(deque<Vector2> snakeBody)
    {
        Vector2 position = RandomCell();
        while (ElementInDeque(position, snakeBody))
        {
            position = RandomCell();
        }
        return position;
    }
};

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = Vector2{1, 0};
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
};

class Game
{
public:
    float updateInterval = 0.2f; // interval in seconds between each update of the snake.
    Snake snake = Snake();
    Food food = Food(snake.body);
    void Render()
    {
        food.Render();
        snake.Render();
    }
    void Update()
    {
        if (eventTriggered(updateInterval))
        {
            snake.Update();
            SnakeCollideWithFood();
        }
    }
    void SnakeCollideWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            cout << "Snake aet food!" << endl;
            food.position = food.RandomPosition(snake.body);
        }
    }
};

int main()
{
    InitWindow(screenSize, screenSize, "Snake Game");
    SetTargetFPS(60);
    Game game = Game();
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(green);
        game.Render();
        game.Update();
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = Vector2{0, -1};
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = Vector2{0, 1};
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = Vector2{-1, 0};
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = Vector2{1, 0};
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}