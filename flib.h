#ifndef flibh
#define flibh

#include <stdarg.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned int long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed int long i64;

typedef u8 byte;

typedef i64 ssize;
typedef u64 usize;

// length of fixed array.
#define arrl(x) \
    (sizeof(x) / sizeof(*(x)))
// traverse elements in array.
#define each(type, x, arr) \
    for (type *x = (arr); x < (arr) + arrl(arr); x++)
// traverse up to n elements in array.
#define eachn(type, x, arr, n) \
    for (type *x = (arr); x < (arr) + min(arrl(arr), (n)); x++)
// traverse elements in array starting from the end (reverse)
#define eachr(type, x, arr) \
    for (type *x = (arr) + arrl(arr) - 1; x >= (arr); x--)
// traverse n elements in array starting from the end (reverse)
#define eachrn(type, x, arr, n) \
    for (type *x = (arr) + arrl(arr) - 1; x >= (arr) + arrl(arr) - (n); x--)

#define abs(x) \
    ((x) < 0 ? (-(x)) : (x))
#define min(x1, x2) \
    ((x1) < (x2) ? (x1) : (x2))
#define max(x1, x2) \
    ((x1) > (x2) ? (x1) : (x2))
#define clamp(x, a, b) \
    (min(max((x), min((a), (b))), max((a), (b))))
#define lerp(x, _min, _max) \
    (((_max) - (_min)) * (x) + (_min))
#define sqr(x) \
    ((x) * (x))

// coroutine/fiber/green-thread
// example:
// void mycoroutine(struct coroutine *ctx)
// {
//      cstart(ctx) {
//          // on first call, this will be executed.
//          // then exit.
//          yield(1);
//          // on second call, this will be executed.
//          // then exit.
//          yield(2)
//          // third call, this chunk will execute, restarting
//          // the coroutine state to start from the beginning.
//          creset;
//      } cend;
// }
#define cstart(ctx)                         \
    do {                                    \
        struct coroutine *__coro = (ctx);   \
        switch((__coro)->state) {           \
        case 0:

// yield and update the state of the coroutine.
// id must be unique.
// id could be replaced with __LINE__ if you don't
// hot-reload your code. if you do though, you
// can't since any change can update your line
// numbers and produce incorrect results.
#define yield(id)               \
    do {                        \
        (__coro)->state = (id); \
        return;                 \
        case (id):;             \
    } while (0)

// sleep and yield until timeout has past.
// dt = how much time has past (if you are writing
//      a game, this would be the delta time of each
//      frame)
#define syield(id, _timeout, dt)        \
    do {                                \
        (__coro)->timeout = (_timeout); \
        yield(id);                      \
        (__coro)->timeout -= (dt);      \
        if ((__coro)->timeout > 0)      \
            return;                     \
    } while(0)

// reset the coroutine state to start from the beginning.
#define creset                              \
    do {                                    \
        *(__coro) = (struct coroutine){0};  \
    } while (0)

// end coroutine block.
#define cend    \
        }       \
    } while (0)

// str format using fixed array as destination.
#define strf2(dest, format, ...) \
    (strf((dest), sizeof(dest), format, __VA_ARGS__))

#define v2z (v2){0}

// longer aliases.
#define arrlen        arrl
#define foreach       each
#define foreachn      eachn
#define foreachrev    eachr
#define foreachrevn   eachrn
#define strmatches    streq
#define strmatchesn   streqn
#define strlen2       strl
#define strstartswith strsw
#define strendswith   strew
#define memset2       mems
#define memmatches    memeq
#define memcpy2       memc
#define v2zero        v2z

typedef union v2 {
    struct { float x, y; };
    struct { float w, h; };
    float f[2];
} v2;

struct coroutine {
    u32 state;
    float timeout;
};

// fast inverse square root.
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt(float number);

// add x1 and x2.
v2 v2add(v2 x1, v2 x2);
// subtract x1 and x2.
v2 v2sub(v2 x1, v2 x2);
// multiply/scale vector x by n.
v2 v2skl(v2 x, float n);
// get normalized/direction from src to dest.
v2 v2dir2(v2 src, v2 dest);
// normalize vector x.
v2 v2norm(v2 x);
// dot product between x1 and x2.
float v2dot(v2 x1, v2 x2);
// get squared length of x.
float v2len2(v2 x);
// get squared distance from src to dest.
float v2dist2(v2 src, v2 dest);
// check if target is inside the range at src.
int v2inrng(v2 target, v2 src, float r);

