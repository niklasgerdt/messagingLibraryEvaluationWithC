#include <assert.h>
#include <string.h>

int main(int argc, char *argv) {
	int i = 0;
	assert(4 == sizeof(i));
	char c;
	assert(1 == sizeof(c));
	char *cp;
	assert(8 == sizeof(cp));
	char *str = "TEST";
	assert(32 == (strlen(str) * sizeof(str)));
	struct ds {
	};
	struct ds d;
	assert(0 == sizeof(d));
	typedef struct {
		char c;
		int i;
	} ds2;
	ds2 d2;
	assert(8 == sizeof(d2));
	typedef struct {
		char *c;
		int i;
	} ds3;
	ds3 d3;
	assert(16 == sizeof(d3));
	char ca[100];
	assert(100 == sizeof(ca));
	char ca2[i];
	assert(0 == sizeof(ca2));

	struct d3 {
		int strLen;
		char *str;
	};
	struct d3 d4 = { .strLen = 100, .str = "" };
	assert(16 == sizeof(d4));
	assert(116 == sizeof(d4) + d4.strLen * sizeof(char));

	return 0;
}

