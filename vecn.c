#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include "util.c"

#define vec(n) struct { float f[n]; }
#define vecn(x) ((int)(sizeof(x) / sizeof(float)))
#define vecn2(name, ...) vec(countargs(__VA_ARGS__)) name = {0}; vset(name, __VA_ARGS__)
#define vfrom(dest, src) vec(vecn(src)) dest = {0}; vcpy(dest, src)

#define vcpy(a, b) \
vcpy_(vecn(a), (a).f, vecn(b), (b).f)
#define vadd(a, b) \
vadd_(vecn(a), (a).f, vecn(b), (b).f)
#define vsub(a, b) \
vsub_(vecn(a), (a).f, vecn(b), (b).f)
#define vscale(a, b) \
vscale_(vecn(a), (a).f, (b))
#define vlen2(a) \
vlen2_(vecn(a), (a).f)
#define vlen(a) \
vlen_(vecn(a), (a).f)
#define vnorm(a) \
vnorm_(vecn(a), (a).f)
#define vdist2(a, b) \
vdist2_(vecn(a), (a).f, vecn(b), (b).f)
#define vdist(a, b) \
vdist_(vecn(a), (a).f, vecn(b), (b).f)
#define vdot(a, b) \
vdot_(vecn(a), (a).f, vecn(b), (b).f)
#define vdir2(a, b) \
vdir2_(vecn(a), (a).f, vecn(b), (b).f)
#define vlerp(a, b, t) \
vlerp_(vecn(a), (a).f, vecn(b), (b).f, (t))
#define vdebug(a) \
vdebug_(#a, vecn(a), (a).f)

#define vset(x, ...) \
vset_(vecn(x), (x).f, countargs(__VA_ARGS__), __VA_ARGS__)

void vset_va(int xn, float *x, int va_count, va_list va)
{
    xn = xn > va_count ? va_count : xn;
    for (int i = 0; i < xn; i++, x++) {
        float v = (float) va_arg(va, double);
        *x = v;
    }
}

void vset_(int xn, float *x, int va_count, ...)
{
    va_list va;
    va_start(va, va_count);
    vset_va(xn, x, va_count, va);
    va_end(va);
}

void vcpy_(int an, float *a, int bn, float *b)
{
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        *a = *b;
}

void vadd_(int an, float *a, int bn, float *b)
{
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        *a += *b;
}

void vsub_(int an, float *a, int bn, float *b)
{
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        *a -= *b;
}

void vscale_(int an, float *a, float b)
{
    for (int i = 0; i < an; i++, a++)
        *a *= b;
}

float vlen2_(int an, float *a)
{
    float r = 0;
    for (int i = 0; i < an; i++, a++)
        r += sqr(*a);
    return r;
}

float vlen_(int an, float *a)
{
    float r = vlen2_(an, a);
    r = sqrtf(r);
    return r;
}

void vnorm_(int an, float *a)
{
    float len = vlen_(an, a);
    
    if (len > 0) {
        float ilen = 1 / len;
        for (int i = 0; i < an; i++, a++)
            *a *= ilen;
    }
}

float vdist2_(int an, float *a, int bn, float *b)
{
    float r = 0;
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        r += sqr(*b - *a);
    
    return r;
}

float vdist_(int an, float *a, int bn, float *b)
{
    float r = vdist2_(an, a, bn, b);
    r = sqrtf(r);
    return r;
}

float vdot_(int an, float *a, int bn, float *b)
{
    float r = 0;
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        r += *a * *b;
    return r;
}

void vdir2_(int an, float *a, int bn, float *b)
{
    float *src = a;
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        *a = *b - *a;
    vnorm_(an, src);
}

void vlerp_(int an, float *a, int bn, float *b, float t)
{
    int n = min(an, bn);
    for (int i = 0; i < n; i++, a++, b++)
        *a = (1.0f - t) * *a + t * *b;
}

void vdebug_(const char *name, int an, float *a)
{
    fprintf(stderr, "%s -> (", name);
    for (int i = 0; i < an; i++, a++)
        fprintf(stderr, "%f%s", *a, i + 1 == an ? "" : ", ");
    fprintf(stderr, ")\n");
}

#ifdef run_vecn
#include <stdio.h>

struct entity {
    vec(3) position;
    vec(2) acceleration;
};

int main() {
    fprintf(stderr, "\n\nrunning tests from vecn.c\n\n");
    
    struct entity monster = {0};
    
    vset(monster.position, 3.0f, 2.0f);
    fprintf(stderr, "initial monster position\n");
    vdebug(monster.position);
    
    // or...
    // monster.position.f[0] = 3.0f;
    // monster.position.f[1] = 2.0f;
    
    monster.acceleration.f[0] = 1.0f;
    monster.acceleration.f[1] = 2.0f;
    fprintf(stderr, "\ninitial monster acceleration\n");
    vdebug(monster.acceleration);
    
    // vec(2) wind;
    // vset(wind, 2.0f, 3.0f);
    // or...
    vecn2(wind, 2.0f, 3.0f);
    // monster.acceleration += wind
    vadd(monster.acceleration, wind);
    fprintf(stderr, "\nacceleration affected by wind\n");
    vdebug(monster.acceleration);
    
    // monster.position += monster.acceleration
    vadd(monster.position, monster.acceleration);
    fprintf(stderr, "\nmonster position updated by acceleration\n");
    vdebug(monster.position);
    
    fprintf(stderr, "\ncreate a copy from a vec\n");
    vfrom(monster_copied_position, monster.position);
    vdebug(monster_copied_position);
    
    fprintf(stderr, "all done!\n");
    
    return 0;
}
#endif