// check circle c1 with radius r1 collides with circle c2 with radius r2.
int cchit(v2 c1, v2 c2, float r1, float r2);
// check circle with radius r collides with point p.
int cphit(v2 c, float r, v2 p);
// check rectangle r with width w and height h collides with point p
int rphit(v2 p, v2 r, float w, float h);
// check rectangle r1 collides with rectangle r2.
int rrhit(v2 r1, v2 r2, float w1, float h1, float w2, float h2);

// check if string src starts with match.
// both strings need to be null terminated.
// src and match can be null.
int strsw(char *src, char *match);
// check if string src ends with match.
// both strings need to be null terminated.
// src and match can be null.
int strew(char *src, char *match);
// check if string a and b are equal.
// both strings need to be null terminated.
// a and b can be null.
int streq(char *a, char *b);
// check if the first n characters of a and b are equal.
// both strings need to be null terminated.
// a and b can be null.
int streqn(char *a, char *b, usize n);
// get length of src.
// src must be null terminated.
// src can be null.
usize strl(char *src);
// convert integer to string.
// dest can be null.
// dest will be null terminated.
// the amount of characters written is returned.
usize stri64(char *dest, i64 x, u64 base, usize n);
// convert double to string.
// dest can be null.
// dest will be null terminated.
// the amount of characters written is returned.
usize strdbl(char *dest, double x, usize n);
// write up to n bytes of formatted string into dest.
// dest will be null terminated.
// returns number of bytes used/written (INCLUDING null terminator)
// formats:
// %%  = escapes % and only writes one %.
// %c  = writes character.
// %s  = writes strings up to null terminator.
// %*s = writes n bytes of string. 
//       example: strf(dest, n, "%*s", 2, "lorem")
//       only writes 2 bytes/chars, meaning, "lo"
// %x  = writes hexadecimal.
// %d  = writes int.
// %f  = writes float/double with 2 decimals.
// %v  = writes vector with format "(x:y)"
usize strf(char *dest, usize n, char *format, ...);

// memset, set n bytes from src to dest.
// dest can be null.
void mems(void *dest, byte src, usize n);
// check if n bytes from a and b are the same.
// a and b can be null.
int memeq(void *a, void *b, usize n);
// memcpy, copy n bytes from src to dest.
// dest and src can be null.
void memc(void *dest, void *src, usize n);

// random integer.
// seed can be null in which case, a hard coded
// seed will be used instead.
u32 randi(u32 *seed);
// random integer between lower and upper.
// seed can be null.
u32 randi2(u32 *seed, u32 lower, u32 upper);

#endif

#ifdef flibc

float Q_rsqrt(float number)
{
    union {
        float f;
        unsigned int i;
    } conv = { .f = number };
    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}

v2 v2add(v2 x1, v2 x2)
{
    v2 r = {{x1.f[0] + x2.f[0], x1.f[1] + x2.f[1]}};
    return r;
}

v2 v2sub(v2 x1, v2 x2)
{
    v2 r = {{x1.f[0] - x2.f[0], x1.f[1] - x2.f[1]}};
    return r;
}

v2 v2skl(v2 x, float n)
{
    v2 r = {{x.f[0] * n, x.f[1] * n}};
    return r;
}

v2 v2dir2(v2 src, v2 dest)
{
    v2 r = v2norm(v2sub(dest, src));
    return r;
}

v2 v2norm(v2 x)
{
    float len2 = v2len2(x);
    v2 r = x;
    if (len2 > 0)
        r = v2skl(r, Q_rsqrt(len2));
    return r;
}

float v2dot(v2 x1, v2 x2)
{
    float r = (x1.f[0] * x2.f[0]) + (x1.f[1] * x2.f[1]);
    return r;
}

float v2len2(v2 x)
{
    float r = sqr(x.f[0]) + sqr(x.f[1]);
    return r;
}

float v2dist2(v2 src, v2 dest)
{
    float r = sqr(src.f[0] - dest.f[0]) + sqr(src.f[1] - dest.f[1]);
    return r;
}

int v2inrng(v2 target, v2 src, float range)
{
    int r = v2dist2(src, target) <= sqr(range);
    return r;
}

int v2inrect(v2 target, v2 src, float w, float h)
{
    int r = (
        src.x     <= target.x &&
        src.x + w >  target.x &&
        src.y     <= target.y &&
        src.y + h >  target.y
    );
    return r;
}

