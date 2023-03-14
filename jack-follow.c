#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <jack/jack.h>


#define LENGTH(X) (sizeof X / sizeof X[0])

struct pair {
	const char *lead;
	const char *mirror;
};

enum evtype {
	CONNECT,
	DISCONNECT,
	NOP
};

struct event {
	char aname[128];
	char bname[128];
	enum evtype action;
};

struct evbuffer {
	int i;
	int pending;
	struct event evs[64];
};

// global vars
#include "config.h"

jack_client_t *client;

struct evbuffer queue;

void new_event(const char *aname, const char *bname, enum evtype action) {
	strncpy(queue.evs[queue.i].aname, aname, LENGTH(queue.evs[queue.i].aname)-1);
	strncpy(queue.evs[queue.i].bname, bname, LENGTH(queue.evs[queue.i].bname)-1);
	queue.evs[queue.i].action = action;

	char *ctype;
	if (action == CONNECT) {
		ctype = "->";
	} else {
		ctype = "xx";
	}
	printf("req: %s %s %s\n", queue.evs[queue.i].aname, ctype, queue.evs[queue.i].bname);

	queue.i = (queue.i+1) % LENGTH(queue.evs);
	queue.pending++;
	return;
}

int
process_handler(jack_nframes_t nframes, void *arg)
{
	// just return if nothing to be done
	if (!queue.pending) {
		return 0;
	}

	for (int i = 0; i<queue.pending; i++) {
		if (queue.evs[i].action == NOP) {
			continue;
		} else if (queue.evs[i].action == CONNECT) {
			jack_connect(client, queue.evs[i].aname, queue.evs[i].bname);
			queue.evs[i].action = NOP;
		} else if (queue.evs[i].action == DISCONNECT) {
			jack_disconnect(client, queue.evs[i].aname, queue.evs[i].bname);
			queue.evs[i].action = NOP;
		}
	}
	queue.pending = 0;
	queue.i = 0;

    return 0;
}

void
handle_connection(jack_port_id_t a, jack_port_id_t b, int connect, void *arg)
{
	jack_port_t *aport = jack_port_by_id(client, a);
	const char *aname = jack_port_name(aport);

	jack_port_t *bport = jack_port_by_id(client, b);
	const char *bname = jack_port_name(bport);

	char *ctype;
	enum evtype action;

	if (connect > 0) {
		ctype = "->";
		action = CONNECT;
	} else {
		ctype = "xx";
		action = DISCONNECT;
	}

	for (int i=0; i<LENGTH(pairs); i++) {
		if (strcmp(bname, pairs[i].lead) == 0) {
			printf("saw: %s %s %s\n", aname, ctype, bname);
			new_event(aname, pairs[i].mirror, action);
		}
	}

	return;
}

int
main()
{
	// setup event queue;
	queue.i = 0;
	for (int i=0; i<LENGTH(queue.evs); i++) {
		queue.evs[i].action = NOP;
	}
	
	client = jack_client_open("jack-follow", JackNullOption, 0);	
	if (client == NULL) {
		fprintf(stderr, "can't open client.\n");
		return 1;
	}

	jack_set_port_connect_callback(client, handle_connection, 0);
	jack_set_process_callback(client, process_handler, 0);
	jack_activate(client);

	sleep(-1);
	
	// not reached
	jack_client_close(client);
	return 0;
}
