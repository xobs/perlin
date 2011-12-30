#include <SDL.h>
#include <math.h>
#include <stdio.h>

#define PERLIN_TIME 200

struct perlin_state {
    SDL_Surface     *screen;
    SDL_TimerID      timer;
    int              should_quit;
    int              tx, ty;
};

int perlin_init(int *p, float *gx, float *gy, int size, unsigned seed);
float perlin_get(int *p, float *gx, float *gy, int size, float x, float y);

int redraw_noise(struct perlin_state *state) {
    SDL_Surface *screen = state->screen;
    int tx = state->tx;
    int ty = state->ty;


    int x, y;
    int *pixels = (int *)screen->pixels;
    float min = 0, max = 0;

    static int initted = 0;
    static float gx[256];
    static float gy[256];
    static int p[256];

    if (!initted) {
        perlin_init(p, gx, gy, 256, 1);
        initted = 1;
    }

    for(y=0; y<screen->h; y++) {
        for(x=0; x<screen->w; x++) {
            float x_field = (x+(tx/2.0))/128.0;
            float y_field = (y+(ty/2.0))/128.0;
            float f = perlin_get(p, gx, gy, 256, x_field, y_field);
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


static Uint32
step_plasma(Uint32 interval, void *state)
{
    SDL_Event e;

    ((struct perlin_state *)state)->tx++;

    /* Issue a redraw */
    e.type = SDL_USEREVENT;
    e.user.code = 1;
    SDL_PushEvent(&e);
    return interval;
}


int main(int argc, char **argv) {
    SDL_Event e;
    struct perlin_state state;

    state.tx = state.ty = 0;
    state.should_quit = 0;

    SDL_Init(SDL_INIT_EVERYTHING);
    state.screen = SDL_SetVideoMode(1280, 720, 32, 0);


    redraw_noise(&state);
    state.timer = SDL_AddTimer(PERLIN_TIME, step_plasma, &state);

    while (!state.should_quit) {
        SDL_WaitEvent(&e);
        switch(e.type) {

            case SDL_QUIT:
                state.should_quit = 1;
                break;

            case SDL_KEYDOWN: {
                SDL_KeyboardEvent *key = (SDL_KeyboardEvent *)&e;

                switch (key->keysym.sym) {
                    case SDLK_ESCAPE:
                        state.should_quit = 1;
                        SDL_Quit();
                        break;
                    case SDLK_LEFT:
                        state.tx--;
                        redraw_noise(&state);
                        break;
                    case SDLK_RIGHT:
                        state.tx++;
                        redraw_noise(&state);
                        break;
                    case SDLK_UP:
                        state.ty--;
                        redraw_noise(&state);
                        break;
                    case SDLK_DOWN:
                        state.ty++;
                        redraw_noise(&state);
                        break;
                    default:
                        break;
                }
                break;
            }

            case SDL_USEREVENT:
                redraw_noise(&state);
                break;

            default:
                break;
        }
    }

    return 0;
}
