#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../mod/event.h"
#include "../pubsub.h"

static struct R20_event dummyEvent;

void initPub(const char *addr, const char *_channel) {
}

void destroyPub() {
}

void initSub(const char *addr, const char *_channel) {
}

void addSub(const char *addr) {
}

void destroySub() {
}

void pub(struct R20_event e, size_t size) {
}

struct R20_event sub() {
	return dummyEvent;
}

void med() {
}


