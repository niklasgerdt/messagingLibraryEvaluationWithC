#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mod/util.h"
#include "pubsub.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
	long pauseNanos = atol(argv[1]);
	char *address = argv[2];
	const char *channel = argv[3];
	int eventMessageLength = atoi(argv[4]);
	char eventFile[20] = "logs/EVENTSTORE-PUB-";
	strcat(eventFile, channel);

	R20_initEventStore(eventFile);
	initPub(address, channel);
	R20_initTerminator();

	size_t size = sizeof(struct R20_event) + eventMessageLength * sizeof(char);
	char *eData = malloc(eventMessageLength * sizeof(char));
	memset(eData, 'A', eventMessageLength * sizeof(char));

	long idCount = 0;
	while (R20_killSignal == 0) {
		struct R20_eventHeader eh = { .source = "XXXX\0", .id = idCount, .created = R20_currentTime() };
		strncpy(eh.source, channel, strlen(channel));
		struct R20_event e = { .header = eh, .dataLength = eventMessageLength, .data = eData };
		pub(e, size);
		e.header.published = R20_currentTime();
		R20_storeEvent(e);
		R20_pause(pauseNanos);
		idCount++;
	}

	R20_finalizeEventStore();
	destroyPub();
	return 0;
}
