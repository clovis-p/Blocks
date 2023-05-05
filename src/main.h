#ifndef MAIN_H
#define MAIN_H

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720

#define RESOLUTION_X_F 1280.0
#define RESOLUTION_Y_F 720.0

typedef struct fpRect
{
    float x, y;
    float w, h;
} fpRect;

typedef struct collision
{
    int left;
    int up;
    int right;
    int down;
} collision;

typedef struct gameObject
{
    SDL_Rect rect; // Position and size for rendering only
    fpRect fp; // Position and size for everything else
    SDL_Color color;
    int direction;
    int active;
    float speed;
    collision collision;
} gameObject;

#endif // MAIN_H
