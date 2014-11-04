#include <assert.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <string.h>
#include <time.h>
#include "../mod/event.h"
#include "../mod/util.h"
#include "../pubsub.h"

static int publisher;
static int subscriber;
static char channel[10];

void initPub(const char *addr, const char *_channel) {
	strcpy(channel, _channel);
	printf("Binding pub %s:%s\n", addr, channel);
	publisher = nn_socket(AF_SP, NN_PUB);
	assert(publisher >= 0);
	assert(nn_bind(publisher, addr) >= 0);
	printf("Pub bind to %s:%s\n", addr, channel);
}

void destroyPub() {
	nn_shutdown(publisher, 0);
	printf("pub down\n");
}

void initSub(const char *addr, const char *_channel) {
	strcpy(channel, _channel);
	printf("Connection sub %s:%s\n", addr, channel);
	subscriber = nn_socket(AF_SP, NN_SUB);
	assert(subscriber >= 0);
	for (int i = 0; i < strlen(channel); i++) {
		if (channel[i] == 'N') {
			printf("no subscription (%s)\n", channel);
			assert(nn_setsockopt (subscriber, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) >= 0);
			break;
		} else {
			printf("subscribed to channel %c\n", channel[i]);
			char c[1] = { channel[i] };
			assert(nn_setsockopt (subscriber, NN_SUB, NN_SUB_SUBSCRIBE, c, 1) >= 0);
		}
	}
	assert(nn_connect(subscriber, addr) >= 0);
	printf("Sub connected to %s:%s\n", addr, channel);
}

void addSub(const char *addr) {
	assert(nn_connect(subscriber, addr) >= 0);
	printf("Sub connected to %s:%s\n", addr, channel);
}

void destroySub() {
	nn_shutdown(subscriber, 0);
	printf("Sub down\n");
}

void pub(struct R20_event e, size_t size) {
	char msgStr[size+1];
	sprintf(msgStr, "%4s;%ld;%lld.%09ld;%d;%s", e.header.source, e.header.id, e.header.created.tv_sec,
			e.header.created.tv_nsec, e.dataLength, e.data);
	void *msg = nn_allocmsg(strlen(msgStr), 0);
	strncpy(msg, msgStr, strlen(msgStr));
	int nbytes = nn_send(publisher, &msg, NN_MSG, 0);
	assert(nbytes == strlen(msgStr));
}

struct R20_event sub() {
	struct R20_event e;
	void *buf = NULL;
	int nbytes = nn_recv(subscriber, &buf, NN_MSG, 0);
	if (nbytes < 0) {
		R20_killSignal = -1;
		return e;
	} else {
		char src[4];
		long id;
		long int cSec;
		long int cNsec;
		sscanf(buf, "%4s;%ld;%lld.%09ld", src, &id, &cSec, &cNsec);
		struct timespec t = { .tv_sec = cSec, .tv_nsec = cNsec };
		struct R20_eventHeader eh = { .source = "XXXX\0", .id = id, .created = t };
		strncpy(eh.source, src, 4);
		struct R20_event e = { e.header = eh };
		nn_freemsg(buf);
		return e;
	}
}

void med() {
	void *buf = NULL;
	int nbytes = nn_recv(subscriber, &buf, NN_MSG, 0);
	if (nbytes < 0) {
		R20_killSignal = -1;
		return;
	} else {
		void *msg = nn_allocmsg(nbytes, 0);
		strncpy(msg, buf, nbytes);
		int sbytes = nn_send(publisher, &msg, NN_MSG, 0);
		assert(nbytes == sbytes);
//		printf("%d-%d-%d\n", nbytes, sbytes, strlen(buf));
		nn_freemsg(buf);
	}
}

