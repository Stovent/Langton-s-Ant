#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

typedef enum
{
    top,
    right,
    bottom,
    left,
} AntOrientation;

typedef struct
{
    int16_t x;
    int16_t y;
    uint8_t orientation;
} Ant;

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Cannot initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    int16_t width = 200;
    int16_t height = 200;
    SDL_Window* window;
    if((window = SDL_CreateWindow("Langton's ant", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == 0)
    {
        fprintf(stderr, "Cannot create window: %s\n", SDL_GetError());
        return -2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == 0)
    {
        fprintf(stderr, "Cannot create renderer: %s\n", SDL_GetError());
        return -3;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB332);
    SDL_Texture* texture = SDL_CreateTexture(renderer, format->format, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(texture == 0)
    {
        fprintf(stderr, "Cannot create texture: %s\n", SDL_GetError());
        return -4;
    }

    Ant ant = {width/2, height/2, top};

    uint8_t *pixels = malloc(width * height * format->BytesPerPixel);
    if(!pixels)
    {
        fprintf(stderr, "Cannot allocate pixels!\n");
        return -5;
    }
    memset(pixels, 0, width * height * format->BytesPerPixel);

    SDL_Event event;
    for(;;)
    {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;

        if(ant.x < 0 || ant.x > width || ant.y < 0 || ant.y > height)
            continue;

        const int pitch = width * format->BytesPerPixel;

        if(pixels[ant.x * width + ant.y] == 0)
        {
            pixels[ant.x * width + ant.y] = 0xFF;
            switch(ant.orientation)
            {
            case top:
                ant.orientation = left;
                ant.x--;
                break;

            case right:
                ant.orientation = top;
                ant.y--;
                break;

            case bottom:
                ant.orientation = right;
                ant.x++;
                break;

            case left:
                ant.orientation = bottom;
                ant.y++;
                break;

            default:
                fprintf(stderr, "Wrong ant orientation, exiting...\n");
                return -6;
            }
        }
        else
        {
            pixels[ant.x * width + ant.y] = 0;
            switch(ant.orientation)
            {
            case top:
                ant.orientation = right;
                ant.x++;
                break;

            case right:
                ant.orientation = bottom;
                ant.y++;
                break;

            case bottom:
                ant.orientation = left;
                ant.x--;
                break;

            case left:
                ant.orientation = top;
                ant.y--;
                break;

            default:
                fprintf(stderr, "Wrong ant orientation, exiting...\n");
                return -7;
            }
        }

        SDL_UpdateTexture(texture, 0, pixels, pitch);
        if(SDL_RenderCopy(renderer, texture, 0, 0) < 0)
        {
            fprintf(stderr, "Cannot copy render: %s\n", SDL_GetError());
            return -8;
        }

        SDL_RenderPresent(renderer);
    }

    free(pixels);
    SDL_FreeFormat(format);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
