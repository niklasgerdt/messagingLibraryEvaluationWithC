#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include "util.h"

#define SECONDINNANOS 1000000000

struct timespec R20_currentTime() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts;
}

//busy wait, because operating systems do not support pausing in nanosecond precision.
static void pauseNanos(struct timespec t) {
	struct timespec now = R20_currentTime();
	if (t.tv_sec > now.tv_sec) {
		pauseNanos(t);
	} else if (t.tv_sec == now.tv_sec && t.tv_nsec > now.tv_nsec) {
		pauseNanos(t);
	}
}

void R20_pause(long pause) {
	assert(pause < SECONDINNANOS);
	struct timespec t = R20_currentTime();
	if (t.tv_nsec + pause >= 1000000000) {
		t.tv_sec++;
		t.tv_nsec = t.tv_nsec + pause - SECONDINNANOS;
	} else {
		t.tv_nsec += pause;
	}
	pauseNanos(t);
}

static struct sigaction action;

void term(int signum) {
    printf("Received SIGTERM, exiting...\n");
    R20_killSignal = 1;
}

void R20_initTerminator(void){
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);
    printf("Set up term signal handler\n");
}


