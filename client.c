#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FICHIER1 "client"
#define FICHIER2 "serveur"

#define PROJET 1
#define LG_MAX 512

struct msgform {
	long mtype;
	char mtext[ LG_MAX ]; 
} msg;

int main(void) { 
	int res;
	int frequete, freponse;
	key_t clef_requetes, clef_reponses ;
	// La gestion des clés
	clef_requetes= ftok(FICHIER1, PROJET);
	if (clef_requetes == -1){
		perror("Problème pour obtenir la clé"); 
		exit(EXIT_FAILURE);
	}
	clef_reponses= ftok(FICHIER2, PROJET); 
	if (clef_reponses == -1){
		perror("Problème pour obtenir la clé"); 
		exit(EXIT_FAILURE);
	}
	// Création des files de messages
	frequete = msgget(clef_requetes, 0700 | IPC_CREAT);
	if (frequete == -1) { 
		perror("msgget"); 
		return (EXIT_FAILURE); 
	}
	freponse = msgget(clef_reponses, 0700 | IPC_CREAT);
	if (freponse == -1) { 
		perror("msgget");
	       	return (EXIT_FAILURE); 
	}
	do{
		msg.mtype = getpid();
		char car; // pour la lecture 
		int ind=0; 
		printf("client entre deux (a+b):\n");
		while((car=getchar())!='\n') msg.mtext[ind++]=car;			
			
		msg.mtext[ind]='\0';
		
		res = msgsnd(frequete, & msg, strlen(msg.mtext) + 1, 0);
       		if (res == -1) { 
			perror("msgsnd"); 
			exit(0); 
		}
		if(strcmp(msg.mtext,"exit")==0) {
			printf("client shutdown\n");
			exit(0);
		}
		res = msgrcv(freponse, & msg, LG_MAX, getpid(), 0); 
		if (res == -1) {
		       	perror("msgrcv"); 
			exit(0); 
		}
		printf("result : %s\n", msg.mtext); 
	}while(1);
	return (EXIT_SUCCESS);
}
