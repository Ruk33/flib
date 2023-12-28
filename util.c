#include <stdint.h>

typedef uint8_t byte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int8_t i8;
typedef int16_t s16;
typedef int16_t i16;
typedef int32_t s32;
typedef int32_t i32;
typedef int64_t s64;
typedef int64_t i64;

#define countof(x) \
(sizeof(x) / sizeof(*(x)))

#ifndef abs
#define abs(x) \
((x) < (0) ? -(x) : (x))
#endif

#ifndef min
#define min(a, b) \
((a) > (b) ? (b) : (a))
#endif

#ifndef max
#define max(a, b) \
((a) < (b) ? (b) : (a))
#endif

#define clamp(x, a, b) \
(min(max((x), min((a), (b))), max((a), (b))))

#define lerp(start, end, t) \
((1 - (t)) * (start) + (t) * (end))

#define sqr(x) \
((x) * (x))

#define coroutine(ctx) \
struct coroutine *__coro = (ctx); \
switch (__coro->state) \
case 0:

#define yield(id) \
__coro->state = (id); \
break; \
case (id):

#define syield(id, sleep_for, dt) \
__coro->sleep = (sleep_for); \
yield(id); \
__coro->sleep -= (dt); \
if (__coro->sleep > 0) break

#define reset \
*__coro = (struct coroutine) {0}

struct coroutine {
    u64 state;
    float sleep;
};

u32 random_int(u32 *seed);
u32 random_int_between(u32 *seed, u32 lower, u32 upper);

u32 random_int(u32 *seed)
{
    static u32 debug_seed = -324516438;
	if (!seed)
		seed = &debug_seed;
	*seed = (214013 * (*seed) + 2531011);
    u32 r = (*seed >> 16) & 0x7FFF;
	return r;
}

u32 random_int_between(u32 *seed, u32 lower, u32 upper)
{
    u32 safe_lower = min(lower, upper);
    u32 safe_upper = max(lower, upper);
    u32 r = (random_int(seed) % (safe_upper - safe_lower + 1)) + safe_lower;
    return r;
}

#ifdef run
#include <assert.h>
#include <stdio.h>

void test_coroutine(struct coroutine *ctx)
{
    coroutine(ctx) {
        fprintf(stderr, "first yield.\n");
        yield(10);
        
        fprintf(stderr, "simulate waiting 3 seconds.\n");
        syield(20, 3.0f, 1.0f);
        
        fprintf(stderr, "final yield!\n");
    }
}

int main()
{
#define test(x) \
(assert((x) && "failed " #x), fprintf(stderr, "passed - %s\n", #x))
    
    fprintf(stderr, "\n\nrunning tests from util.c\n\n");
    test(sizeof(byte) == 1);
    test(sizeof(u8) == 1);
    test(sizeof(u16) == 2);
    test(sizeof(u32) == 4);
    test(sizeof(u64) == 8);
    
    test(sizeof(i8) == 1);
    test(sizeof(i16) == 2);
    test(sizeof(i32) == 4);
    test(sizeof(i64) == 8);
    
    test(sizeof(s8) == 1);
    test(sizeof(s16) == 2);
    test(sizeof(s32) == 4);
    test(sizeof(s64) == 8);
    
    int test[8] = {0};
    test(countof(test) == 8);
    
    test(abs(-2) == 2);
    test(abs(42) == 42);
    
    test(min(2, 3) == 2);
    test(max(2, 3) == 3);
    test(clamp(1, 2, 3) == 2);
    test(clamp(4, 2, 3) == 3);
    
    test(55.0f - lerp(10, 100, 0.5f) < 0.01f);
    test(100.0f - lerp(10, 100, 1.0f) < 0.01f);
    
    test(sqr(25) == 25 * 25);
    
    fprintf(stderr, "random int: %d\n", random_int(0));
    fprintf(stderr, "random int: %d\n", random_int(0));
    fprintf(stderr, "random int: %d\n", random_int(0));
    
    fprintf(stderr, "random int between 10 and 100: %d\n", random_int_between(0, 10, 100));
    fprintf(stderr, "random int between 10 and 100: %d\n", random_int_between(0, 10, 100));
    fprintf(stderr, "random int between 10 and 100: %d\n", random_int_between(0, 10, 100));
    
    struct coroutine ctx = {0};
    test_coroutine(&ctx);
    test_coroutine(&ctx);
    test_coroutine(&ctx);
    test_coroutine(&ctx);
    test_coroutine(&ctx);
}
#endif
