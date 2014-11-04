#ifndef PUBSUB_H_
#define PUBSUB_H_

#include "mod/event.h"

void initPub(const char *address, const char *channel);

void pub(struct R20_event e, size_t size);

void med();

void destroyPub(void);

struct R20_event sub();

void initSub(const char *address, const char *channel);

void addSub(const char *address);

void destroySub(void);

#endif /* PUBSUB_H_ */
