#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

int c=0;

void handler(int signumber){
	c++;
	printf("Signal with number %i has arrived %dX\n",signumber,c);
}

int main(){
	signal(SIGUSR1, handler);

	int status;
	int child1_pid;
	int child2_pid;
	int child3_pid;

	pid_t child3;
	pid_t child2;
	pid_t child1=fork();
	if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (child1>0){
		child2=fork();
		if(child2>0){
			child3=fork();
			if(child3>0){ //parent process
				//pause();
				
				printf("parent\n");
				printf("pid: %d\n\n",getpid());
				waitpid(child1,&status,0);
				waitpid(child2,&status,0);
				waitpid(child3,&status,0);
				//wait(&status);
			}else{ //child3 process
				
				printf("child3\n");
				printf("pid: %d\nppid: %d\n\n",getpid(),getppid());
				//kill(getppid(),SIGUSR1);

			}
		}else{ //child2 process
			printf("child2\n");
			printf("pid: %d\nppid: %d\n\n",getpid(),getppid());
			//kill(getppid(),SIGUSR1);
		}
	}else{ //child1 process
		printf("child1\n");
		printf("pid: %d\nppid: %d\n\n",getpid(),getppid());
		//kill(getppid(),SIGUSR1);
	}
	return 0;
}