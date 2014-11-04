#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mod/event.h"
#include "pubsub.h"
#include "mod/util.h"

int main(int argc, char *argv[]) {
	char *address = argv[1];
	char *channel = argv[2];
	char eventFile[21] = "logs/EVENTSTORE-SUB-";
	strcat(eventFile, channel);

	R20_initTerminator();
	R20_initEventStore(eventFile);
	initSub(address, channel);

	while (R20_killSignal == 0) {
		struct R20_event e = sub();
		strncpy(e.header.destination, "XXXX\0", 4);
		strncpy(e.header.destination, channel, strlen(channel));
		e.header.routed = R20_currentTime();
		R20_storeEvent(e);
	}

	R20_finalizeEventStore();
	destroySub();
	return 0;
}
