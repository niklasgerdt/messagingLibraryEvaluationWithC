#ifndef EVENT_H_
#define EVENT_H_

#include <time.h>

struct R20_eventHeader {
	char source[5];
	char destination[5];
	long id;
	struct timespec created;
	struct timespec published;
	struct timespec routed;
};

struct R20_event {
	struct R20_eventHeader header;
	int dataLength;
	char *data;
};

void R20_initEventStore(const char *fileName);

void R20_storeEvent(struct R20_event e);

void R20_finalizeEventStore();

void R20_printEvent(struct R20_event e);

void R20_printEventHeader(struct R20_eventHeader e);

#endif /* EVENT_H_ */
