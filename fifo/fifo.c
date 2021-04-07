#include "fifo.h"





fifo *create_fifo(size_t fifo_length, size_t fifo_data_size) {
  // On commence par créer un segment de mémoire partagée
  int shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (shm_fd == -1) {
    perror("shm_open");
    exit(EXIT_SUCCESS);
  }

  if (ftruncate(shm_fd, (fifo_length * fifo_data_size) + sizeof(fifo)) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  fifo *shm_ptr =
      mmap(NULL, (fifo_length * fifo_data_size) + sizeof(fifo), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  init_fifo(shm_ptr, fifo_length);

  return shm_ptr;
}





static void init_fifo(fifo *fifo_p, size_t fifo_length) {
  // Initialisation des variables
  if (sem_init(&fifo_p->mutex, 1, 1) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  if (sem_init(&fifo_p->vide, 1, fifo_length) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  if (sem_init(&fifo_p->plein, 1, 0) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  fifo_p->tete = 0;
  fifo_p->queue = 0;
}





fifo *get_fifo(size_t fifo_size) {
  
  int shm_fd = shm_open(SHM_NAME, O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
  
  if (shm_fd == -1) {
    perror("503 ERROR : CAN'T REACH SERVER\n");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(shm_fd, fifo_size + sizeof(fifo)) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  fifo *shm_ptr =
      mmap(NULL, fifo_size + sizeof(fifo), PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }
  return shm_ptr;
}





void close_fifo(fifo *fifo_p) {

  if (shm_unlink(SHM_NAME) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
  
  // Il faudrait prendre des précautions particulières afin d'être sûr
  // que les sémaphores sont détruits à la fin du processus (gestion des signaux).
  if (sem_destroy(&fifo_p->mutex) == -1) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }
  if (sem_destroy(&fifo_p->plein) == -1) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }
  if (sem_destroy(&fifo_p->vide) == -1) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }
}






void enfiler(void *data, fifo *fifo_p, size_t fifo_length, size_t fifo_data_size) {
  if (sem_wait(&fifo_p->vide) == -1) {
    perror("sem_wait");
    exit(EXIT_FAILURE);
  }

  if (sem_wait(&fifo_p->mutex) == -1) {
    perror("sem_wait");
    exit(EXIT_FAILURE);
  }
  
  memcpy(((char *) fifo_p->buffer) + fifo_p->tete * fifo_data_size, (char *)data, fifo_data_size);
  fifo_p->tete = (fifo_p->tete + 1) % fifo_length;

  if (sem_post(&fifo_p->mutex) == -1) {
    perror("sem_post");
    exit(EXIT_FAILURE);
  }

  if (sem_post(&fifo_p->plein) == -1) {
    perror("sem_post");
    exit(EXIT_FAILURE);
  }
}

void defiler(void *data, fifo *fifo_p, size_t fifo_length, size_t fifo_data_size) {
  if (sem_wait(&fifo_p->plein) == -1) {
    perror("sem_wait");
    exit(EXIT_FAILURE);
  }

  if (sem_wait(&fifo_p->mutex) == -1) {
    perror("sem_wait");
    exit(EXIT_FAILURE);
  }

  memcpy((char *) data, fifo_p->buffer + fifo_p->queue * fifo_data_size, fifo_data_size);
  fifo_p->queue = (fifo_p->queue + 1) % fifo_length;

  if (sem_post(&fifo_p->mutex) == -1) {
    perror("sem_post");
    exit(EXIT_FAILURE);
  }

  if (sem_post(&fifo_p->vide) == -1) {
    perror("sem_post");
    exit(EXIT_FAILURE);
  }
}
