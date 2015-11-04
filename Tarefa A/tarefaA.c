#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

#define NUM_CORES sysconf(_SC_NPROCESSORS_ONLN)
#define RADIUS 1
enum boolean {
    true = 1, 
    false = 0
};
typedef  enum boolean  bool;

float x, y; //coordenadas
float coord;
double pi;
long circle = 0;
long total = 0;
long shots;
pthread_mutex_t mutex;
int cores;

bool piMonteCarlo(){
	
	x = ((double)rand())/((double)RAND_MAX);
	y = ((double)rand())/((double)RAND_MAX);
	coord = (x * x) + (y * y); 
	if(coord <= RADIUS)
		return true;
	return false;	
	
}
double openMP(){
	#pragma omp parallel for num_threads(cores)
		for(int i =0; i<shots; i++) {
			if(piMonteCarlo())
				circle++;
			total++;
		}
	pi = 4.0 * (((double)circle)/ ((double)total));
	return pi;	

}
void *calculaPi(void *arg){ //calcula Pi pelas threads
	for(int i =0; i<shots/cores; i++) {
		pthread_mutex_lock(&mutex);
		if(piMonteCarlo())
			circle++;			
		total++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
	
	return 0;
}
double pthread(){
	pthread_t threads[cores];
	
	pthread_mutex_init(&mutex,NULL);
	
	for(int i = 0; i < cores; i++)
		pthread_create(&(threads[i]), NULL, &calculaPi, NULL);

	for(int i = 0; i < cores; i++)
		pthread_join(threads[i], NULL);
	
		
	return pi = 4.0 * (((double)circle)/ ((double)total));
	
}

double sequencial(){ 
	for(int i =0; i<shots; i++) {
			
			if(piMonteCarlo())
				circle++;
			total++;
		}
	pi = 4 * (((double)circle)/ ((double)total));
	return pi;
}
int main(int argc,  char *argv[]){
	int choice;
	cores = NUM_CORES;
	if (argc<=2){
		printf("Argumentos insuficientes!\n");
		printf("Entre com argumentos:\n1 - [executavel] \n2 - [numero de iterações] \n3 - [abordagem: (0 - sequencial, 1 - openMP, 2 - pthread]) \n4 - (opcional)[numero de threads]\n");      
		exit(0);
	}
	srand(time(0));
	
	shots = atoi(argv[1]);
	choice = atoi(argv[2]);
	if(argv[3])
		cores = atoi(argv[3]);
	switch(choice){
		case(0): 
			printf("\n%f\n",sequencial());
			return 0;
		case(1): 
			printf("\n%f\n",openMP());
			return 0;
		case(2): 
			printf("\n%f\n",pthread());
			return 0;
	}
}

