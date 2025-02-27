#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>


#define PORT 2908


extern int errno;
typedef struct thData
{
  int idThread;
  int cl;
} thData;

static void *treat(void *);
int raspunde(void *, int logged);
char *Use_Exec(char *command);
void Run_command(char *command);
void Login(void *arg);
int tackle_login(char *Username);

int main()
{
  struct sockaddr_in server;
  struct sockaddr_in from;
  int sd;
  int pid;
  pthread_t th[100];
  int i = 0;

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[server]Eroare la socket().\n");
    return errno;
  }
  int on = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  bzero(&server, sizeof(server));
  bzero(&from, sizeof(from));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT);

  if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[server]Eroare la bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen(sd, 2) == -1)
  {
    perror("[server]Eroare la listen().\n");
    return errno;
  }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
  {

    {
      int client;
      thData *td; // parametru functia executata de thread
      int length = sizeof(from);

      printf("[server]Asteptam la portul %d...\n", PORT);
      fflush(stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0)
      {
        perror("[server]Eroare la accept().\n");
        continue;
      }

      /* s-a realizat conexiunea, se astepta mesajul */

      // int idThread; //id-ul threadului
      // int cl; //descriptorul intors de accept

      td = (struct thData *)malloc(sizeof(struct thData));
      td->idThread = i++;
      td->cl = client;
      pthread_create(&th[i], NULL, &treat, td);
    }

  } // while
};
static void *treat(void *arg)
{
    struct thData tdL = *((struct thData *)arg);
    free(arg); // Free memory allocated for thread data
    printf("[Thread %d] Handling new client connection...\n", tdL.idThread);
    fflush(stdout);

    pthread_detach(pthread_self());

    int logged = 0;
    while (1)
    {
        int new_logged = raspunde(&tdL, logged);

        if (new_logged > 0)
        {
            logged = new_logged; // Update logged status
        }
        else if (new_logged < 0)
        {
            printf("[Thread %d] Client disconnected.\n", tdL.idThread);
            break; // Exit loop on client disconnection
        }
    }

    close(tdL.cl); // Close client socket
    printf("[Thread %d] Client connection closed.\n", tdL.idThread);
    pthread_exit(NULL); // Exit the thread
    return NULL;
}


int raspunde(void *arg, int logged)
{
  char Tbuf[2048];
  int i = 0;
  struct thData tdL;
  tdL = *((struct thData *)arg);
  memset(Tbuf, 0, sizeof(Tbuf));
  int read_bytes = read(tdL.cl, &Tbuf, sizeof(Tbuf) - 1);
    if (read_bytes <= 0)
    {
        if (read_bytes == 0)
        {
            printf("[Thread %d] Client disconnected.\n", tdL.idThread);
        }
        else
        {
            perror("[Thread] Error reading from client.\n");
        }
        return -1; // Signal disconnection
    }
  printf("[Thread %d]Mesajul a fost receptionat...%s\n", tdL.idThread, Tbuf);

  /*pregatim mesajul de raspuns */
  if (strstr(Tbuf, "login : ") == Tbuf && logged == 0)
  {
    strcpy(Tbuf, Tbuf + 8);
    int result = tackle_login(Tbuf);
    if (result == 2)
    {
      return errno;
    }
    else if (result == 1)
    {
      strcpy(Tbuf, "Conectare reusita! Va rugam scrieti comenzile dorite");
      printf("[Thread %d]Trimitem mesajul inapoi...%s\n", tdL.idThread, Tbuf);

      /* returnam mesajul clientului */
      if (write(tdL.cl, Tbuf, strlen(Tbuf) + 1) <= 0)
      {
        printf("[Thread %d] ", tdL.idThread);
        perror("[Thread]Eroare la write() catre client.\n");
      }
      else
        printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
      return 1;
    }
    else
    {
      strcpy(Tbuf, "Incercati sa va logati astfel: login : [username] [parola]");
    }
  }
  else if (logged == 0)
  {
    strcpy(Tbuf, "Incercati sa va logati astfel: login : [username] [parola]");
  }
  else
  {
    char *output = Use_Exec(Tbuf);
    if (output)
    {
      strcpy(Tbuf, output);
      free(output);
    }
    else if ( output == NULL)
    {
      strcpy(Tbuf, "Wrong command");
    }

  }
  printf("[Thread %d]Trimitem mesajul inapoi...%s\n", tdL.idThread, Tbuf);

  /* returnam mesajul clientului */
  if (write(tdL.cl, Tbuf, strlen(Tbuf) + 1) <= 0)
  {
    printf("[Thread %d] ", tdL.idThread);
    perror("[Thread]Eroare la write() catre client.\n");
  }
  else
    printf("[Thread %d]Mesajul a fost transmis cu succes.\n", tdL.idThread);
  return 0;
}

int tackle_login(char *LoginInfo)
{
  FILE *FisierAdmini = fopen("Admins", "r");
  if (FisierAdmini == NULL)
  {
    return 2; // Error opening file
  }
  char User[300];
  while (fgets(User, sizeof(User), FisierAdmini))
  {
    size_t len = strlen(User);
    if (len > 0 && User[len - 1] == '\n')
    {
      User[len - 1] = '\0';
    }

    if (strcmp(User, LoginInfo) == 0)
    {
      fclose(FisierAdmini);
      return 1;
    }
  }

  fclose(FisierAdmini);
  return 0;
}
char *Use_Exec(char *command)
{
  FILE *fp;
  char *output = malloc(2048);
  if (!output)
  {
    perror("Memory allocation failed");
    return NULL;
  }
  output[0] = '\0';

  fp = popen(command, "r");
  if (fp == NULL)
  {
    perror("Failed to run command");
    free(output);
    return NULL;
  }

  size_t total_len = 0;
  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
    size_t len = strlen(buffer);
    if (total_len + len + 1 > 2048)
    {
      fprintf(stderr, "Output exceeds buffer size\n");
      free(output);
      pclose(fp);
      return NULL;
    }
    strncpy(output + total_len, buffer, len);
    total_len += len;
    output[total_len] = '\0'; // Ensure null-termination
  }

  if (pclose(fp) == -1)
  {
    perror("Error closing pipe");
  }

  return output;
}