int cchit(v2 c1, v2 c2, float r1, float r2)
{
    float d2 = v2dist2(c1, c2);
    int r = sqr(r1 + r2) >= d2;
    return r;
}

int cphit(v2 c, float r, v2 p)
{
    float d2 = v2dist2(c, p);
    int result = d2 < sqr(r);
    return result;
}

int rphit(v2 p, v2 r, float w, float h)
{
    int result = (
        p.x >= min(r.x, r.x + w) && p.x <= max(r.x, r.x + w) &&
        p.y >= min(r.y, r.y + h) && p.y <= max(r.y, r.y + h)
    );
    return result;
}

int rrhit(v2 r1, v2 r2, float w1, float h1, float w2, float h2)
{
    int xhit = (
        max(r1.x, r1.x + w1) >= min(r2.x, r2.x + w2) &&
        min(r1.x, r1.x + w1) <= max(r2.x, r2.x + w2)
    );
    int yhit = (
        max(r1.y, r1.y + h1) >= min(r2.y, r2.y + h2) &&
        min(r1.y, r1.y + h1) <= max(r2.y, r2.y + h2)
    );
    int r = xhit && yhit;
    return r;
}

int strsw(char *src, char *match)
{
    if (!src || !match)
        return 0;
    while (*match && *src && *src == *match && (src++, match++, 1));
    int r = *match == 0;
    return r;
}

int strew(char *src, char *match)
{
    if (!src || !match)
        return 0;
    char *srcend = src;
    char *matchend = match;
    // find end of src.
    while (*srcend && (srcend++, 1));
    // find end of match.
    while (*matchend && (matchend++, 1));
    // now check if they match.
    while (src < srcend && 
           match < matchend && 
           *srcend == *matchend && 
           (srcend--, matchend--, 1));
    int r = match == matchend && *srcend == *matchend;
    return r;
}

int streq(char *a, char *b)
{
    if (!a || !b)
        return 0;
    while (*a && *b && *a == *b && (a++, b++, 1));
    int r = *a == 0 && *b == 0;
    return r;
}

int streqn(char *a, char *b, usize n)
{
    if (!a || !b || n == 0)
        return 0;
    while (n > 0 && *a && *b && *a == *b && (a++, b++, n--, 1));
    int r = n == 0;
    return r;
}

usize strl(char *src)
{
    if (!src)
        return 0;
    usize r = 0;
    while (*src && (r++, src++, 1));
    return r;
}

usize stri64(char *dest, i64 x, u64 base, usize n)
{
    if (!dest || !n || !base)
        return 0;
    int negative = x < 0;
    usize r = 0;
    // count digits in number.
    for (i64 i = x; i; i /= base)
        r++;
    // if we can't store the whole number
    // just return.
    if (r + negative >= n)
        return 0;
    // add negative symbol.
    if (negative)
        *dest++ = '-';
    x = abs(x);
    for (usize i = 0; i < r; i++) {
        i64 rem = x % base;
        x = x / base;
        *(dest + r - i - 1) = rem < 10 ? rem + '0' : 'a' + rem - 10;
    }
    // null terminator.
    *(dest + negative + r) = 0;
    return r + negative;
}

usize strdbl(char *dest, double x, usize n)
{
    if (!dest || !n)
        return 0;
    int negative = x < 0;
    usize r = 0;
    x = abs(x);
    i64 d = (i64)x;
    i64 dec = (i64)(x * 100) % 100;
    if (negative)
        *dest++ = '-';
    while (r + negative < n && d > 0) {
        *dest++ = (d % 10) + '0';
        r++;
        d /= 10;
    }
    if (r + 3 + negative >= n) {
        // if there isn't enough space, roll back what we wrote.
        for (usize i = 0; i < r + negative; i++)
            *dest-- = 0;
        return 0;
    }
    for (usize i = 0, m = r / 2; i < m; i++) {
        char last = *(dest - r + i);
        *(dest - r + i) = *(dest - (i + 1));
        *(dest - (i + 1)) = last;
    }
    *dest++ = '.';
    *dest++ = (dec / 10) + '0';
    *dest++ = (dec % 10) + '0';
    // null terminator.
    *dest = 0;
    return r + 3 + negative;
}

