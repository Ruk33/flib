#include <assert.h>
#include <stdio.h>
#include "flib.h"

void test_coroutine(struct coroutine *coro)
{
	coroutine_start(coro) {
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
		reset;
	} coroutine_end;
}

unsigned int custom_formatter(char *dest, void *v, unsigned int n)
{
	int x = *(int *)v;
	x /= 10;
	unsigned int written = str_int(dest, x, 10, n);
	return written;
}

int main(void)
{
	{
		int r[32] = {0};
		assert(array_length(r) == 32);
	}
	{
		int r[] = {1,2,3,4};
		for_each(int, x, r)
			*x = *x+1;
		assert(r[0] == 2);
		assert(r[1] == 3);
		assert(r[2] == 4);
		assert(r[3] == 5);
	}
	{
		int r[] = {1,2,3,4};
		for_each_n(int, x, r, 2)
			*x = *x+1;
		assert(r[0] == 2);
		assert(r[1] == 3);
		assert(r[2] == 3);
		assert(r[3] == 4);
	}
	{
		int r[] = {1,2,3,4};
		for_each_reverse(int, x, r)
			*x = *x+1;
		assert(r[0] == 2);
		assert(r[1] == 3);
		assert(r[2] == 4);
		assert(r[3] == 5);
	}
	{
		int r[] = {1,2,3,4};
		for_each_reverse_n(int, x, r, 2)
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
		assert(lerp(0.5, 1, 2) - 1.5 <= 0.001);
		assert(lerp(1, 1, 2) == 2);
		assert(lerp(0, 1, 2) == 1);
		assert(sqr(2) == 2*2);
	}
	{
		v2 x1 = {{3, 2}};
		v2 x2 = {{1, 4}};
		v2 r = {0};
		r = v2_add(x1, x2);
		assert(r.f[0] == 4);
		assert(r.f[1] == 6);
		r = v2_sub(x1, x2);
		assert(r.f[0] == 2);
		assert(r.f[1] == -2);
		r = v2_scale(x1, 2);
		assert(r.f[0] == 6);
		assert(r.f[1] == 4);
		r = v2_direction_to(x1, x2);
		assert(r.f[0] - 0.7f < 0.01f);
		assert(r.f[1] - 0.7f < 0.01f);
		// r = v2dot
		assert(v2_sqr_length(x1) == sqr(x1.f[0]) + sqr(x1.f[1]));
		assert(v2_sqr_distance(x1, x2) == v2_sqr_length(v2_sub(x2, x1)));
	}
	{
		assert(str_starts_with("something", "som"));
		assert(str_starts_with("something", ""));
		assert(!str_starts_with("something", "asom"));
		assert(!str_starts_with("something", "soma"));
		assert(!str_starts_with("something", "soam"));
	}
	{
		assert(str_ends_with("something", ""));
		assert(str_ends_with("something", "hing"));
		assert(!str_ends_with("something", "ahing"));
		assert(!str_ends_with("something", "hiang"));
		assert(!str_ends_with("hing", "shing"));
	}
	{
		assert(str_equals("something", "something"));
		assert(!str_equals("something", ""));
		assert(!str_equals("something", "somethings"));
		assert(!str_equals("somethings", "something"));
	}
	{
		assert(str_equals_n("something", "something", 2));
		assert(!str_equals_n("something", "", 1));
		assert(str_equals_n("something", "somethings", 9));
		assert(str_equals_n("some", "somethings", 4));
		assert(str_equals_n("some", "somethings", 3));
	}
	{
		assert(str_length("s") == 1);
		assert(str_length("") == 0);
		assert(str_length("something") == 9);
	}
	{
		char b[32] = {0};
		assert(str_int(b, 12345, 10, sizeof(b)) == 6);
		assert(str_equals(b, "12345"));
		str_int(b, 0, 10, sizeof(b));
		assert(str_equals(b, "0"));
		assert(str_int(b, -32, 10, sizeof(b)) == 4);
		assert(str_equals(b, "-32"));
	}
	{
		char b[32] = {0};
		assert(str_double(b, 12345.321, sizeof(b)) == 9);
		assert(str_equals(b, "12345.32"));
		assert(str_double(b, -12345.321, sizeof(b)) == 10);
		assert(str_equals(b, "-12345.32"));
		assert(str_double(b, 12345, sizeof(b)) == 9);
		assert(str_equals(b, "12345.00"));
		assert(str_double(b, 0.0f, sizeof(b)) == 4);
		assert(str_equals(b, ".00"));
	}
	{
		char *b1 = "this is a test";
		char b2[]= "this is a test";
		assert(str_hash(b1) == str_hash(b2));
		char *b3 = "tihs is a test";
		assert(str_hash(b3) != str_hash(b2));
	}
	{
		char b[256] = {0};
		unsigned int n = strf_ex(
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
		assert(str_equals(b, "test 32 (2.25:33.45) 23.45 fafa lo a sample.\n"));
		assert(n == sizeof("test 32 (2.25:33.45) 23.45 fafa lo a sample.\n"));
		// strf adds a null terminator.
		assert(strf(b, 4, "%s", "tset") == 4);
		assert(str_equals(b, "tse"));
		
		int x = 30;
		n = strf_ex(b,
				  "%? my custom formatter.",
				  custom_formatter,
				  &x);
		assert(str_equals(b, "3 my custom formatter."));
	}
	{
		unsigned int id = 0;
		unsigned int ids[4] = {0};
		int tmp = -1;
		id_get(&id, ids, array_length(ids));
		assert(id == 0);
		id_get(&id, ids, array_length(ids));
		assert(id == 1);
		id_get(&id, ids, array_length(ids));
		assert(id == 2);
		id_get(&id, ids, array_length(ids));
		assert(id == 3);
		
		id_recycle(ids, 2);
		id_recycle(ids, 3);
		
		id_get(&id, ids, array_length(ids));
		assert(id == 3);
		id_get(&id, ids, array_length(ids));
		assert(id == 2);
		
		assert(id_get(&id, ids, array_length(ids)) == 0);
		assert(id_get(&id, ids, array_length(ids)) == 0);
		assert(id_get(&id, ids, array_length(ids)) == 0);
		// make sure we don't overflow.
		assert(tmp == -1);
	}
	{
		printf("random: %d\n", random_int(0));
		printf("random: %d\n", random_int(0));
		printf("random: %d\n", random_int(0));
		printf("random: %d\n", random_int(0));
		printf("random: %d\n", random_int(0));
		printf("random between 5 and 10: %d\n", random_int_ex(0, 5, 10));
		printf("random between 5 and 10: %d\n", random_int_ex(0, 5, 10));
		printf("random between 5 and 10: %d\n", random_int_ex(0, 5, 10));
		printf("random between 5 and 10: %d\n", random_int_ex(0, 5, 10));
		printf("random between 5 and 10: %d\n", random_int_ex(0, 5, 10));
		printf("random between 5 and 10: %d\n", random_int_ex(0, 5, 10));
	}
	{
		struct coroutine coro = {0};
		test_coroutine(&coro);
		test_coroutine(&coro);
		test_coroutine(&coro);
		test_coroutine(&coro);
		test_coroutine(&coro);
		test_coroutine(&coro);
	}
	
	printf("all good!\n");
	return 0;
}
