#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

int eier;
int milch;
int mehl;
int bleche;
int run;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vorratAufgebraucht=PTHREAD_COND_INITIALIZER;
pthread_cond_t vorratAufgefuellt=PTHREAD_COND_INITIALIZER;


void backen(void){
    //Für Bäckerzwerg
    //Wenn nicht geweckt warten
    while(run){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&vorratAufgefuellt, &mutex);

		if(eier==5 && milch==1 && mehl==2){
			printf("            Bäcker: Alle Zutaten vorhanden. Backen beginnt..\n");
			eier=0;
			mehl=0;
			milch=0;
			bleche+=1;
			sleep(5);
			printf("            Bäcker: %d Blech(e) Kekse fertig!\n",bleche);
			//ALLE benachrichtigen
			pthread_cond_broadcast(&vorratAufgebraucht);
			if(bleche==10){
				printf("\n");
				printf("\n");
				printf("Gewünschte Anzahl an Blechen fertig!\n");
				run=0;
				break;
			}
			//Reseten der Zutaten
        } else {
			printf("            Bäcker: Es fehlen noch Zutaten. Warte ..\n");
		}
		pthread_mutex_unlock(&mutex);
	}
}

void eierAblegen(void){
	//Nur ein Zwerg in Stube
    
	while(run){
        sleep(1);
		pthread_mutex_lock(&mutex);
		printf("Eier: Anlieferung\n");
    
		if(eier<5){
			eier++;
			printf("Eier: Es sind %d Eier in der Backstube\n",eier);
		}
		if(eier==5){
			//Wenn Zutat voll wird Bäcker benachrichtigt
			printf("Eier: Voll\n");
			printf("Eier: Wecke Bäcker..\n");
			printf("Eier: Backstube verlassen\n");
			pthread_cond_signal(&vorratAufgefuellt);
			pthread_cond_wait(&vorratAufgebraucht, &mutex);
		} else {
			printf("Eier: Backstube verlassen\n");
		}
		//Freigeben
		pthread_mutex_unlock(&mutex);
		//Ein Ei pro Minute
    }
}

void mehlAblegen(void){
	//Nur ein Zwerg in Stube
    
	while(run){
		sleep(3);
		//pthread_cond_wait(&vorratAufgebraucht, &mutex);
		pthread_mutex_lock(&mutex);
		printf("    Mehl: Anlieferung \n");
		if(mehl<2){
			mehl++;
			printf("    Mehl: Es sind %d Pck Mehl in der Backstube\n",mehl);
		}
		if(mehl==2){
			//Wenn Zutat voll wird Bäcker benachrichtigt
			printf("    Mehl: Voll!\n");
			printf("    Mehl: Wecke Bäcker..\n");
			printf("    Mehl: Backstube verlassen\n");
			pthread_cond_signal(&vorratAufgefuellt);
			pthread_cond_wait(&vorratAufgebraucht, &mutex);
		} else {
			printf("    Mehl: Backstube verlassen\n");
		}
		//Freigeben
		pthread_mutex_unlock(&mutex);
		//Mehl alle drei Minuten
	}
}

void milchAblegen(void){
    
	while(run){
		sleep(2);
		//Nur ein Zwerg in Stube
		pthread_mutex_lock(&mutex);
		milch=1;;
		printf("        Milch: Anlieferung\n");
		printf("        Milch: Es ist eine Milch in der Backstube\n");
		printf("        Milch: Voll!\n"); 
		printf("        Milch: Wecke Bäcker..\n");
		printf("        Milch: Backstube verlassen\n");
		pthread_cond_signal(&vorratAufgefuellt);
		pthread_cond_wait(&vorratAufgebraucht, &mutex);
		pthread_mutex_unlock(&mutex);
		//Milch alle zwei Minuten
	}
}

int main(int argc, char *argv[])
{
	printf("baeckerei.c\n");
	
	eier=0;
	mehl=0;
	milch=0;
	bleche=0;
	run=1;
    
	pthread_t baeckerMeister, baeckerMeister2, eierZwerg, milchZwerg, mehlZwerg;
    
	if(pthread_create(&baeckerMeister, NULL, (void*)&backen, NULL)!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&baeckerMeister2, NULL, (void*)&backen, NULL)!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&eierZwerg, NULL, (void*)&eierAblegen, NULL)!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&milchZwerg, NULL, (void*)&milchAblegen, NULL)!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
	if(pthread_create(&mehlZwerg, NULL, (void*)&mehlAblegen, NULL)!=0){
		printf("ERROR creating thread!");
		return EXIT_FAILURE;
	}
    
	//Waiting for running threads
	pthread_join(baeckerMeister, NULL);
	pthread_join(baeckerMeister2, NULL);
	pthread_join(eierZwerg, NULL);
	pthread_join(mehlZwerg, NULL);
	pthread_join(milchZwerg, NULL);
    
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&vorratAufgebraucht);
	pthread_cond_destroy(&vorratAufgefuellt	);

	return EXIT_SUCCESS;
}
