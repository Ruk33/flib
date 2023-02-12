#ifndef flibh
#define flibh

#include <stdarg.h>

typedef unsigned char       u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned int long  u64;

typedef signed char       i8;
typedef signed short     i16;
typedef signed int       i32;
typedef signed int long  i64;

typedef u8 byte;

typedef i64 ssize;
typedef u64 usize;

#define arrlen(x) \
    (sizeof(x)/sizeof(*(x)))
// traverse elements in array.
#define foreach(type, x, arr) \
    for (type *x = (arr); x < (arr) + arrlen(arr); x++)
// traverse up to n elements in array.
#define foreachn(type, x, arr, n) \
    for (type *x = (arr); x < (arr) + min(arrlen(arr), (n)); x++)
// traverse elements in array starting from the end.
#define foreachrev(type, x, arr) \
    for (type *x = (arr) + arrlen(arr) - 1; x >= (arr); x--)
// traverse back n elements in array.
#define foreachrevn(type, x, arr, n) \
    for (type *x = (arr) + arrlen(arr) - 1; x >= (arr) + arrlen(arr) - (n); x--)

#define abs(x) \
    ((x) < 0 ? (-(x)) : (x))
#define min(x1, x2) \
    ((x1) < (x2) ? (x1) : (x2))
#define max(x1, x2) \
    ((x1) > (x2) ? (x1) : (x2))
#define clamp(x, a, b) \
    (min(max((x), min((a), (b))), max((a), (b))))
#define sqr(x) \
    ((x)*(x))

#define cstart(ctx)                         \
    do {                                    \
        struct coroutine *__coro = (ctx);   \
        switch((__coro)->state) {           \
        case 0:

#define yield(id)               \
    do {                        \
        (__coro)->state = (id); \
        return;                 \
        case (id):;             \
    } while (0)

#define syield(id, _timeout, dt)        \
    do {                                \
        (__coro)->timeout = (_timeout); \
        yield(id);                      \
        (__coro)->timeout -= (dt);      \
        if ((__coro)->timeout > 0)      \
            return;                     \
    } while(0)

#define creset                              \
    do {                                    \
        *(__coro) = (struct coroutine){0};  \
    } while (0)

#define cend    \
        }       \
    } while (0)

// str format using fixed array as destination.
#define strf2(dest, format, ...) \
    (strf((dest), sizeof(dest), format, __VA_ARGS__))

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
int v2inrange(v2 target, v2 src, float r);
// check if target is inside a rect starting at src.
int v2inrect(v2 target, v2 src, float w, float h);

// check if string src starts with match.
// both strings need to be null terminated.
// src and match can be null.
int strstartswith(char *src, char *match);
// check if string src ends with match.
// both strings need to be null terminated.
// src and match can be null.
int strendswith(char *src, char *match);
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
usize strlen2(char *src);
// convert integer to string.
// dest can be null.
// dest will be null terminated.
// the amount of characters written is returned.
usize stri64(char *dest, i64 x, usize n);
// convert double to string.
// dest can be null.
// dest will be null terminated.
// the amount of characters written is returned.
usize strdbl(char *dest, double x, usize n);
// write up to n bytes of formatted string into dest.
// dest will be null terminated.
// formats:
// %%  = escapes % and writes %.
// %c  = writes character.
// %s  = writes strings up to null terminator.
// %*s = writes n bytes of string. 
//       example: strf(dest, n, "%*s", 2, "lorem")
//       only writes 2 bytes/chars, meaning, "lo"
// %d  = writes int.
// %f  = writes float/double with 2 decimals.
// %v  = writes vector with format "(x:y)"
usize strf(char *dest, usize n, char *format, ...);

// random integer.
// seed can be null.
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
    v2 r = {{x1.f[0]+x2.f[0], x1.f[1]+x2.f[1]}};
    return r;
}

v2 v2sub(v2 x1, v2 x2)
{
    v2 r = {{x1.f[0]-x2.f[0],x1.f[1]-x2.f[1]}};
    return r;
}

v2 v2skl(v2 x, float n)
{
    v2 r = {{x.f[0]*n, x.f[1]*n}};
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
    if (len2 > 0) {
        len2 = Q_rsqrt(len2);
        r = (v2){{x.f[0]*len2, x.f[1]*len2}};
    }
    return r;
}

float v2dot(v2 x1, v2 x2)
{
    float r = (x1.f[0]*x2.f[0])+(x1.f[1]*x2.f[1]);
    return r;
}

float v2len2(v2 x)
{
    float r = sqr(x.f[0])+sqr(x.f[1]);
    return r;
}

