#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SEGSIZE 100                                 /* a tárolandó szöveg max. mérete */

void writeshm(int shmid, char *segptr, char *text);
void readshm(int shmid, char *segptr);
void removeshm(int shmid);                          /* függvények deklarálása */
void changemode(int shmid, char *mode);
void usage(void);

int main(int argc, char *argv[])                    /* parancssorból a paraméterek */
{
  key_t key;                                        /* kulcs */
  int shmid;                                        /* osztott memória ID-ja */
  char *segptr;                                     /* osztott memória címe */

  if (argc == 1)
    usage();                                        /* hiányos paraméterlista */

  key = ftok(".", 'S');                             /* egyedi kulcs létrehozása */

                                                    /* megnyitás, szükség esetén létrehozás */

  if ((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0666)) == -1)
  {
    printf("az osztott memoriazona letezik - megnyitom\n");

    if ((shmid = shmget(key, SEGSIZE, 0)) == -1)
    {
      perror("shmget hiba");
      exit(1);
    }
  }
  else
    printf("letrehozok egy uj osztott memoriazonat\n");

                                                    /* memóriacím hozzárendelése */

  if ((segptr = shmat(shmid, 0, 0)) == (void *) -1)
  {
    perror("shmat hiba");
    exit(1);
  }

  switch (tolower(argv[1][0]))                      /* a param. alapján elvégzi a műv. */
  {
    case 'w':                                       /* megadott szöveg írása */
      writeshm(shmid, segptr, argv[2]);
      break;
    case 'r':                                       /* osztott memóriazóna kiolvasása */
      readshm(shmid, segptr);
      break;
    case 'd':                                       /* törlés */
      removeshm(shmid);
      break;
    case 'm':                                       /* jogok módosítása */
      changemode(shmid, argv[2]);
      break;
    default:                                        /* hibás opció */
      usage();
  }
}

void writeshm(int shmid, char *segptr, char *text)
{
  strcpy(segptr, text);                             /* szöveg beírása a memóriába */
  printf("kesz...\n");
}

void readshm(int shmid, char *segptr)
{
  printf("segptr: %s\n", segptr);                   /* mem. tartalmának kiolvasása */
}

void removeshm(int shmid)
{
  shmctl(shmid, IPC_RMID, 0);                       /* osztott memória törlése */
  printf("torolve\n");
}

void changemode(int shmid, char *mode)
{
  struct shmid_ds myshmds;
  shmctl(shmid, IPC_STAT, &myshmds);               /* aktuális jogok lekérdezése */
  printf("a regi jogok: %o\n", myshmds.shm_perm.mode);
  sscanf(mode, "%o", &myshmds.shm_perm.mode);
  shmctl(shmid, IPC_SET, &myshmds);                /* új jogok beállítása */
  printf("az uj jogok: %o\n", myshmds.shm_perm.mode);
}

void usage(void)                                   /* használati útmutató */
{
  printf("shmtool - osztott memoria menedzselo rendszer\n\n");
  printf("HASZNALAT: shmtool (w)rite <szoveg>\n");
  printf("                   (r)ead\n");
  printf("                   (d)elete\n");
  printf("                   (m)ode change <oktalis_mod>\n");
  exit(1);
}