#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <sys/types.h>

#define REQ_CMD_SIZE 250
#define REQ_ARGS_SIZE 250
#define REQ_PIPE_NAME_PATTERN "client_pipe"
#define REQ_PIPE_NAME_SIZE 2000


#define MAX_THREAD_NB 10      //Nombre de threads gérable par le serveur
 


typedef struct {
  char cmd[REQ_CMD_SIZE];
  char args[REQ_ARGS_SIZE];
  char pipeName[REQ_PIPE_NAME_SIZE];
  pid_t pid;
} request;


#define FIFO_SIZE (MAX_THREAD_NB * sizeof(request))


#endif /* __REQUEST_H__ */
