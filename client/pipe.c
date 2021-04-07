#include "pipe.h" 

void createPipe(char *pipeName) {
  if ((mkfifo(pipeName, S_IRUSR | S_IWUSR) == -1)) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }
}


int openPipe(char *pipeName) {

  int pipe_fd = open(pipeName, O_RDONLY);

  if (pipe_fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  
  return pipe_fd;
}

void disposePipe(char *pipeName) {
  if (unlink(pipeName) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
}

void closePipe(int pipe_fd) {

  if (close(pipe_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}
