/*
	Copyright (C) 2013 - 2016 CurlyMo

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

typedef enum hwtype_t {
	HWINTERNAL = -1,
	NONE = 0,
	RF433,
	RF868,
	RFIR,
	ZWAVE,
	SENSOR,
	HWRELAY,
	API
} hwtype_t;

typedef enum {
	COMNONE = 0,
	COMOOK,
	COMPLSTRAIN,
	COMAPI
} communication_t;

#include <pthread.h>
#include "../core/options.h"
#include "../core/json.h"
#include "../core/threadpool.h"
#include "defines.h"

typedef struct rawcode_t {
	int pulses[MAXPULSESTREAMLENGTH];
	int length;
} rawcode_t;

typedef struct hardware_t {
	char *id;
	// unsigned short wait;
	// unsigned short stop;
	// pthread_mutex_t lock;
	// pthread_cond_t signal;
	// pthread_mutexattr_t attr;
	// unsigned short running;
	hwtype_t hwtype;
	communication_t comtype;
	struct options_t *options;

	int minrawlen;
	int maxrawlen;
	int mingaplen;
	int maxgaplen;

	unsigned short (*init)(void *(*)(void *));
	unsigned short (*deinit)(void);
	union {
		int (*receiveOOK)(void);
		void *(*receiveAPI)(void *param);
		int (*receivePulseTrain)(struct rawcode_t *r);
	};
	union {
		int (*sendOOK)(int *code, int rawlen, int repeats);
		int (*sendAPI)(struct JsonNode *code);
	};
	int (*gc)(void);
	unsigned short (*settings)(JsonNode *json);
	struct hardware_t *next;
} hardware_t;


extern struct hardware_t *hardware;

void hardware_init(void);
void hardware_register(struct hardware_t **hw);
void hardware_set_id(struct hardware_t *hw, const char *id);
int hardware_gc(void);

#endif