#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <time.h>
#include <string.h>

typedef struct Order {
  int id;
  char date [20];
  char name [20];
  char email [20];
  int tel;
  int performance;
} ORD ;

void menu();
void help();
void list();
void search();
void delete();
void add_element();
void modify_element();
char* get_time();
int get_order_num();
void copy_file(const char *,const char *);

void clear(){system("@cls||clear");};
//void clear(){system("clear");}
int main(){
  menu();
  return 0;
}

void wait_for_press(){
  printf("\nPress any button for return to the menu...\n");
  getchar();
}

void menu(){
  help();
  char c;
  do{
    scanf("%c", &c);
    clear();
    help();
    switch(c){
      case '1': list(); break;
      case '2': search(); break;
      case '3': add_element(); break;
      case '4': modify_element(); break;
      case '5': delete(); break;
    }
  }while(c!='q');
}

void help(){
  char help [] = "Choose menu point:\n1)\tList\n2)\tSearch\n3)\tAdd new order\n4)\tModify order\n5)\tDelete order\nq)\tQuit\nh)\tHelp\n";
  printf("%s",help);
}

char* get_time(){
  char* retval = malloc(17);
  time_t ttime = time(NULL);
  struct tm *ptm = localtime(&ttime);
  sprintf(retval, "%02d.%02d.%02d_%02d:%02d", ptm->tm_mday, ptm->tm_mon, ptm->tm_year + 1900, ptm->tm_hour, 
         ptm->tm_min);
  return retval;
}

void add_element(){
  clear();
  FILE *f = fopen("t.txt","a+");

  int id;
  int date;
  char name [20];
  char email [20];
  int tel;
  int performance;

  printf("Please provide details of order!\n");
  printf("Name:\n");
  scanf("%s",name);
  printf("E-mail:\n");
  scanf("%s",email);
  printf("Tel:\n");
  scanf("%d",&tel);
  printf("Performance:\n");
  scanf("%d",&performance);

  fprintf(f, "%s %s %d %d %s\n", name, email, tel, performance, get_time());
  fclose(f);
}

void list(){
  clear();

  FILE *f = fopen("t.txt","r");
  char buff[512];
  printf("List of orders:\n");
  while(fgets(buff,100,f) != NULL){
    printf("%s",buff);
  }
  wait_for_press();
}

void search(){
  clear();

  FILE *f = fopen("t.txt","r");
  int id;
  int date;
  char name [20];
  char email [20];
  char odate [20];
  int tel;
  int performance;
  char buff[512];
  char sname[20];

  printf("Provide name for search!\n");
  scanf("%s",&sname);
  printf("Search results for %s:\n",sname);

  while(fgets(buff,100,f) != NULL){
    sscanf(buff,"%s", name);
    if(strcmp(name,sname) == 0){
      printf("%s", buff);
    }
  }
  wait_for_press();
}

void delete(){
  FILE *t = fopen("t.txt","r");
  FILE *tmp = fopen("tmp.txt","wb");
  
  char name [20];
  char buff[512];
  char sname[20];

  printf("Provide name for delete!\n");
  scanf("%s",sname);
  while(fgets(buff,sizeof(buff),t) != NULL){
    sscanf(buff,"%s ", name);
    if(strcmp(name,sname) != 0){
      fputs(buff,tmp);
    }
  }
  fclose(t);
  fclose(tmp);
  copy_file("tmp.txt","t.txt");
  wait_for_press();
}

void modify_element (){
  FILE *t = fopen("t.txt","r");
  FILE *tmp = fopen("tmp.txt","wb");
  
  int date;
  char name [20];
  char email [20];
  int tel;
  int performance;
  char buff[512];
  char sname[20];

  printf("Provide name for detail modification!\n");
  scanf("%s",sname);
  while(fgets(buff,sizeof(buff),t) != NULL){
    sscanf(buff,"%s ", name);
    if(strcmp(name,sname) != 0){
      fputs(buff,tmp);
    }
  }

  printf("Please provide new details of order!\n");
  printf("Name:\n");
  scanf("%s",name);
  printf("E-mail:\n");
  scanf("%s",email);
  printf("Tel:\n");
  scanf("%d",&tel);
  printf("Performance:\n");
  scanf("%d",&performance);

  fprintf(tmp, "%s %s %d %d %s\n", name, email, tel, performance, get_time());

  fclose(t);
  fclose(tmp);
  copy_file("tmp.txt","t.txt");
  wait_for_press();
}

void copy_file(const char *source,const char *target){
  FILE *tmp = fopen(source,"r");
  FILE *t = fopen(target,"w");

  char name [20];
  char email [20];
  char odate [20];
  char buff [200];
  int tel;
  int performance;
  while(fgets(buff,sizeof(buff),tmp) != NULL)
  {
    sscanf(buff,"%s %s %d %d %s",name,email,&tel,&performance,odate);
    fprintf(t,"%s %s %d %d %s\n",name,email,tel,performance,odate);
    //fputs(buff, t);
  }
  
  fclose(tmp);
  fclose(t);
  
  remove("tmp.txt");
}

int get_order_num(){
  FILE *f = fopen("t.txt","r");
  char c; 
  int i = 0;
  while ((c = fgetc(f)) != EOF){
    if (c == '\n') ++i;
  }
  printf("%d\n",i);
  return i;
}
