/*
row x column
currentPoint[0][0]    x
currentPoint[0][1]    y
currentPoint[0][2]    z
*/

#include "matrixMath.h"
#include "slider.h"
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <cmath>
#include <string>

int WIDTH = 1280;
int HEIGHT = 720;
double AR = ((double)HEIGHT / WIDTH);
const int textHeight = 20;

void SDL_RenderDrawPointFloat(SDL_Renderer *renderer, double x, double y);
void SDL_RenderDrawLineFloat(SDL_Renderer *renderer, double x, double y, double x1, double y1);
void DrawCircle(SDL_Renderer *renderer, double centreXX, double centreYY, int32_t radius);
void renderText(const char *message, TTF_Font *font, SDL_Renderer *renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void drawFPS(int fps, TTF_Font *font, SDL_Renderer *renderer);

int main(int argc, char *argv[])
{
    double angleX = 0, angleY = 0, angleZ = 0;
    double scale = 1.5f;
    double distance = 2;
    bool quit = false, fullScreen = false;
    bool rotX = false, rotY = false, rotZ = false;
    vec3d rotatepoint = {0, 0, 0};
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    SDL_DisplayMode *beforeMode;
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetWindowTitle(window, "3D rotating cube");
    TTF_Font *consola = TTF_OpenFont("cour.ttf", textHeight);
    TTF_Font *consolaFat = TTF_OpenFont("cour.ttf", textHeight * 3);
    if (!consola)
    {
        (consola) = TTF_OpenFont("C:/Windows/Fonts/cour.ttf", textHeight);
        (consolaFat) = TTF_OpenFont("C:/Windows/Fonts/cour.ttf", textHeight * 3);
        if (!consola)
            SDL_ShowSimpleMessageBox(0, "Missing Fonts", "Looks like there are some fonts missing, add cour.tff", window);
    }
    matrix points = {{-0.25f, -0.25f, 0.25f}, //8 cube vertices
                     {0.25f, -0.25f, 0.25f},
                     {0.25f, 0.25f, 0.25f},
                     {-0.25f, 0.25f, 0.25f},
                     {-0.25f, -0.25f, -0.25f},
                     {0.25f, -0.25f, -0.25f},
                     {0.25f, 0.25f, -0.25f},
                     {-0.25f, 0.25f, -0.25f},
                     {0.0f, 0.0f, 0.0f}

    };
    points = matScale(points, scale);

    Uint32 start_time, frame_time;
    int fps;

    SDL_Color bg_passiv = {200, 200, 200};
    SDL_Color bg_hover = {123, 129, 133};

    // SDL_Color fg_passive = {255, 0, 0};
    // SDL_Color fg_hover = {200, 0, 0};
    // SDL_Color fg_click = {170, 0, 0};
    SDL_Color fg_passive = {0, 255, 0};
    SDL_Color fg_hover = {0, 200, 0};
    SDL_Color fg_click = {0, 170, 0};
    Slider XSlider(400, textHeight * 2.5 + 2, textHeight - 4, 300, 0.0f, 2 * M_PI, renderer, bg_passiv, bg_hover, fg_passive, fg_hover, fg_click);
    Slider YSlider(400, textHeight * 3.5 + 2, textHeight - 4, 300, 0.0f, 2 * M_PI, renderer, bg_passiv, bg_hover, fg_passive, fg_hover, fg_click);
    Slider ZSlider(400, textHeight * 4.5 + 2, textHeight - 4, 300, 0.0f, 2 * M_PI, renderer, bg_passiv, bg_hover, fg_passive, fg_hover, fg_click);
    Slider distanceSlider(400, textHeight * 5.5 + 2, textHeight - 4, 300, 0.5f, 4.0f, renderer, bg_passiv, bg_hover, fg_passive, fg_hover, fg_click);
    distanceSlider.setValue(2.0f);

    while (!quit)
    {
        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                XSlider.tick(&event);
                YSlider.tick(&event);
                ZSlider.tick(&event);
                distanceSlider.tick(&event);
                break;
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    WIDTH = event.window.data1;
                    HEIGHT = event.window.data2;
                    AR = ((double)HEIGHT / WIDTH);
                    break;
                }
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_w:
                    points = matMove(points, 0, -0.01f, 0);
                    rotatepoint.y -= 0.01f;
                    break;
                case SDLK_s:
                    points = matMove(points, 0, 0.01f, 0);
                    rotatepoint.y += 0.01f;
                    break;
                case SDLK_a:
                    points = matMove(points, -0.01f, 0, 0);
                    rotatepoint.x -= 0.01f;
                    break;
                case SDLK_d:
                    points = matMove(points, 0.01f, 0, 0);
                    rotatepoint.x += 0.01f;
                    break;
                case SDLK_UP:
                    points = matMove(points, 0, 0, -0.01f);
                    rotatepoint.z -= 0.01f;
                    break;
                case SDLK_DOWN:
                    points = matMove(points, 0, 0, 0.01f);
                    rotatepoint.z += 0.01f;
                    break;
                case SDLK_LEFT:
                    distance -= 0.1f;
                    break;
                case SDLK_RIGHT:
                    distance += 0.1f;
                    break;
                case SDLK_x:
                    rotX = !rotX;
                    break;
                case SDLK_y:
                    rotY = !rotY;
                    break;
                case SDLK_z:
                    rotZ = !rotZ;
                    break;
                case SDLK_F11:
                    fullScreen = !fullScreen;
                    if (fullScreen)
                    {
                        SDL_DisplayMode *mode;
                        SDL_GetWindowDisplayMode(window, beforeMode);
                        // for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
                        // {

                        //     int should_be_zero = SDL_GetCurrentDisplayMode(i, mode);

                        //     if (should_be_zero != 0)
                        //         // In case of error...
                        //         SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

                        //     else
                        //         // On success, print the current display mode.
                        //         SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, mode->w, mode->h, mode->refresh_rate);
                        // }
                        std::cout << SDL_GetError();
                        SDL_GetCurrentDisplayMode(0, mode);
                        std::cout << SDL_GetError();
                        SDL_SetWindowSize(window, mode->w, mode->h);
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(window, 0);
                        SDL_RenderPresent(renderer);
                        SDL_SetWindowSize(window, beforeMode->w, beforeMode->h);
                    }
                    break;
                }
                break;
            }
        }
        if (rotX)
            angleX += 0.003f;
        else
            angleX = XSlider.value;
        if (rotY)
            angleY += 0.006f;
        else
            angleY = YSlider.value;
        if (rotZ)
            angleZ += 0.009f;
        else
            angleZ = ZSlider.value;
        distance = distanceSlider.value;
        matrix drawPoints;
        for (int i = 0; i < points.size(); i++)
        {
            matrix point{{points[i][0]}, {points[i][1]}, {points[i][2]}}; //get the point ready for matrix multiplication
            point[0][0] -= rotatepoint.x;                                 //  the rotation matrix around an arbitrary point can be expressed as moving the rotation point to the origin, rotating around the origin and moving back to the original position: https://bit.ly/3mJ3Q2v
            point[1][0] -= rotatepoint.y;
            point[2][0] -= rotatepoint.z;
            rotateX(point, angleX);
            rotateY(point, angleY);
            rotateZ(point, angleZ);
            point[0][0] += rotatepoint.x;
            point[1][0] += rotatepoint.y;
            point[2][0] += rotatepoint.z;
            project(point, distance);
            drawPoints.push_back({point[0][0], point[1][0]});
        }
        std::vector<std::string> textLeft = {
            "3D Rotating Cube by thomas weichhart",
            " ",
            "X angle: " + std::to_string(angleX * (180 / M_PI)) + "10\xB0",
            "Y angle: " + std::to_string(angleY * (180 / M_PI)) + "10\xB0",
            "Z angle: " + std::to_string(angleZ * (180 / M_PI)) + "10\xB0",
            "camera distance: " + std::to_string(distance),
            "vertices: " + std::to_string(points.size()),
            "rotation point/center of cube:",
            "    x: " + std::to_string(rotatepoint.x),
            "    y: " + std::to_string(rotatepoint.y),
            "    z: " + std::to_string(rotatepoint.z)

        };
        std::vector<std::string> textRight = {
            "Controls:",
            "W to move cube up",
            "S to move cube down",
            "A to move cube left",
            "D to move cube right",
            "Up Arrow to move cube closer to camera",
            "Down Arrow to move cube away from camera",
            "Left/Right Arrow to change distance",
            "X/Y/Z to toggle between auto rotation and manual rotation using sliders for X/Y/Z axis",
            "F11 to toggle fullscreen"};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        XSlider.show();
        YSlider.show();
        ZSlider.show();
        distanceSlider.show();
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        for (int i = 0; i < 4; i++) //draw cube edges
        {
            SDL_RenderDrawLineFloat(renderer, drawPoints[i][0], drawPoints[i][1], drawPoints[(i + 1) % 4][0], drawPoints[(i + 1) % 4][1]);
            SDL_RenderDrawLineFloat(renderer, drawPoints[i + 4][0], drawPoints[i + 4][1], drawPoints[((i + 1) % 4) + 4][0], drawPoints[((i + 1) % 4) + 4][1]);
            SDL_RenderDrawLineFloat(renderer, drawPoints[i][0], drawPoints[i][1], drawPoints[i + 4][0], drawPoints[i + 4][1]);
        }
        for (int i = 0; i < drawPoints.size(); i++)
            DrawCircle(renderer, drawPoints[i][0], drawPoints[i][1], 5);
        for (int i = 0; i < textLeft.size(); i++)
            renderText(textLeft[i].c_str(), consola, renderer, textHeight / 2, textHeight / 2 + i * textHeight, 0, 255, 0);
        for (int i = 0; i < textRight.size(); i++)
            renderText(textRight[i].c_str(), consola, renderer, textHeight / 2, i * textHeight - textHeight / 2 + (HEIGHT - textHeight * textRight.size()), 0, 255, 0);
        drawFPS(fps, consolaFat, renderer);
        Uint32 timeout = SDL_GetTicks() + (int)((1 / 80.) * 1000);
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
        {
        }

        frame_time = SDL_GetTicks() - start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void drawFPS(int fps, TTF_Font *font, SDL_Renderer *renderer)
{
    SDL_Color alienGreen = {0, 255, 0};
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, ("fps: " + std::to_string(fps)).c_str(), alienGreen);
    SDL_Texture *textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect messageRect = {WIDTH - surfaceMessage->w - textHeight / 2, textHeight / 2, surfaceMessage->w, surfaceMessage->h};
    SDL_RenderCopy(renderer, textureMessage, NULL, &messageRect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(textureMessage);
}

