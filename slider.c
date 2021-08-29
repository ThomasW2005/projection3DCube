#include "slider.h"
#include <SDL.h>

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Slider::setValue(double valueSet)
{
    value = valueSet;
    rectShowing.w = map(value, vs, ve, 0, rect.w);
}

void Slider::tick(SDL_Event *event)
{
    int xPos, yPos;
    SDL_GetMouseState(&xPos, &yPos);
    static bool isDown;
    if (event->type == SDL_MOUSEBUTTONDOWN)
        isDown = true;
    if (event->type == SDL_MOUSEBUTTONUP)
        isDown = false;

    if (xPos > rect.x && xPos < (rect.x + rect.w) && yPos > rect.y && yPos < (rect.y + rect.h))
    {
        currentState = HOVER;
        if (isDown)
        {
            currentState = SLIDING;
            value = (((((double)xPos - (double)rect.x) / (double)rect.w) * (double)((double)ve - vs))) + vs; // (double)
            rectShowing.w = map(value, vs, ve, 0, rect.w);
        }
    }
    else
        currentState = PASSIVE;
}

void Slider::show()
{
    if (currentState == PASSIVE)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, slideColor.r, slideColor.g, slideColor.b, slideColor.a);
        SDL_RenderFillRect(renderer, &rectShowing);
    }
    else if (currentState == HOVER)
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, staticSlideColor.r, staticSlideColor.g, staticSlideColor.b, staticSlideColor.a);
        SDL_RenderFillRect(renderer, &rectShowing);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, slideClickColor.r, slideClickColor.g, slideClickColor.b, slideColor.a);
        SDL_RenderFillRect(renderer, &rectShowing);
    }
}

Slider::Slider(int xPos, int yPos, int height, int width, double valStart, double valEnd, SDL_Renderer *sdlRenderer, SDL_Color sdlColor, SDL_Color sdlHoverColor, SDL_Color sdlSlideColor, SDL_Color sdlSlideStaticColor, SDL_Color sdlSlideClickColor)
{
    rect.h = height;
    rect.w = width;
    rect.x = xPos;
    rect.y = yPos;
    rectShowing.x = xPos;
    rectShowing.y = yPos;
    rectShowing.h = height;
    rectShowing.w = 0;
    vs = valStart;
    ve = valEnd;
    renderer = sdlRenderer;
    color = sdlColor;
    hoverColor = sdlHoverColor;
    slideColor = sdlSlideColor;
    staticSlideColor = sdlSlideStaticColor;
    slideClickColor = sdlSlideClickColor;
    value = vs;
}