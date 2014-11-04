#include "beanstalk.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../mod/event.h"
#include "../mod/util.h"
#include "../pubsub.h"

static char channel[10];
static int publisher;
static int subscriber;

void initPub(const char *_addr, const char *_channel) {
	strcpy(channel, _channel);
	char del[1] = ":";
	char addr[20];
	strcpy(addr, _addr);
	char *host = strtok(addr, del);
	char *port = strtok(NULL, del);
	printf("Binding pub %s:%d\n", host, atoi(port));
	publisher = bs_connect(host, atoi(port));
	assert(publisher != BS_STATUS_FAIL);
	if (channel[0] != 'N') {
		printf("using tube (%s)\n", channel);
		assert(bs_use(publisher, channel) == BS_STATUS_OK);
	} else {
		printf("using tube (%s)\n", "default");
	}
	printf("Pub bind to %s:%s\n", _addr, channel);
}

void destroyPub() {
	bs_disconnect(publisher);
	printf("pub down\n");
}

void initSub(const char *_addr, const char *_channel) {
	strcpy(channel, _channel);
	char del[1] = ":";
	char addr[20];
	strcpy(addr, _addr);
	char *host = strtok(addr, del);
	char *port = strtok(NULL, del);
	printf("Connecting sub %s:%d\n", host, atoi(port));
	subscriber = bs_connect(host, atoi(port));
	assert(subscriber != BS_STATUS_FAIL);
	for (int i = 0; i < strlen(channel); i++) {
		if (channel[i] == 'N') {
			printf("tube (%s)\n", channel);
			break;
		} else {
			printf("using tube %c\n", channel[i]);
			char *c = "N\0";
			c[0] = channel[i];
//			memcpy(c, channel[i], 1);
			printf("tubing!!! %s\n", c);
			assert(bs_watch(subscriber, c) == BS_STATUS_OK);
			assert(bs_ignore(subscriber, "default") == BS_STATUS_OK);
		}
	}
}

void addSub(const char *addr) {
}

void destroySub() {
	bs_disconnect(subscriber);
	printf("Sub down\n");
}

void pub(struct R20_event e, size_t size) {
	char msgStr[size + 1];
	sprintf(msgStr, "%c;%ld;%lld.%09ld;%d;%s\0", e.header.source, e.header.id, e.header.created.tv_sec,
			e.header.created.tv_nsec, e.dataLength, e.data);
	int id = bs_put(publisher, 0, 0, 60, msgStr, strlen(msgStr));
}

struct R20_event sub() {
	BSJ *job;
	struct R20_event en;
	int rc = bs_reserve(subscriber, &job);
	if (rc == BS_STATUS_OK) {
		assert(job);
		char src;
		long id;
		long int cSec;
		long int cNsec;
		sscanf(job->data, "%c;%ld;%lld.%09ld", &src, &id, &cSec, &cNsec);
		struct timespec t = { .tv_sec = cSec, .tv_nsec = cNsec };
		struct R20_eventHeader eh = { .source = src, .id = id, .created = t };
		struct R20_event e = { e.header = eh };
		printf("reserve job id: %d size: %lu\n", job->id, job->size);
		assert(bs_delete(subscriber, job->id) == BS_STATUS_OK); // TODO: do we need this?
		bs_free_job(job);
		return e;
	} else {
		return en;
	}
}

void med() {
}
