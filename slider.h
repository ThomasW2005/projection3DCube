#include <SDL.h>

double map(double x, double in_min, double in_max, double out_min, double out_max);

class Slider
{
public:
    enum state
    {
        PASSIVE,
        HOVER,
        SLIDING
    };
    state currentState = PASSIVE;
    SDL_Rect rect;
    SDL_Rect rectShowing;
    SDL_Renderer *renderer;
    SDL_Color color = {255, 255, 255, 255};
    SDL_Color hoverColor = {255, 255, 255, 255};
    SDL_Color slideColor = {255, 255, 255, 255};
    SDL_Color staticSlideColor = {255, 255, 255, 255};
    SDL_Color slideClickColor = {255, 255, 255, 255};
    double value, vs, ve;
    Slider(int xPos, int yPos, int height, int width, double valStart, double valEnd, SDL_Renderer *sdlRenderer, SDL_Color sdlColor, SDL_Color sdlHoverColor, SDL_Color sdlSlideColor, SDL_Color sdlSlideStaticColor, SDL_Color sdlSlideClickColor);
    void show();
    void tick(SDL_Event *event);
    void setValue(double value);
};