#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include "event.h"
#include "util.h"

#define MILLION 1000000
#define EVENTSTORESIZE 10 * MILLION

static int eventCount = 0;
static struct R20_eventHeader *events;
static FILE *f;

void R20_initEventStore(const char *fileName) {
	printf("Opening file %s\n", fileName);
	f = fopen(fileName, "w");
	if (f == NULL) {
		printf("Error opening file!\n");
	}
	events = malloc(EVENTSTORESIZE * sizeof(struct R20_eventHeader));
}

void R20_printEvent(struct R20_event e) {
	R20_printEventHeader(e.header);
}

void R20_printEventHeader(struct R20_eventHeader e) {
	printf("%s;%s;%d;%lld.%09ld;%lld.%09ld;%lld.%09ld\n", e.source, e.destination, e.id, e.created.tv_sec, e.created.tv_nsec,
			e.published.tv_sec, e.published.tv_nsec, e.routed.tv_sec, e.routed.tv_nsec);
}

static void write() {
	for (int i = 0; i < eventCount; i++) {
		fprintf(f, "%s;%s;%d;%lld.%09ld;%lld.%09ld;%lld.%09ld\n", events[i].source, events[i].destination, events[i].id,
				events[i].created.tv_sec, events[i].created.tv_nsec, events[i].published.tv_sec, events[i].published.tv_nsec,
				events[i].routed.tv_sec, events[i].routed.tv_nsec);
	}
	struct timespec ts = R20_currentTime();
	printf("Events stored to file %d: %lld.%09ld\n", f->_fileno, ts.tv_sec, ts.tv_nsec);
	eventCount = 0;
	free(events);
	events = malloc(EVENTSTORESIZE * sizeof(struct R20_eventHeader));
}

void R20_storeEvent(struct R20_event e) {
	if (eventCount == EVENTSTORESIZE) {
		write();
	}
	events[eventCount] = e.header;
	eventCount++;
}

void R20_finalizeEventStore() {
	write();
	fclose(f);
	free(events);
}
