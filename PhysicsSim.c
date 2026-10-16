#include "raylib.h"
#include <stdio.h>
#include "math.h"

#define WIDTH 1200
#define HEIGHT 700
#define PIXELS_PER_METER 100.0f
#define GRAVITY (9.8f * PIXELS_PER_METER)
#define FRAMES 60.0f
#define TIME_STEP (1.0f / FRAMES)
#define MAX_SYSTEMS 10
#define MASS_CONSTANT 5
#define RADIUS_CONSTANT 3

int systems_in_use = 3;

typedef struct {
    Vector2 position, velocity;
    float mass, radius;
} Ball;

typedef struct {
    Vector2 anchor;
    float restLength, stiffness, damping;
} Spring;

typedef struct {
    Ball ball;
    Spring spring;
} SpringSystem;

SpringSystem systems[MAX_SYSTEMS];

void draw_ball(SpringSystem *sys) {
    DrawCircleV(sys->ball.position, sys->ball.radius, WHITE);
    DrawLineV(sys->spring.anchor, sys->ball.position, WHITE);
}

void update_ball(SpringSystem *sys) {

    float stretch = (sys->ball.position.y - sys->spring.anchor.y) - sys->spring.restLength;
    float tension = -sys->spring.stiffness * stretch;
    float dampingForce = -sys->spring.damping * sys->ball.velocity.y;
    float springAccel = (tension + dampingForce) / sys->ball.mass;
    float totalAccel = GRAVITY + springAccel;

    sys->ball.velocity.y = sys->ball.velocity.y + totalAccel * TIME_STEP;
    sys->ball.position.x = sys->ball.position.x + sys->ball.velocity.x * TIME_STEP;
    sys->ball.position.y = sys->ball.position.y + sys->ball.velocity.y * TIME_STEP;

}

int main() {

    InitWindow(WIDTH, HEIGHT, "Physics Sim");
    SetTargetFPS(FRAMES);

    float sectionWidth = WIDTH / (float)systems_in_use;

    for (int i = 0; i < systems_in_use; i++) {
        float anchorX = i * sectionWidth + (sectionWidth / 2);
        systems[i] = (SpringSystem) {
            .ball = {
                .mass = (float) (i+1) * MASS_CONSTANT,
                .radius = (float) (i+1) * RADIUS_CONSTANT,
                .velocity = { .x = 0.0f, .y = 0.0f }
            },
            .spring = {
                .anchor = {.x = anchorX, .y = HEIGHT / 8},
                .restLength = 100.0f,
                .stiffness = 50.0f,
                .damping = 2.0f
            }
        };
        systems[i].ball.position = (Vector2) { .x = anchorX, .y = systems[i].spring.anchor.y + systems[i].spring.restLength};
    }

    while(!WindowShouldClose()) //runs once per frame until window is closed
        {
            for (int i = 0; i < systems_in_use; i++)
            {
                update_ball(&systems[i]);
            }
            BeginDrawing();
            ClearBackground(BLACK);
            for (int i = 0; i < systems_in_use; i++)
            {
                draw_ball(&systems[i]);
            }
            EndDrawing();
        }
}
