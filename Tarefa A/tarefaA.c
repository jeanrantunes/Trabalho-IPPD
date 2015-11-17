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
long double pi;
int circle = 0;
int total = 0;
int shots;
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
	int i;
	#pragma omp parallel for num_threads(cores)
		for(i =0; i<shots; i++) {
			if(piMonteCarlo())
				circle++;
			total++;
		}

	pi = 4.0 * (((long double)circle)/ ((long double)total));
	return pi;	

}
void *calculaPi(void *arg){ //calcula Pi pelas threads
	int i;
	//int *n = (int*)arg;
	//printf("%d\n",n );
	for(i =0; i<shots; i++) {
		pthread_mutex_lock(&mutex);
		if(piMonteCarlo())
			circle++;			
		total++;
		pthread_mutex_unlock(&mutex);
	}
	
	return 0;
}
double pthread(){
	pthread_t threads[cores];
	pthread_mutex_init(&mutex,NULL);
	int i;

	for(i = 0; i < cores; i++)
		pthread_create(&(threads[i]), NULL, &calculaPi, &i);

	for(i = 0; i < cores; i++)
		pthread_join(threads[i], NULL);
	
	return pi = 4.0 * (((long double)circle)/ ((long double)total));
	
}

double sequencial(){ 
	int i;
	for(i =0; i<shots; i++) {
			
			if(piMonteCarlo())
				circle++;
			total++;
		}
	pi = 4 * (((long double)circle)/ ((long double)total));
	return pi;
}
int main(int argc,  char *argv[]){
	clock_t start,end;
	double tempo, result;
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
			start = clock();
			result = sequencial();
			end= clock();
			break;
		case(1): 
			start = clock();
			result = openMP();
			end= clock();
			break;
			
		case(2): 
			start = clock();
			result = pthread();
			end= clock();
			break;
			
		default:
			printf("Opção invélida.\n");
			break;

	
	}
	printf("%f\t%f\n", (tempo = (end-start)/(double)CLOCKS_PER_SEC), result);
}
