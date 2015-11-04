# Trabalho-IPPD
## Execução tarefa A:
	$ make
	./Executável Nºtiros Abordagem (opcional)Nºthreads  
## Execução tarefa B:
	$ make
	mpirun Executável Nºtiros Abordagem (opcional)Nºthreads ||
	mpirun -c Nºnós_virtuais Executável Nºtiros Abordagem (opcional)Nºthreads
### Abordagem: 
  0 - Sequêncial;
  1 - OpenMP;
  2 - Pthread;
### Obs:
	Por default o número de threads é o número de cores do processador.
