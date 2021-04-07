
/*
Il s'agit ici de l'ensemble des fonctions permettant la gesttion du tube
  nommé par le client
*/

#ifndef __PIPE_H__
#define __PIPE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/*
  Permet la création du tube de réponse, dans le dossier courrant du client
  le nom du tube a créé est passé en paramètre
*/
void createPipe(char *pipeName);

/*
  Permet la suppression du tube de réponse créé sur le système de fichier,
  dans le dossier courrant du client le nom du tube est passé en paramètre.
*/
void disposePipe(char *pipeName);


/*
  Ouvre un descripteur de fichier en lecture sur le fichier de nom contenu
  dans pipeName et le retourne
*/
int openPipe(char *pipeName);


/*
  Ferme le descripteur de fichier d'identifiant pipe_fd
*/
void closePipe(int pipe_fd);

#endif /* __PIPE_H__ */
