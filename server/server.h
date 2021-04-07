#ifndef __SERVER_H__
#define __SERVER_H__

#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include "fifo.h"
#include "analyse.h"
#include "request.h"


typedef struct {
  int th_state;
  request req;
} th_data;

void initServer(th_data data[]);



void gestionnaire(int signum);



void declineRequest(request *req);



void sendResponse(th_data *data);



void respondRequest(request *req, th_data *data, pthread_t th_id);



int errnum;

fifo *fifo_ptr;

#endif /* __SERVER_H__*/
