#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int perlin_init(int *p, float *gx, float *gy, int size) {
    int i;
    for (i=0; i<size; i++)
        p[i] = i;

    /* Randomly swap values */
    for (i=0; i<size; i++) {
        int j = rand() % size;
        int tmp = p[i];
        p[i] = p[j];
        p[j] = tmp;
    }

    /* Precompute table of gradients */
    for (i=0; i<size; i++) {
        gx[i] = rand()/(float)RAND_MAX;
        gy[i] = rand()/(float)RAND_MAX;
    }
    return 0;
}

static inline int perlin_grab_q(int *p, int x, int y, int size) {
    return p[(y + p[x&0xff]) & 0xff];
}

float perlin_get(int *p, float *gx, float *gy, int size, float x, float y) {
    int qx0 = (int)floorf(x);
    int qx1 = qx0 + 1;
    int qy0 = (int)floorf(y);
    int qy1 = qy0 + 1;

    /* Grab the precomputed, random values from the integer plane
     * surrounding this point.
     */
    int q00 = perlin_grab_q(p, qx0, qy0, size);
    int q01 = perlin_grab_q(p, qx1, qy0, size);
    int q10 = perlin_grab_q(p, qx0, qy1, size);
    int q11 = perlin_grab_q(p, qx1, qy1, size);

    float tx0 = x - floorf(x);
    float tx1 = tx0 - 1;
    float ty0 = y - floorf(y);
    float ty1 = ty0 - 1;

    float v00 = gx[q00]*tx0 + gy[q00]*ty0;
    float v01 = gx[q01]*tx1 + gy[q01]*ty0;
    float v10 = gx[q10]*tx0 + gy[q10]*ty1;
    float v11 = gx[q11]*tx1 + gy[q11]*ty1;

    float wx = (3 - 2*tx0)*tx0*tx0;
    float v0 = v00 - wx*(v00 - v01);
    float v1 = v10 - wx*(v10 - v11);
    float wy = (3 - 2*ty0)*ty0*ty0;
    float v = v0 - wy*(v0-v1);

    return v;
}
