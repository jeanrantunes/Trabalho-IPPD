# Trabalho-IPPD
## Execução tarefa A:
	$ make
	./Executável Nºtiros Abordagem (opcional)Nºthreads 
## Execução exec.sh para tarefa A:
	$ chmod 777 exec.sh
	$ ./exec.sh N°tiros Abordagem N°threads
	Gera um arquivo saida(N°tiros).txt com 30 execuções.
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
