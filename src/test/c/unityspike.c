#include "../unity/src/unity.h"
#include "../unity/src/unity_internals.h"

#include <stdio.h>

extern void test_pointerSpike(void);

void test_pointerSpike(void) {
	int *p;
	int i = 5;
	p = &i;
	TEST_ASSERT_EQUAL(&i, p); //addresses match
	TEST_ASSERT_EQUAL(i, *p); // values match
}

struct ds {
	int i;
};

void test_struct(void) {
	struct ds d = { .i = 100 };
	struct ds *c = &d;
	TEST_ASSERT_EQUAL(&d, c); // same addresses
	TEST_ASSERT_EQUAL(d.i, c->i); // same values
	TEST_ASSERT_EQUAL(&d.i, &c->i); // same addresses
}

static void runTest(UnityTestFunction test) {
	test();
}

int main(int argc, char * argv[]) {
	UnityBegin("unityspike.c");

	// RUN_TEST calls runTest
	RUN_TEST(test_pointerSpike);
	RUN_TEST(test_struct);

	UnityEnd();
	return 0;
}
