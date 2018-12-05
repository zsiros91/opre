#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

int c=0;
int flag=0;

void handler(int signumber){
   c++;
   printf("Signal with number %i has arrived %dX\n\n",signumber,c);
}

int main(){
signal(SIGUSR1, handler);

int pipe1[2];
int pipe2[2];
int pipe3[2];

pipe(pipe1);
pipe(pipe2);
pipe(pipe3);

pid_t child3;
pid_t child2;
pid_t child1=fork();
if (child1<0){perror("The fork calling was not succesful\n\n"); exit(1);}
if (child1>0){
    child2=fork();
    if(child2>0){
            child3=fork();
            if(child3>0){ //parent process
                int status;

                close(pipe1[0]);
                write(pipe1[1], "msg1",6);
                close(pipe1[1]);
                sleep(3);

                close(pipe2[0]);
                write(pipe2[1], "msg2",6);
                close(pipe2[1]);
                sleep(3);

                close(pipe3[0]);
                write(pipe3[1], "msg3",6);
                close(pipe3[1]);
                sleep(3);

                
                while(c<3){
                	sleep(2);
                }
				sleep(2);
                
                printf("parent: pid: %d\n\n",getpid());

                waitpid(child1,&status,0);
                waitpid(child3,&status,0);
                waitpid(child2,&status,0);
                
                printf("parent: End of parent!\n\n");
            }else{ //child3 process
				
				char p3[10];
				close(pipe3[1]);
				read(pipe3[0],p3,sizeof(p3));
				close(pipe3[0]);
				sleep(1);
				printf("child3: read from pipe3 %s\n\n", p3);                
                kill(getppid(),SIGUSR1);
    		}
		}else{ //child2 process
			
			char p2[10];
			close(pipe2[1]);
			read(pipe2[0],p2,sizeof(p2));
			close(pipe2[0]);
			sleep(1);
			printf("child2: read from pipe2 %s\n\n", p2);
			//kill(child1,SIGUSR1);
			//printf("Send signal to child1:%d...\n",child1);
			
			kill(getppid(),SIGUSR1);
		}
	}else{ //child1 process
		//pause();
		//printf("Signal recieved from child2...\n");
		
		char p1[10];
		close(pipe1[1]);
		read(pipe1[0],p1,sizeof(p1));
		close(pipe1[0]);
		sleep(1);
		printf("child1: read from pipe1 %s\n\n", p1);

		kill(getppid(),SIGUSR1);
	}
	return 0;
}