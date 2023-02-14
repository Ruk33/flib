#include <assert.h>
#include <stdio.h>
#include "flib.h"

#define flibc
#include "flib.h"

void testcoroutine(struct coroutine *coro)
{
    cstart(coro) {
        printf("this is the first state.\n");
        yield(1);
        printf("this is the second state.\n");
        yield(2);
        printf("this is the third state.\n");
        syield(
            3, 
            /* waiting time in seconds */ 3,
            /* time past each time the function is called */ 2
        );
        printf("three seconds past!\n");
        creset;
    } cend;
}

int main(void)
{
    {
        int r[32] = {0};
        assert(arrl(r) == 32);
    }
    {
        int r[] = {1,2,3,4};
        each(int, x, r)
            *x = *x+1;
        assert(r[0] == 2);
        assert(r[1] == 3);
        assert(r[2] == 4);
        assert(r[3] == 5);
    }
    {
        int r[] = {1,2,3,4};
        eachn(int, x, r, 2)
            *x = *x+1;
        assert(r[0] == 2);
        assert(r[1] == 3);
        assert(r[2] == 3);
        assert(r[3] == 4);
    }
    {
        int r[] = {1,2,3,4};
        eachr(int, x, r)
            *x = *x+1;
        assert(r[0] == 2);
        assert(r[1] == 3);
        assert(r[2] == 4);
        assert(r[3] == 5);
    }
    {
        int r[] = {1,2,3,4};
        eachrn(int, x, r, 2)
            *x = *x+1;
        assert(r[0] == 1);
        assert(r[1] == 2);
        assert(r[2] == 4);
        assert(r[3] == 5);
    }
    {
        assert(abs(-32) == 32);
        assert(abs(32) == 32);
        assert(min(32, 1) == 1);
        assert(max(32, 1) == 32);
        assert(clamp(32, 30, 31) == 31);
        assert(clamp(20, 15, 30) == 20);
        assert(clamp(10, 30, 31) == 30);
        assert(sqr(2) == 2*2);
    }
    {
        v2 x1 = {{3, 2}};
        v2 x2 = {{1, 4}};
        v2 r = {0};
        r = v2add(x1, x2);
        assert(r.f[0] == 4);
        assert(r.f[1] == 6);
        r = v2sub(x1, x2);
        assert(r.f[0] == 2);
        assert(r.f[1] == -2);
        r = v2skl(x1, 2);
        assert(r.f[0] == 6);
        assert(r.f[1] == 4);
        r = v2dir2(x1, x2);
        assert(r.f[0] - 0.7f < 0.01f);
        assert(r.f[1] - 0.7f < 0.01f);
        // r = v2dot
        assert(v2len2(x1) == sqr(x1.f[0]) + sqr(x1.f[1]));
        assert(v2dist2(x1, x2) == v2len2(v2sub(x2, x1)));
        // r = v2inrng
        // r = v2inrect
    }
    {
        assert(strstartswith("something", "som"));
        assert(strstartswith("something", ""));
        assert(!strstartswith("something", "asom"));
        assert(!strstartswith("something", "soma"));
        assert(!strstartswith("something", "soam"));
    }
    {
        assert(strendswith("something", ""));
        assert(strendswith("something", "hing"));
        assert(!strendswith("something", "ahing"));
        assert(!strendswith("something", "hiang"));
        assert(!strendswith("hing", "shing"));
    }
    {
        assert(streq("something", "something"));
        assert(!streq("something", ""));
        assert(!streq("something", "somethings"));
        assert(!streq("somethings", "something"));
    }
    {
        assert(streqn("something", "something", 2));
        assert(!streqn("something", "", 1));
        assert(streqn("something", "somethings", 9));
        assert(streqn("some", "somethings", 4));
        assert(streqn("some", "somethings", 3));
    }
    {
        assert(strlen2("s") == 1);
        assert(strlen2("") == 0);
        assert(strlen2("something") == 9);
    }
    {
        char b[32] = {0};
        assert(stri64(b, 12345, 10, sizeof(b)) == 5);
        assert(streq(b, "12345"));
    }
    {
        char b[32] = {0};
        assert(strdbl(b, 12345.321, sizeof(b)) == 8);
        assert(streq(b, "12345.32"));
        assert(strdbl(b, -12345.321, sizeof(b)) == 9);
        assert(streq(b, "-12345.32"));
        assert(strdbl(b, 12345, sizeof(b)) == 8);
        assert(streq(b, "12345.00"));
    }
    {
        char b[256] = {0};
        usize n = strf2(
            b, 
            "%s %d %v %f %x %*s %c sample.\n", 
            "test", 
            32, 
            (v2){{2.25, 33.45}}, 
            23.45, 
            0xfafa, 
            2, 
            "lorem", 
            'a'
        );
        assert(streq(b, "test 32 (2.25:33.45) 23.45 fafa lo a sample.\n"));
        assert(n == sizeof("test 32 (2.25:33.45) 23.45 fafa lo a sample.\n"));
        // strf adds a null terminator.
        assert(strf(b, 4, "%s", "tset") == 4);
        assert(streq(b, "tse"));
    }
    {
        int r[] = {1,2,3,4};
        mems(r, 0, sizeof(r));
        assert(r[0] + r[1] + r[2] + r[3] == 0);
        char *a = "something like this";
        char *b = "something else like this";
        assert(memeq(a, b, 10));
        assert(!memeq(a, b, 11));
        int x1[] = {1,2,3,4};
        int x2[] = {3,4,5,6};
        memc(x2, x1, sizeof(int)*2);
        assert(x2[0] == 1);
        assert(x2[1] == 2);
        assert(x2[2] == 5);
        assert(x2[3] == 6);
    }
    {
        printf("random: %d\n", randi(0));
        printf("random: %d\n", randi(0));
        printf("random: %d\n", randi(0));
        printf("random: %d\n", randi(0));
        printf("random: %d\n", randi(0));
        printf("random between 5 and 10: %d\n", randi2(0, 5, 10));
        printf("random between 5 and 10: %d\n", randi2(0, 5, 10));
        printf("random between 5 and 10: %d\n", randi2(0, 5, 10));
        printf("random between 5 and 10: %d\n", randi2(0, 5, 10));
        printf("random between 5 and 10: %d\n", randi2(0, 5, 10));
        printf("random between 5 and 10: %d\n", randi2(0, 5, 10));
    }
    {
        struct coroutine coro = {0};
        testcoroutine(&coro);
        testcoroutine(&coro);
        testcoroutine(&coro);
        testcoroutine(&coro);
        testcoroutine(&coro);
        testcoroutine(&coro);
    }

    printf("all good!\n");
    return 0;
}
