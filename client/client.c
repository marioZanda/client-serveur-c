#include "client.h"

char *cmd = NULL;
char *args = "";


/**
  Fonction principale de l'exécution du client.
  Elle enregistre l'action d'arrêt au niveau du client en cas de signal SIGUSR1
  Récupère les paramètres du programme
  Le chemein absolu jusqu'au dossier courant est aussi récupéré afin de permetrre 
    de faire communiquer le client et le serveur par le tube de réponse même si
    les programmes sont exécuter dans des dossiers différents
  Les paramètres de la requête sont ensuite envoyés au serveur et le client
   attend sa réponse 
  */
int main(int argc, char **argv) {
  char pipe_name[REQ_PIPE_NAME_SIZE];

  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_flags = 0;

  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }

  // On associe l'action à SIGINT
  if (sigaction(SIGUSR1, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  
  getOptions(argc, argv);
  
  char dirPath[1000];
  
  realpath(".", dirPath);
  
  sprintf(pipe_name, "%s/%s_%d", dirPath, REQ_PIPE_NAME_PATTERN, getpid());

  createPipe(pipe_name);

  sendRequest(cmd, args, pipe_name);

  waitResponse(pipe_name);

  return 0;
}





void getOptions(int argc, char **argv) {

  int option;

  if (argc > 5 || argc < 2) {
    printUsage();
    exit(EXIT_FAILURE);
  }

  while ((option = getopt(argc, argv, CLIENT_OPTIONS)) != -1) {

    switch (option) {

    case HELP_OPTION:
      printHelp();
      break;
      
    case REQ_COMMAND_OPTION:
      cmd = optarg;
      break;
    case REQ_ARGUMENTS_OPTION:
      args = optarg;
      break;
      
    default:
      printUsage();
      exit(EXIT_SUCCESS);
    }
  }
  

  if (cmd == NULL) {
    printUsage();
    exit(EXIT_FAILURE);
  }

  if ((strlen(cmd) + strlen(args) > (REQ_CMD_SIZE + REQ_ARGS_SIZE))) {
    printf("Too big parameters\n");
    exit(EXIT_FAILURE);
  }
}




void printUsage() {
  execlp("head", "head", "-n", "1", "./help");
  exit(1);
}




void printHelp() {
  execlp("cat", "cat", "./help");
  exit(1);
}




void gestionnaire(int signum) {
  if (signum != SIGUSR1) {
    fprintf(stderr, "Wrong signal number\n");
    exit(EXIT_FAILURE);
  }
  printf("503 ERROR: Service Unavailable !\n");
  exit(EXIT_SUCCESS);
}



void sendRequest(char *cmd, char *args, char *pipeName) {
  request req;
  sprintf(req.cmd, "%s", cmd);
  sprintf(req.args, "%s", args);
  sprintf(req.pipeName, "%s", pipeName);
  req.pid = getpid();
  
  enfiler((char *) &req, get_fifo(FIFO_SIZE), MAX_THREAD_NB, sizeof(req));
}



void waitResponse(char *pipeName) {
  ssize_t n;
  int pipe_fd;
  char string[MAX_READING_SIZE];

  pipe_fd = openPipe(pipeName);

  disposePipe(pipeName);

  printf("Response : \n");
  while ((n = read(pipe_fd, &string, sizeof(string))) > 0) {
    string[n] = '\0';
    printf("%s\n", string);
  }

  if (n == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  closePipe(pipe_fd);
  
  exit(EXIT_SUCCESS);
}
