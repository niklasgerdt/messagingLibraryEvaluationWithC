#include <stdio.h>

int main() {

	FILE *f = fopen("build/temp.txt", "w");
	fprintf(f, "%s", "TEMP\n");
	fclose(f);

	return 0;
}

