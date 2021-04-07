#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <sys/types.h>
#include <string.h>
#include <signal.h>

#include "fifo.h"
#include "pipe.h"
#include "request.h"


#define MAX_READING_SIZE 1000     //Taille maximale de lecture dans le tupe

#define CLIENT_OPTIONS "hc:a:"    //Liste des options utilisés au niveau de la fonction getopt()

#define HELP_OPTION 'h'           //Option d'affichaque de l'aide du programme  

#define REQ_COMMAND_OPTION 'c'    //Option de précision de la commande à exécuter
                                  //au niveau du serveur
                                    
#define REQ_ARGUMENTS_OPTION 'a'  //Option de précision des arguments de la commande


/**
  Permet la récupération des paramètres au niveau de la commande.
  Entraîne l'arrêt du programme lorque le paramètre -c obligatoire est manquant
 */
void getOptions(int argc, char **argv);


/**
  Affiche l'aide en cas de mauvaise utilisation de la commande
  */
void printUsage();

/**
  Enregistre l'action à exécuter lorsque le signal SIGUSR1 est reçu par le cient.
  Il entraîne l'arrêt du client
  */ 
void gestionnaire(int signum);

/**
  Lance l'envoie d'une requête au serveur à travers un élément de type requête
   qu'il remplie et enfile dans la file synchronisée
 */
void sendRequest(char *cmd, char *args, char *pipeName);


/**
  Permet au client l'attente de la réponse du serveur en cas de réponse favorable
  Elle ouvre le tube de réponse en lecture en remplacement de la sortie standard
    et attend une entrée au niveau du serveur
  Une fois la réponse reçu elle est affichée sur l'entrée standard du client
  et referme le tube
  */
void waitResponse(char *pipeName);



void printHelp();

#endif /* __CLIENT_H__*/
