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
   //printf("Signal with number %i has arrived %dX\n\n",signumber,c);
}

int generate_rand(int f, int e);

int main(){
signal(SIGUSR1, handler);

int pipe1[2];
int pipe2[2];

pipe(pipe1);
pipe(pipe2);

int pipe1b[2];
int pipe2b[2];

pipe(pipe1b);
pipe(pipe2b);

pid_t child2;
pid_t child1=fork();
if (child1<0){perror("The fork calling was not succesful\n\n"); exit(1);}
if (child1>0){
    child2=fork();
    if(child2>0){
      int status;

      close(pipe1[0]);
      write(pipe1[1], "task1",6);
      close(pipe1[1]);
      kill(child1, SIGUSR1);
      printf("parent: task sent to child1!\n\n");
      sleep(2);

      close(pipe2[0]);
      write(pipe2[1], "task2",6);
      close(pipe2[1]);
      kill(child2, SIGUSR1);
      printf("parent: task sent to child2!\n\n");
      
      char b1[20];
      close(pipe1b[1]);
      read(pipe1b[0],b1,sizeof(b1));
      close(pipe1b[0]);
      printf("parent: child1: %s\n\n",b1 );
      sleep(2);
      
      char b2[20];
      close(pipe2b[1]);
      read(pipe2b[0],b2,sizeof(b2));
      close(pipe2b[0]);
      printf("parent: child2: %s\n\n",b2 );
      sleep(2);
      
      
      waitpid(child1,&status,0);
      waitpid(child2,&status,0);
      
      printf("parent: End of parent! All orders delivere\n\n");    
        
    }else{ //child2 process
      pause();
      char p2[10];
      close(pipe2[1]);
      read(pipe2[0],p2,sizeof(p2));
      close(pipe2[0]);
      printf("child2: task have been read from pipe2 %s\n\n", p2);
      sleep(2);

      close(pipe2b[0]);
      write(pipe2b[1], "Copy that!",20);
      close(pipe2b[1]);
      sleep(2);

      kill(getppid(),SIGUSR1);
      printf("child2: final task resolved!\n\n\n");
      
    }
  }else{ //child1 process
    pause();
    char p1[10];
    close(pipe1[1]);
    read(pipe1[0],p1,sizeof(p1));
    close(pipe1[0]);
    printf("child1: task have been read from pipe1 %s\n\n", p1);
    sleep(2);

    close(pipe1b[0]);
    write(pipe1b[1], "Copy that!",20);
    close(pipe1b[1]);
    sleep(2);
    
    printf("child1: final task resolved!\n\n");
    kill(getppid(),SIGUSR1);
  }
  return 0;
}

int generate_rand(int f, int e){ 
  srand(time(NULL));
  return (rand()%(e-f)+f);
}