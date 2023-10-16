#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 54, 5, 255};

int cellSize = 30;  // 30 x 30 pixels
int cellCount = 25; // 25 x 25 cells
int offset = 75;    // 75 pixels offset from the top and left of the screen
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
        DrawTexture(foodTexture, offset + position.x * cellSize, offset + position.y * cellSize, darkGreen);
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
    bool AddSegment = false; // Add a new segment to the snake body on next tickUpdate.
    void Render()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x * static_cast<float>(cellSize);
            float y = body[i].y * static_cast<float>(cellSize);
            Rectangle segment = Rectangle{offset + x, offset + y, static_cast<float>(cellSize), static_cast<float>(cellSize)};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);
        }
    }
    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (AddSegment)
        {
            AddSegment = false;
        }
        else
        {
            body.pop_back();
        }
    }
    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = Vector2{1, 0};
    }
};

class Game
{
public:
    float updateInterval = 0.2f; // interval in seconds between each update of the snake.
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    void Render()
    {
        food.Render();
        snake.Render();
    }
    void Update()
    {
        if (eventTriggered(updateInterval) && running)
        {
            snake.Update();
            CheckSnakeCollideWithFood();
            CheckSnakeCollideWithWall();
            CheckSnakeCollideWithSelf();
        }
    }
    void CheckSnakeCollideWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.RandomPosition(snake.body);
            snake.AddSegment = true;
        }
    }
    void CheckSnakeCollideWithWall()
    {
        // Snake collide with wall on x axis.
        if (snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        // Snake collide with wall on y axis.
        if (snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }
    void CheckSnakeCollideWithSelf()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }
    void GameOver()
    {
        snake.Reset();
        food.position = food.RandomPosition(snake.body);
        running = false;
    }
};

int main()
{
    InitWindow(2 * offset + screenSize, 2 * offset + screenSize, "Snake Game");
    SetTargetFPS(60);
    Game game = Game();
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5.0, darkGreen);
        game.Render();
        game.Update();
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = Vector2{0.0f, -1.0f};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = Vector2{0.0f, 1.0f};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = Vector2{-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = Vector2{1, 0};
            game.running = true;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}