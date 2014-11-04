#include <time.h>
#include <stdio.h>
#include "../../main/c/mod/event.h"

int main() {

//	struct event *e;
//	initEventStore("filename.txt");
//	storeEvent(e);
//	finalizeEventStore();

	FILE *ff = fopen("bin/speed", "w");
	fprintf(ff, "%s %s;", "Niklas", "Gerdt");
	fclose(ff);

	char d = 'B';
	struct R20_eventHeader eh = { .source = 'A', };
	struct R20_event ee = { .header = eh };
	ee.header.destination = d;
	printf("%c-%c-%c-%c\n", eh.source, ee.header.source, d, ee.header.destination);

}

