#ifndef MAIN_H
#define MAIN_H

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720

#define RESOLUTION_X_F 1280.0
#define RESOLUTION_Y_F 720.0

#define NB_ENEMY 14

typedef struct fpRect
{
    float x, y;
    float w, h;
} fpRect;

typedef struct gameObject
{
    SDL_Rect rect;
    fpRect fp;
    SDL_Color color;
    int direction;
    int active;
    int speed;
} gameObject;

#endif // MAIN_H