usize strf(char *dest, usize n, char *format, ...)
{
    if (!dest || !format)
        return 0;
    char *head = dest;
    va_list va;
    va_start(va, format);
    while (*format && (dest - head) < n) {
        int iscommand = *format == '%';
        if (iscommand && *(format + 1) == '%') {
            // skip %%.
            format++;
            format++;
            *dest++ = '%';
            continue;
        }
        if (iscommand && *(format + 1) == 'c') {
            // skip %c.
            format++;
            format++;
            char c = (char)va_arg(va, int);
            *dest++ = c;
            continue;
        }
        if (iscommand && *(format + 1) == 's') {
            // skip %s.
            format++;
            format++;
            char *buf = va_arg(va, char *);
            if (!buf)
                buf = "(null)";
            usize len = strl(buf);
            for (usize i = 0; i < len; i++) {
                *dest++ = *buf++;
                if (dest - head >= n)
                    goto finish;
            }
            continue;
        }
        if (iscommand && *(format + 1) == '*' && *(format + 2) == 's') {
            // skip %*s.
            format++;
            format++;
            format++;
            usize len = va_arg(va, usize);
            char *buf = va_arg(va, char *);
            if (!buf) {
                len = sizeof("(null)") - 1;
                buf = "(null)";
            }
            for (usize i = 0; i < len; i++) {
                *dest++ = *buf++;
                if (dest - head >= n)
                    goto finish;
            }
            continue;
        }
        if (iscommand && *(format + 1) == 'x') {
            // skip %x.
            format++;
            format++;
            i64 x = va_arg(va, i64);
            usize r = stri64(dest, x, 16, n - (dest - head));
            if (!r)
                *dest++ = '?';
            else
                dest += r;
            continue;
        }
        if (iscommand && *(format + 1) == 'd') {
            // skip %d.
            format++;
            format++;
            i64 d = va_arg(va, i64);
            usize r = stri64(dest, d, 10, n - (dest - head));
            if (!r)
                *dest++ = '?';
            else
                dest += r;
            continue;
        }
        if (iscommand && *(format + 1) == 'f') {
            // skip %f.
            format++;
            format++;
            double f = va_arg(va, double);
            usize r = strdbl(dest, f, n - (dest - head));
            if (!r)
                *dest++ = '?';
            else
                dest += r;
            continue;
        }
        if (iscommand && *(format + 1) == 'v') {
            // skip %v.
            format++;
            format++;
            *dest++ = '(';
            if (dest - head >= n)
                goto finish;
            v2 v = va_arg(va, v2);
            usize d = strdbl(dest, v.f[0], n - (dest - head));
            if (!d)
                *dest++ = '?';
            else
                dest += d;
            if (dest - head >= n)
                goto finish;
            *dest++ = ':';
            if (dest - head >= n)
                goto finish;
            d = strdbl(dest, v.f[1], n - (dest - head));
            if (!d)
                *dest++ = '?';
            else
                dest += d;
            if (dest - head >= n)
                goto finish;
            *dest++ = ')';
            continue;
        }
        *dest++ = *format++;
    }
finish:
    va_end(va);
    // null terminator.
    if ((dest - head) < n)
        *dest++ = 0;
    else
        *(dest - 1) = 0;
    usize r = dest - head;
    return r;
}

void mems(void *dest, byte src, usize n)
{
    if (!dest)
        return;
    byte *bdest = (byte *)dest;
    for (usize i = 0; i < n; i++)
        *bdest++ = src;
}

int memeq(void *a, void *b, usize n)
{
    if (!a || !b)
        return 0;
    byte *ba = (byte *) a;
    byte *bb = (byte *) b;
    for (usize i = 0; i < n; i++) {
        if (*ba != *bb)
            return 0;
        ba++;
        bb++;
    }
    return 1;
}

void memc(void *dest, void *src, usize n)
{
    if (!dest || !src)
        return;
    byte *bdest = (byte *) dest;
    byte *bsrc = (byte *) src;
    for (usize i = 0; i < n; i++)
        *bdest++ = *bsrc++;
}

u32 randi(u32 *seed)
{
    static u32 debugseed = -324516438;
    if (!seed)
        seed = &debugseed;
    *seed = (214013 * (*seed) + 2531011);
    u32 r = (*seed >> 16) & 0x7FFF;
    return r;
}

u32 randi2(u32 *seed, u32 lower, u32 upper)
{
    lower = min(lower, upper);
    upper = max(lower, upper);
    u32 r = (randi(seed) % (upper - lower + 1)) + lower;
    return r;
}

#endif
