#include <zmq.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../mod/event.h"
#include "../pubsub.h"

static void *context;
static void *publisher;
static void *subscriber;
static char channel[10];

void initPub(const char *addr, const char *_channel) {
	strcpy(channel, _channel);
	context = zmq_ctx_new();
	publisher = zmq_socket(context, ZMQ_PUB);
	assert(zmq_bind(publisher, addr) == 0);
	printf("Pub bind to %s:%s\n", addr, channel);
}

void destroyPub() {
	zmq_close(publisher);
	zmq_ctx_destroy(context);
	printf("pub down\n");
}

void initSub(const char *addr, const char *_channel) {
	strcpy(channel, _channel);
	printf("Connection sub %s:%s\n", addr, channel);
	context = zmq_ctx_new();
	subscriber = zmq_socket(context, ZMQ_SUB);
	assert(zmq_connect(subscriber, addr) == 0);
	for (int i = 0; i < strlen(channel); i++) {
		if (channel[i] == 'N') {
			printf("no subscription (%s)\n", channel);
			assert(zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0) == 0);
			break;
		} else {
			printf("subscribed to channel %c\n", channel[i]);
			char c[1] = { channel[i] };
			assert(zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, c, 1) == 0);
		}
	}
	printf("Sub connected to %s:%s\n", addr, channel);
}

void addSub(const char *addr) {
	assert(zmq_connect(subscriber, addr) == 0);
	printf("Sub connected to %s:%s\n", addr, channel);
}

void destroySub() {
	zmq_close(subscriber);
	if (context != NULL) {
		zmq_ctx_destroy(context);
	}
	printf("Sub down\n");
}

void pub(struct R20_event e, size_t size) {
	zmq_msg_t msgChn;
	assert(zmq_msg_init_size(&msgChn, strlen(channel)) == 0);
	memcpy(zmq_msg_data(&msgChn), channel, strlen(channel));
	zmq_msg_t msg;
	assert(zmq_msg_init_size(&msg, size) == 0);
	char msgStr[size];
	sprintf(msgStr, "%4s;%ld;%lld.%09ld;%d;%s", e.header.source, e.header.id, e.header.created.tv_sec,
			e.header.created.tv_nsec, e.dataLength, e.data);
	memcpy(zmq_msg_data(&msg), msgStr, strlen(msgStr));
	assert(zmq_msg_send(&msgChn, publisher, ZMQ_SNDMORE) == strlen(channel));
	assert(zmq_msg_send(&msg, publisher, 0) == size);
}

struct R20_event sub() {
	struct R20_event e;
	zmq_msg_t msg;
	assert(zmq_msg_init(&msg) == 0);
	if (zmq_msg_recv(&msg, subscriber, 0) == -1) {
		return e;
	}
	if (!zmq_msg_more(&msg)) {
		char msgStr[zmq_msg_size(&msg)];
		memcpy(msgStr, zmq_msg_data(&msg), zmq_msg_size(&msg));
		char src[4];
		long id;
		long int cSec;
		long int cNsec;
		sscanf(msgStr, "%4s;%ld;%lld.%09ld", src, &id, &cSec, &cNsec);
		struct timespec t = { .tv_sec = cSec, .tv_nsec = cNsec };
		struct R20_eventHeader eh = { .source = "XXXX\0", .id = id, .created = t };
		strncpy(eh.source, src, 4);
		struct R20_event e = { e.header = eh };
		zmq_msg_close(&msg);
		return e;
	} else {
		return sub();
	}
}

void med() {
	zmq_msg_t msg_in;
	assert(zmq_msg_init(&msg_in) == 0);
	if (zmq_msg_recv(&msg_in, subscriber, 0) == -1) {
		return;
	}
	zmq_msg_t msg_out;
	assert(zmq_msg_init_size(&msg_out, zmq_msg_size(&msg_in)) == 0);
	memcpy(zmq_msg_data(&msg_out), zmq_msg_data(&msg_in), zmq_msg_size(&msg_in));
	if (!zmq_msg_more(&msg_in)) {
		assert(zmq_msg_send(&msg_out, publisher, 0) == zmq_msg_size(&msg_in));
		zmq_msg_close(&msg_in);
	} else {
		assert(zmq_msg_send(&msg_out, publisher, ZMQ_SNDMORE) == zmq_msg_size(&msg_in));
		zmq_msg_close(&msg_in);
		med();
	}
}
