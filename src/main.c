#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"

#define TARGET_FPS 100
#define W 320
#define H 200
#define DROP_SIZE 100

Color cpu_data[W * H];
Vector2 drop_velocity = {1, 1};
Vector2 drop = {0};

inline double dist(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void update_drop()
{
    double drop_half = DROP_SIZE / 2.0;

    drop.x += drop_velocity.x;
    drop.y += drop_velocity.y;

    if (drop.x + drop_half > W - 1) {
        drop.x = W - 1 - drop_half;
        drop_velocity.x *= -1;
    }
    if (drop.x - drop_half < 0) {
        drop.x = drop_half;
        drop_velocity.x *= -1;
    }
    if (drop.y + drop_half > H - 1) {
        drop.y = H - drop_half;
        drop_velocity.y *= -1;
    }
    if (drop.y - drop_half < 0) {
        drop.y = drop_half;
        drop_velocity.y *= -1;
    }
}

void draw(double time, int num_dists)
{
    double value = 0;
    double t = time * 10.0;

    for(int y = 0; y < H; y++) {
        for(int x = 0; x < W; x++) {

            // plasma pattern
            value = 0;
            double d1 = dist(x + t, y, W, H) / 17.0;
            double d2 = dist(x, y + t * 2.0, W / 2.0, H / 2.0) / 14.0;
            double d3 = dist(x, y + t * 1.0, W * 2, H * 2) / 13.0;
            double d4 = dist(x + t, y, 0, 0) / 12.0;
            value += sin(d1) + sin(d2) + sin(d3) + sin(d4);

            // droplet
            double drop_dist = dist(x, y, drop.x, drop.y);
            if (drop_dist < DROP_SIZE / 2.0) {
                d1 = dist(x, y, W, H) / 27.0;
                d2 = dist(x, y, W / 2.0, H / 2.0) / 24.0;
                d3 = dist(x, y, W * 2, H * 2) / 23.0;
                d4 = dist(x, y, 0, 0) / 22.0;
                d1 *= drop_dist / 70.0 + cos(drop_dist / 100.0) + sin(drop_dist / 100.0);
                d2 *= drop_dist / 70.0 + cos(drop_dist / 100.0) + sin(drop_dist / 100.0);
                d3 *= drop_dist / 70.0 + cos(drop_dist / 100.0) + sin(drop_dist / 100.0);
                d4 *= drop_dist / 70.0 + cos(drop_dist / 100.0) + sin(drop_dist / 100.0);
                value += sin(d1) + sin(d2) + sin(d3) + sin(d4);
            }
            int color = (32 + value) * 32;//(int)((8 + value)) * 32;
            int r = 100 + color + cos(t / 10.0) * 20;
            int g = 10 + color;
            int b = 10 + 255 - (color * sin(t / 400.0));
            int a = 255;
            cpu_data[y * W + x] = (Color) { r, g, b, a };
        }
    }
}

int main(int argc, char * argv[])
{
    double time;
    unsigned num_dists = 3;

    InitWindow(W, H, "software plasma");

    Image img = {
        .width = W,
        .height = H,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };
    img.data = (Color *)malloc(W * H * sizeof(Color));
    memcpy(img.data, cpu_data, (W * H * sizeof(Color)));
    Texture2D gpu_data = LoadTextureFromImage(img);
    UnloadImage(img);

    SetTargetFPS(TARGET_FPS);
    DisableCursor();
    ToggleBorderlessWindowed();
    while (!WindowShouldClose()) {

        time = GetTime();

        if (IsKeyDown(KEY_F)) {
            WaitTime(.2);
            if (IsWindowFullscreen()) {
                RestoreWindow();
            } else {
                ToggleBorderlessWindowed();
            }
        }

        update_drop();
        draw(time, num_dists);
        BeginDrawing();
            UpdateTexture(gpu_data, cpu_data);
            Rectangle source = {0, 0, W, H};
            Rectangle dest = {0, 0, GetRenderWidth(), GetRenderHeight()};
            Vector2 origin = {0, 0};
            DrawTexturePro(gpu_data, source, dest, origin, 0.0, WHITE);
            DrawFPS(10, 10);
        EndDrawing();

    }
    EnableCursor();

    CloseWindow();
    return 0;
}
