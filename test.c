#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){
	signal(SIGTERM,handler);
	int status;
	pid_t child2;
	pid_t child=fork(); //forks make a copy of variables
	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (child>0){
		child2=fork();
		if(child2>0){
			waitpid(child,&status,0);
			printf("parent  \n");
		}else{ //child2 process
			printf("child2 \n");
		}
	}
	else //child1 process
	{
		waitpid(child2,&status,0);  
		printf("child \n");
	}
	return 0;
}