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
    assert(strstartswith("something", "som"));
    assert(strstartswith("something", ""));
    assert(!strstartswith("something", "asom"));
    assert(!strstartswith("something", "soma"));
    assert(!strstartswith("something", "soam"));

    assert(strendswith("something", ""));
    assert(strendswith("something", "hing"));
    assert(!strendswith("something", "ahing"));
    assert(!strendswith("something", "hiang"));

    assert(streq("something", "something"));
    assert(!streq("something", ""));
    assert(!streq("something", "somethings"));

    assert(streqn("something", "something", 2));
    assert(!streqn("something", "", 1));
    assert(streqn("something", "somethings", 9));

    assert(strlen2("s") == 1);
    assert(strlen2("") == 0);
    assert(strlen2("something") == 9);

    int arr[] = {1,2,3,4,5};
    foreach(int, x, arr)
        printf("%d\n", *x);
    foreachn(int, x, arr, 3)
        printf("%d\n", *x);
    printf("---\n");
    foreachrev(int, x, arr)
        printf("%d\n", *x);
    foreachrevn(int, x, arr, 3)
        printf("%d\n", *x);

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

    struct coroutine coro = {0};
    testcoroutine(&coro);
    testcoroutine(&coro);
    testcoroutine(&coro);
    testcoroutine(&coro);
    testcoroutine(&coro);
    testcoroutine(&coro);

    printf("all good!\n");

    char buf[256] = {0};
    usize written = strf2(
        buf, 
        "xtest %s %c %%xs %s qwedvfplx  0x%x %f %v!\n", 
        0,
        'a',
        "dynamic", 
        0xfafa, 
        -12345.215,
        (v2){{26.31, 451245.12}}
    );
    printf("%d %s", written, buf);

    return 0;
}
