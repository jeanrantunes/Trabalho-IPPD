#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

#define NUM_CORES sysconf(_SC_NPROCESSORS_ONLN)
#define RADIUS 1

float x, y; //coordenadas
float coord;
double pi;
long circle = 0;
long total = 0;
long shots;
pthread_mutex_t mutex;
using namespace std;

bool piMonteCarlo(){
	
	x = (double) (rand() / double(RAND_MAX));
	y = (double) (rand() / double(RAND_MAX));
	coord = (x * x) + (y * y); 
	if(coord <= RADIUS)
		return true;
	return false;	
	
}
double openMP(){
	#pragma omp parallel for num_threads(NUM_CORES)
		for(int i =0; i<shots; i++) {
			if(piMonteCarlo())
				circle++;
			total++;
		}
	pi = 4.0 * (((double)circle)/ ((double)total));
	return pi;	

}
void *calculaPi(void *arg){ //calcula Pi pelas threads
	for(int i =0; i<shots; i++) {
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
	pthread_t threads[NUM_CORES+6];
	
	pthread_mutex_init(&mutex,NULL);
	
	for(int i = 0; i < NUM_CORES+6; i++)
		pthread_create(&(threads[i]), NULL, &calculaPi, NULL);

	for(int i = 0; i < NUM_CORES+6; i++)
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
	if (argc!=3){
		cout << "Argumentos insuficientes";
		exit(0);
	}
	srand(time(0));
	
	shots = atoi(argv[1]);
	choice = atoi(argv[2]);
	
	switch(choice){
		case(0): 
			cout << sequencial();
			return 0;
		case(1): 
			cout << openMP();
			return 0;
		case(2): 
			cout << pthread();
			return 0;
	}
}

