#include <raylib.h>
#include <iostream>
#include <string>

struct Player
{
    Vector2 position{400.0f, 400.0f};

    float radius = 20.0f;
    float hitboxRadius = 18.0f;
    float speed = 180.0f;
};

struct Wall
{
    Rectangle hitbox{
        50.0f,
        50.0f,
        100.0f,
        300.0f
    };
};

class Popup
{
private:
    std::string message;

    Rectangle rectangle{
        200.0f,
        250.0f,
        400.0f,
        200.0f
    };

    bool visible = false;

public:
    explicit Popup(const std::string& message)
        : message(message)
    {
    }

    void Show()
    {
        visible = true;
    }

    void Hide()
    {
        visible = false;
    }

    bool IsVisible() const
    {
        return visible;
    }

    void Draw() const
    {
        if (!visible)
        {
            return;
        }

        // Затемнение игры под окном
        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Fade(BLACK, 0.5f)
        );

        DrawRectangleRec(rectangle, RAYWHITE);

        DrawRectangleLinesEx(
            rectangle,
            3.0f,
            DARKGRAY
        );

        constexpr int fontSize = 40;

        int textWidth = MeasureText(
            message.c_str(),
            fontSize
        );

        int textX = static_cast<int>(
            rectangle.x +
            (rectangle.width - textWidth) / 2.0f
        );

        int textY = static_cast<int>(
            rectangle.y +
            (rectangle.height - fontSize) / 2.0f
        );

        DrawText(
            message.c_str(),
            textX,
            textY,
            fontSize,
            BLACK
        );

        DrawText(
            "Press ENTER to close",
            static_cast<int>(rectangle.x + 95.0f),
            static_cast<int>(
                rectangle.y + rectangle.height - 45.0f
            ),
            20,
            GRAY
        );
    }
};

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "My Game");
    SetTargetFPS(60);

    Player player;
    Wall wall;

    Vector2 coinPosition{
        static_cast<float>(GetRandomValue(100, 700)),
        static_cast<float>(GetRandomValue(100, 700))
    };

    float coinRadius = 10.0f;
    int coins = 0;
    int targetCoins = 10;

    Popup pop("You win!");

    while (!WindowShouldClose())
    {
        float movement = player.speed * GetFrameTime();

        float oldX = player.position.x;

        if (IsKeyDown(KEY_RIGHT))
        {
            player.position.x += movement;
        }

        if (IsKeyDown(KEY_LEFT))
        {
            player.position.x -= movement;
        }

        if (CheckCollisionCircleRec(
                player.position,
                player.hitboxRadius,
                wall.hitbox))
        {
            player.position.x = oldX;
        }

        float oldY = player.position.y;

        if (IsKeyDown(KEY_DOWN))
        {
            player.position.y += movement;
        }

        if (IsKeyDown(KEY_UP))
        {
            player.position.y -= movement;
        }

        if (CheckCollisionCircleRec(
                player.position,
                player.hitboxRadius,
                wall.hitbox))
        {
            player.position.y = oldY;
        }

        if (player.position.x - player.hitboxRadius < 0.0f)
        {
            player.position.x = player.hitboxRadius;
        }

        if (player.position.x + player.hitboxRadius > screenWidth)
        {
            player.position.x =
                screenWidth - player.hitboxRadius;
        }

        if (player.position.y - player.hitboxRadius < 0.0f)
        {
            player.position.y = player.hitboxRadius;
        }

        if (player.position.y + player.hitboxRadius > screenHeight)
        {
            player.position.y =
                screenHeight - player.hitboxRadius;
        }

        bool touchedCoin = CheckCollisionCircles(
            player.position,
            player.hitboxRadius,
            coinPosition,
            coinRadius
        );

        if (touchedCoin)
        {
            coins++;

            if (coins >= targetCoins)
            {
                pop.Show();
            }

            std::cout
                << "[DEBUG] Coins: "
                << coins
                << '\n';
            do
            {
                coinPosition.x = static_cast<float>(
                    GetRandomValue(100, 700)
                );

                coinPosition.y = static_cast<float>(
                    GetRandomValue(100, 700)
                );
            }
            while (
                CheckCollisionCircleRec(
                    coinPosition,
                    coinRadius,
                    wall.hitbox
                )
                ||
                CheckCollisionCircles(
                    coinPosition,
                    coinRadius,
                    player.position,
                    player.hitboxRadius
                )
            );
        }
        if (pop.IsVisible() && IsKeyPressed(KEY_ENTER))
        {
            pop.Hide();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(wall.hitbox, GRAY);

        DrawCircleV(
            coinPosition,
            coinRadius,
            YELLOW
        );

        DrawCircleV(
            player.position,
            player.radius,
            BLACK
        );

        DrawText(
            TextFormat("Coins: %d", coins),
            330,
            10,
            30,
            BLACK
        );

        DrawText(
            TextFormat("You need to collect %d coins", targetCoins),
            225,
            40,
            25,
            BLACK
        );

        pop.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}