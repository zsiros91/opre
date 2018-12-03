#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/stat.h>

int c=0;
int flag=0;

void handler(int signumber){
   c++;
   //printf("Signal with number %i has arrived %dX\n\n",signumber,c);
}

int generate_rand(int f, int e);

int main(int argc,char* argv[]){
  signal(SIGUSR1, handler);

  key_t kulcs;
  int oszt_mem_id;

  char *s;
  kulcs=ftok("kulcs",1);
  oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
  s = shmat(oszt_mem_id,NULL,0);


  pid_t child2;
  pid_t child1=fork();
  if (child1<0){perror("The fork calling was not succesful\n\n"); exit(1);}
  if (child1>0){
    child2=fork();
    if(child2>0){
      int status;

      char buffer[] = "Hajra Fradi! \n";                   
       
      strcpy(s,buffer);
      
      shmdt(s);     
      pause();
      sleep(5);
 
      shmctl(oszt_mem_id,IPC_RMID,NULL);
      
      waitpid(child1,&status,0);
      waitpid(child2,&status,0);
      printf("parent: End of parent!\n\n");    
        
    }else{ //child2 process
      printf("child2: Read from sh: %s\n\n",s);
      shmdt(s);
      kill(getppid(),SIGUSR1);
      printf("child2: End of child2!\n\n");
      
    }
  }else{ //child1 process
    
    
    //kill(getppid(),SIGUSR1);
    //printf("child1: End of child1!\n\n\n");
  }
  return 0;
}

int generate_rand(int f, int e){ 
  srand(time(NULL));
  return (rand()%(e-f)+f);
}