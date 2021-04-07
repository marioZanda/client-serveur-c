/**
  Il s'git ici de l'implémentation d'une file synchronisée permettant 
    la gestion d'objets de taille dynamique en mémoire partagée
  Elle est défnie sur créaion d'une structure représentant la file et fournissant
    des fonctions pour interargir avec elle.
 */

#ifndef __FIFO_H__
#define __FIFO_H__

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


/**
  Définition de la strucute de file synchronisée qui utilise des sémaphores 
    pour la synchronisation
 */
typedef struct {
  sem_t mutex;      // Sémaphore pour la disponibilité de la file
  sem_t vide;       // Sémaphore pour gérer la suppression dans la file
  sem_t plein;      // Sémaphore pour gérer l'ajout dans la file
  int tete;         // Position d'ajout dans le tampon
  int queue;        // Position de suppression dans le tampon
  char buffer[];    // Le tampon contenant les données
} fifo;

#define SHM_NAME "fifo_shm"   //Nom de la mémoire partagée

/**
 
 Crée la mémoire partagée de taille fifoLen
 
  */
  
fifo *create_fifo(size_t fifo_lenght, size_t fifo_data_size);




static void init_fifo(fifo *fifo_p, size_t fifo_length);




fifo *get_fifo(size_t fifo_size) ;




void close_fifo(fifo *fifo_p);




void enfiler(void *data, fifo *fifo_p, size_t fifo_length, size_t fifo_data_size);




void defiler(void *data, fifo *fifo_p, size_t fifo_length, size_t fifo_data_size);

#endif /* __FIFO_H__ */