float v2dist2(v2 src, v2 dest)
{
    float r = sqr(src.f[0]-dest.f[0])+sqr(src.f[1]-dest.f[1]);
    return r;
}

int v2inrange(v2 target, v2 src, float range)
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

int strstartswith(char *src, char *match)
{
    if (!src || !match)
        return 0;
    while (*match && *src == *match && (src++, match++));
    int r = *match == 0;
    return r;
}

int strendswith(char *src, char *match)
{
    if (!src || !match)
        return 0;
    char *srcend = src;
    char *matchend = match;
    // find end of src.
    while (*srcend && srcend++);
    // find end of match.
    while (*matchend && matchend++);
    // now check if they match.
    while (match <= matchend && *srcend == *matchend && (srcend--, matchend--));
    int r = match > matchend;
    return r;
}

int streq(char *a, char *b)
{
    if (!a || !b)
        return 0;
    while (*a && *b && *a == *b && (a++, b++));
    int r = *a == 0 && *b == 0;
    return r;
}

int streqn(char *a, char *b, usize n)
{
    if (!a || !b || n == 0)
        return 0;
    while (n > 0 && *a && *b && *a == *b && (a++, b++, n--));
    int r = n == 0;
    return r;
}

usize strlen2(char *src)
{
    if (!src)
        return 0;
    usize r = 0;
    while (*src && (r++, src++));
    return r;
}

usize stri64(char *dest, i64 x, usize n)
{
    if (!dest || !n)
        return 0;
    int negative = x < 0;
    usize r = 0;
    // count digits in number.
    for (i64 i = x; i; i /= 10)
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
        i64 rem = x % 10;
        x = x / 10;
        *(dest + r - i - 1) = rem + '0';
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
    x += 0.005;
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
    for (usize i = 0, m = r/2; i < m; i++) {
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
    while (*format && dest - head < n) {
        if (*format + *(format+1) == '%' + '%') {
            // skip %%.
            format++;
            format++;
            *dest++ = '%';
            continue;
        }
        if (*format + *(format+1) == '%' + 'c') {
            // skip %c.
            format++;
            format++;
            char c = (char)va_arg(va, int);
            *dest++ = c;
            continue;
        }
        if (*format + *(format+1) == '%' + 's') {
            // skip %s.
            format++;
            format++;
            char *buf = va_arg(va, char *);
            if (!buf)
                buf = "(null)";
            usize len = strlen2(buf);
            for (usize i = 0; i < len; i++) {
                *dest++ = *buf++;
                if (dest - head >= n)
                    goto finish;
            }
            continue;
        }
        if (*format + *(format+1) + *(format+2) == '%' + '*' + 's') {
            // skip %*s.
            format++;
            format++;
            format++;
            usize len = va_arg(va, usize);
            char *buf = va_arg(va, char *);
            if (!buf) {
                len = 6;
                buf = "(null)";
            }
            for (usize i = 0; i < len; i++) {
                *dest++ = *buf++;
                if (dest - head >= n)
                    goto finish;
            }
            continue;
        }
        if (*format + *(format+1) == '%' + 'd') {
            // skip %d.
            format++;
            format++;
            i64 d = va_arg(va, i64);
            usize r = stri64(dest, d, n - (dest - head));
            if (!r)
                *dest++ = '?';
            else
                dest+=r;
            continue;
        }
        if (*format + *(format+1) == '%' + 'f') {
            // skip %f.
            format++;
            format++;
            double f = va_arg(va, double);
            usize r = strdbl(dest, f, n - (dest - head));
            if (!r)
                *dest++ = '?';
            else
                dest+=r;
            continue;
        }
        if (*format + *(format+1) == '%' + 'v') {
            // skip %v.
            format++;
            format++;
            *dest++ = '(';
            if (dest - head >= n)
                goto finish;
            v2 v = va_arg(va, v2);
            double dble = (double)v.f[0];
            usize d = strdbl(dest, dble, n - (dest - head));
            if (!d)
                *dest++ = '?';
            else
                dest+=d;
            if (dest - head >= n)
                goto finish;
            *dest++ = ':';
            if (dest - head >= n)
                goto finish;
            dble = (double)v.f[1];
            d = strdbl(dest, dble, n - (dest - head));
            if (!d)
                *dest++ = '?';
            else
                dest+=d;
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
    *dest = 0;
    usize r = dest - head;
    // don't count null terminator.
    if (r)
        r--;
    return r;
}

u32 randi(u32 *seed)
{
    static u32 debugseed = 0;
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
