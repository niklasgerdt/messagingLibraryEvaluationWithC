#include <zmq.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>

static void *context;
static void *publisher;
static void *subscriber;

static struct sigaction action;
int killSignal;

void initNetwork(){
	context = zmq_ctx_new();
	printf("zmq ctx up\n");
}

void initPublisher(const char *pub){
	printf("Binding publisher to %s\n", pub);
	publisher = zmq_socket(context, ZMQ_PUB);
	assert(zmq_bind(publisher, pub) == 0);
	printf("Publisher binded to %s\n", pub);
}

void initSubscriber(const char *sub){
	printf("Connecting subscriber to %s\n", sub);
	subscriber = zmq_socket(context, ZMQ_SUB);
	assert(zmq_connect(subscriber, sub) == 0);
	assert(zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0) == 0);
	printf("Subscriber connected to %s\n", sub);
}

void addSubscriber(const char *sub){
	printf("Connecting subscriber to %s\n", sub);
	assert(zmq_connect(subscriber, sub) == 0);
	assert(zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0) == 0);
	printf("Subscriber connected to %s\n", sub);
}

void destroyNetwork() {
	zmq_close(publisher);
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	printf("zmq pubsub down\n");
}

void mediate() {
	zmq_msg_t msgIn;
	zmq_msg_t msgOut;
	assert(zmq_msg_init(&msgIn) == 0);
	assert(zmq_msg_init(&msgOut) == 0);
	assert(zmq_msg_recv(&msgIn, subscriber, 0) != -1);
 	assert(zmq_msg_copy(&msgOut, &msgIn) == 0);
	assert(zmq_msg_send(&msgOut, publisher, 0) 
		== zmq_msg_size(&msgIn));
	zmq_msg_close(&msgIn);
}

void term(int signum) {
    printf("Received SIGTERM, exiting...\n");
    killSignal = 1;
}

void initTerminator(void){
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);
    printf("Set up term signal handler\n");
}

int main(int argc, char *argv[]) {
	char *pubAddr = argv[1];
	char *subAddr[4];
	for (int i = 1; i < argc; i++) {
		subAddr[i - 1] = argv[i + 1];
	}

	initNetwork();
	initPublisher(pubAddr);
	initSubscriber(subAddr[0]);
	int i = 1;
	while (subAddr[i] != NULL) {
		addSubscriber(subAddr[i]);
		i++;
	}

	initTerminator();

	while (killSignal == 0) {
		mediate();
	}

	destroyNetwork();
	return 0;
}