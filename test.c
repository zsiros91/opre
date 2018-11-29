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

pid_t child3;
pid_t child2;
pid_t child1=fork();
if (child1<0){perror("The fork calling was not succesful\n"); exit(1);}
if (child1>0){
    child2=fork();
    if(child2>0){
            child3=fork();
            if(child3>0){ //parent process
                int status;
                while(c<3){
                	
                }
				sleep(5);
                printf("Signal recieved 1...\n");

                    //wait(&status);

                printf("parent\n");
                printf("pid: %d\n\n",getpid());

                waitpid(child3,&status,0);
                waitpid(child2,&status,0);
                waitpid(child1,&status,0);
                //wait(&status);
                printf("End of parent!\n");
            }else{ //child3 process
                printf("child3\n");
                printf("pid: %d\nppid: %d\n\n",getpid(),getppid());
                printf("Send signal...\n");
                kill(getppid(),SIGUSR1);
    		}
		}else{ //child2 process
			printf("child2\n");
			printf("pid: %d\nppid: %d\n\n",getpid(),getppid());
			//kill(child1,SIGUSR1);
			printf("Send signal to child1:%d...\n",child1);
			kill(getppid(),SIGUSR1);
		}
	}else{ //child1 process
		//pause();
		printf("Signal recieved from child2...\n");
		printf("child1\n");
		printf("pid: %d\nppid: %d\n\n",getpid(),getppid());
		kill(getppid(),SIGUSR1);
	}
	return 0;
}