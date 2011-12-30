#include <SDL.h>
#include <math.h>
#include <stdio.h>

int perlin_init(int *p, float *gx, float *gy, int size);
float perlin_get(int *p, float *gx, float *gy, int size, float x, float y);

int redraw_noise(SDL_Surface *screen, int tx, int ty) {
    int p[256];
    float gx[256];
    float gy[256];
    int x, y;
    int *pixels = (int *)screen->pixels;
    float min = 0, max = 0;

    perlin_init(p, gx, gy, 256);
    for(y=0; y<screen->h; y++) {
        for(x=0; x<screen->w; x++) {
            float f = perlin_get(p, gx, gy, 256, (x+tx/65536.)/128.,
                    (y+ty/65536.)/128.);
            int px;
            f = sinf(f*64)*128+128;
            if (f < min)
                min = f;
            if (f > max)
                max = f;
            px = SDL_MapRGB(screen->format, f, f, f);
            pixels[y*screen->w+x] = px;
        }
    }
    //fprintf(stderr, "min: %f  max: %f\n", min, max);
    SDL_Flip(screen);
    //fprintf(stderr, "Done\n");
    return 0;
}

int main(int argc, char **argv) {
    SDL_Surface *screen;
    SDL_Event e;
    int should_quit = 0;
    int tx=0, ty=0;

    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(1280, 720, 32, 0);


    redraw_noise(screen, tx, ty);

    while (!should_quit) {
        SDL_WaitEvent(&e);
        switch(e.type) {
            case SDL_QUIT:
                should_quit = 1;
                break;
            case SDL_KEYDOWN: {
                SDL_KeyboardEvent *key = (SDL_KeyboardEvent *)&e;

                switch (key->keysym.sym) {
                    case SDLK_ESCAPE:
                        should_quit = 1;
                        SDL_Quit();
                        break;
                    case SDLK_LEFT:
                        tx--;
                        redraw_noise(screen, tx, ty);
                        break;
                    case SDLK_RIGHT:
                        tx++;
                        redraw_noise(screen, tx, ty);
                        break;
                    case SDLK_UP:
                        ty--;
                        redraw_noise(screen, tx, ty);
                        break;
                    case SDLK_DOWN:
                        ty++;
                        redraw_noise(screen, tx, ty);
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    return 0;
}
