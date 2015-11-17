#!/bin/bash
for((i=0;i < 30; i++)); 
	do sleep 2 && ./tarefaA $1 $2 $3  >> saida$1.txt 
done