void renderText(const char *message, TTF_Font *font, SDL_Renderer *renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Color alienGreen = {r, g, b};
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, message, alienGreen);
    SDL_Texture *textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect messageRect = {x, y, surfaceMessage->w, surfaceMessage->h};
    SDL_RenderCopy(renderer, textureMessage, NULL, &messageRect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(textureMessage);
}

void SDL_RenderDrawPointFloat(SDL_Renderer *renderer, double x, double y)
{
    SDL_RenderDrawPointF(renderer, map(x * AR, -1.0f, 1.0f, 0, WIDTH), map(y, -1.0f, 1.0f, 0, HEIGHT));
}

void SDL_RenderDrawLineFloat(SDL_Renderer *renderer, double x, double y, double x1, double y1)
{
    SDL_RenderDrawLineF(renderer, map(x * AR, -1.0f, 1.0f, 0, WIDTH), map(y, -1.0f, 1.0f, 0, HEIGHT), map(x1 * AR, -1.0f, 1.0f, 0, WIDTH), map(y1, -1.0f, 1.0f, 0, HEIGHT));
}

void DrawCircle(SDL_Renderer *renderer, double centreXX, double centreYY, int32_t radius)
{
    //https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
    const int32_t diameter = (radius * 2);

    double centreX = map(centreXX * AR, -1.0f, 1.0f, 0, WIDTH); //translate
    double centreY = map(centreYY, -1.0f, 1.0f, 0, HEIGHT);

    double x = (radius - 1);
    double y = 0;
    double tx = 1;
    double ty = 1;
    double error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}