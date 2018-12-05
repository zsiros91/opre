#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h>
#include <wait.h> 

int c=0;

struct uzenet { 
	long mtype;
	char mtext [ 1024 ]; 
};

int kuld( int uzenetsor,char* str);
char* fogad( int uzenetsor);

void handler(int signumber){
   c++;
   printf("Signal with number %i has arrived %dX\n\n",signumber,c);
}
int main(int argc, char* argv[]){

	int uzenetsor, status; 
	key_t kulcs;
	kulcs = ftok(argv[0],1); 
	printf ("A kulcs: %d\n",kulcs);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	}

	signal(SIGTERM, handler);

	int pipe1[2];
	pipe(pipe1);

	pid_t child1=fork();
	if (child1<0){perror("The fork calling was not succesful\n\n"); exit(1);}
	if (child1>0){
	    int status;
        int i=0;
        
        while(i<3){
        	pause();
        	int s=i+1;
        	
            write(pipe1[1], &s,(int)sizeof(int));

            sleep(1);
            printf("parent sent %d\n",s);
            ++i;
            kill(child1,SIGTERM);
        }
    	close(pipe1[1]);
        close(pipe1[0]);
		sleep(2);

		printf("%s\n",fogad(uzenetsor));
        waitpid(child1,&status,0);
        printf("parent: End of parent!\n\n");

	}else{ //child1 process
		int i=0;
		int r;
		kill(getppid(),SIGTERM);
		while(i<3){
			pause();
			read(pipe1[0],&r,(int)sizeof(int));
			printf("child receivied %d\n", r);
			sleep(1);
			++i;
			kill(getppid(),SIGTERM);
		}

		char* buff="asd";
		kuld(uzenetsor,buff);

		close(pipe1[1]);
		close(pipe1[0]);
		
	}
	return 0;
}

// sendig a message
int kuld( int uzenetsor,char* str ) 
{ 
     struct uzenet uz = { 5, "" }; 
     strcpy(uz.mtext,str);
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
// receiving a message. 
char* fogad( int uzenetsor ) 
{ 
	struct uzenet uz; 
	int status; 
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
	char* retval =  uz.mtext;
	if ( status < 0 ) 
		perror("msgsnd"); 
	else
		printf( "A kapott uzenet kodja: %ld, szovege: %s\n", uz.mtype, uz.mtext ); 
	return retval;
} 