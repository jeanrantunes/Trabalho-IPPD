#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include "mpi.h"

#define NUM_CORES sysconf(_SC_NPROCESSORS_ONLN)
#define RADIUS 1

enum boolean {
    true = 1, 
    false = 0
};
typedef  enum boolean  bool;

float x, y; //coordenadas
float coord;
int circle;
long shots;
int cores;
pthread_mutex_t mutex;

bool piMonteCarlo(){
	x = ((double)rand())/((double)RAND_MAX);
	y = ((double)rand())/((double)RAND_MAX);
	coord = (x * x) + (y * y); 
	if(coord <= RADIUS)
		return true;
		
	return false;	
	
}
void openMP(){
	int i;
	#pragma omp parallel for num_threads(cores)
		for(i =0; i<shots; i++) {
			if(piMonteCarlo())
				circle++;
			
	}
}
void *calculaPi(void *arg){ //calcula Pi pelas threads
	int i;
	for(i =0; i<shots/cores; i++) {
		pthread_mutex_lock(&mutex);
		if(piMonteCarlo())
			circle++;			
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
	
	return 0;
}
void pthread(){
	int i;
	pthread_t threads[cores];
	
	pthread_mutex_init(&mutex,NULL);
	
	for(int i = 0; i < cores; i++)
		pthread_create(&(threads[i]), NULL, &calculaPi, NULL);

	for(int i = 0; i < cores; i++)
		pthread_join(threads[i], NULL);

}
void sequencial(){ 
	int i;
	for(i =0; i<shots; i++) {	
		if(piMonteCarlo())
			circle++;
		}
}
int main(int argc, char *argv[]){
	int choice;
	int count=0;
	double pi;
	int myrank,size,proc;
	MPI_Status status;
	int master =0;
	int tag = 1;             
    cores = NUM_CORES;  //por default numero de cores do processador
	if (argc <=2) {
		printf("Argumentos insuficientes!\n");
		printf("Entre com argumentos:\n1 - [executavel] \n2 - [numero de iterações] \n3 - [abordagem: (0 - sequencial, 1 - openMP, 2 - pthread]) \n4 - (opcional)[numero de threads]\n");      
		exit(0);
	}
	
	shots = atoi(argv[1]); //tiros
	choice = atoi(argv[2]); //abordagem 
	if(argv[3])
		cores = atoi(argv[3]); //numero de cores do processador(opcional), por default numero de cores do processador
	
	srand(time(0)); //semente
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	switch(choice){
		case(0): 
			sequencial();
			break;
		case(1): 
			openMP();
			break;
		case(2): 
			pthread();
			break;
	}
	if (myrank ==0) {
		count = circle;
		for (proc=1; proc<size; proc++) {
			MPI_Recv(&circle,1,MPI_REAL,proc,tag,MPI_COMM_WORLD,&status);
			count +=circle;        
		}
		pi = 4.0 * (((double)count)/ ((double)shots*size));
		printf("\n%f\n",pi);
	}
	else { 
		MPI_Send(&circle,1,MPI_REAL,master,tag,MPI_COMM_WORLD);
	}
   
	MPI_Finalize();             
	return 0;
}
