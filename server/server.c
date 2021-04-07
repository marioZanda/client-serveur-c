#include "server.h"

int main(void) {

  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_flags = SA_RESTART;

  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < 32; ++i) {
    if (i != SIGKILL && i != SIGCHLD && i != SIGSEGV && i != SIGSTOP &&
        sigaction(i, &action, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  }

  th_data data[MAX_THREAD_NB];
  fifo *fifo_p = create_fifo(MAX_THREAD_NB, sizeof(request));
  
  fifo_ptr = fifo_p;
  
  pthread_t threads[MAX_THREAD_NB];

  initServer(data);
  
  
  while (1) {
      
    int i = 0;
    request req; 
    defiler(&req, fifo_p, MAX_THREAD_NB, sizeof(req));
    
    while (i < MAX_THREAD_NB) {

      if (data[i].th_state == 1) {

        printf("Request \"%s %s\" treat by thread %d\n", req.cmd,
               req.args, i + 1);

        respondRequest(&req, &data[i], threads[i]);

        break;
      }

      i++;
    }

    if (i == MAX_THREAD_NB) {
      declineRequest(&req);
    }
  }

  return 0;
}

void *treatRequest(void *arg) {
  pthread_t th = pthread_self();
  th_data *data = ((th_data *)arg);

  if (pthread_detach(th) != 0) {
    perror("pthread detach ");
    exit(EXIT_FAILURE);
  }

  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
    break;

  case 0:
    sendResponse(data);
    break;

  default:
    wait(NULL);
    data->th_state = 1;
    printf("Response for request \"%s %s\" sent \n", data->req.cmd, data->req.args);
    break;
  }
  return 0;
}

void gestionnaire(int signum) {
  printf("\n Server shutdown ! \n");
  
  close_fifo(fifo_ptr);
  
  exit(EXIT_SUCCESS);
}

void initServer(th_data data[]) {
  for (int i = 0; i < MAX_THREAD_NB; i++) {
    data[i].th_state = 1;
  }
  printf("Server started !\n");
}

void respondRequest(request *req, th_data *data, pthread_t th_id) {

  data->th_state = 0;
  data->req = *req;

  if ((errnum = pthread_create(&th_id, NULL, treatRequest, data)) != 0) {
    fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
}

void declineRequest(request *req) {
  printf("Request \"%s %s\" Rejected !\n", req->cmd,
         req->args);
  kill(req->pid, SIGUSR1);
}

void sendResponse(th_data *data) {
  int pipe_fd;
  pipe_fd = open(data->req.pipeName, O_WRONLY);

  if (pipe_fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  dup2(pipe_fd, STDOUT_FILENO);

  if (close(pipe_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  char command[REQ_ARGS_SIZE + REQ_CMD_SIZE];

  sprintf(command, "%s %s", data->req.cmd, data->req.args);
  execvp(data->req.cmd, analyse_arg(command));
}


