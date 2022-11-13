#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <ctype.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <math.h>

#define FICHIER1 "client" 
#define FICHIER2 "serveur" 

#define PROJET 1
#define LG_MAX 512
struct msgform {
	long mtype;
	char mtext[ LG_MAX ];
} msg;
int is_number(char c){
	return ('0'<=c)&&(c<='9');
}
float char_to_num(char number[20]){
	int i=0, second=0;
	float num1=0,num2=0;
	
	while(is_number(number[i])){
		num1 = (num1 * 10)+ ((float)(number[i]-'0'));
		i++;
	}
	while(number[i]!='\0'){
		if(is_number(number[i])) num2 = (num2 * 10)+ ((float)(number[i]-'0'));
		i++;
	}
	
	return num1+num2;
}
// inverser une chaine 
void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

// convertir u un entier vers une chaine de caracteres 
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0'; 
		x = x / 10;
	}

	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
		str[i++] = '0';
    // ex: entier = 123  => str = 321 
    // c'est pour cela on inverse la chaine
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Convertir une reel en une chaine de caracteres 
void ft(float n, char* res, int apres_virgule)
{
	// Extracter la partie entier
	int part_entier = (int)n;

	// Extracter la partie flotante
	float part_flotant = n - (float)part_entier;

	// convertir la partie entier en une chaine 
	int i = intToStr(part_entier, res, 0);

	if (apres_virgule != 0) {
		res[i] = '.'; // add dot

		// ex: part_flotant= 0.123 apres_virgule=2  => 12.3 => str= 123.12
		part_flotant = part_flotant * pow(10, apres_virgule);
		intToStr((int)part_flotant, res + i + 1, apres_virgule);
	}
	return;
}
int main(void) { 
	int res, i;
	key_t clef_requetes, clef_reponses ;
	int frequete, freponse;
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
	while(1){
		
		printf("serveur...\n");
		res = msgrcv(frequete, & msg, LG_MAX, 0, 0); 
		if (res == -1) {
		       	perror("msgrcv"); 
			exit(0); 
		}
		if(strcmp(msg.mtext,"exit")==0){
			printf("server shutdown\n");
			exit(0);
		}
		float somme =char_to_num(msg.mtext);
		ft(somme,msg.mtext,2);
		res = msgsnd(freponse, & msg, strlen(msg.mtext) + 1, 0);
		if (res == -1) { 
			perror("msgsnd");
		       	exit(0);
	       	}
       	}
       	return (EXIT_SUCCESS);
}


