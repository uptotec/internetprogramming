#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 10
#define PORT 8080
#define SA struct sockaddr

// function designed for chat between client and server.
void calculateTotalGrade(int sockfd)
{
  float grade;
  char buffer[MAX];

  while (1)
  {
    memset(buffer, 0, MAX); // clearing the memory (bzero is deprecated)

    // read the message from client and copy it in buffer
    read(sockfd, buffer, MAX);

    // Exit the loop if the client wants to quit
    if (strncmp("exit", buffer, 4) == 0)
    {
      printf("closing connection with client...\n");
      return;
    }

    grade = atof(buffer);

    // processing the grade
    if (grade < 40)
    {
      strcpy(buffer, "F");
    }
    else if (grade < 44)
    {
      strcpy(buffer, "D-");
    }
    else if (grade < 47)
    {
      strcpy(buffer, "D");
    }
    else if (grade < 50)
    {
      strcpy(buffer, "D+");
    }
    else if (grade < 54)
    {
      strcpy(buffer, "C-");
    }
    else if (grade < 57)
    {
      strcpy(buffer, "C-");
    }
    else if (grade < 60)
    {
      strcpy(buffer, "C+");
    }
    else if (grade < 64)
    {
      strcpy(buffer, "B-");
    }
    else if (grade < 67)
    {
      strcpy(buffer, "B");
    }
    else if (grade < 70)
    {
      strcpy(buffer, "B+");
    }
    else if (grade < 74)
    {
      strcpy(buffer, "A-");
    }
    else if (grade < 77)
    {
      strcpy(buffer, "A");
    }
    else
    {
      strcpy(buffer, "A+");
    }

    // Send the average grade in litters to the client
    write(sockfd, buffer, MAX);
  }
}

// Driver function
int main()
{
  int sockfd, connfd;
  struct sockaddr_in servaddr;
  int servaddr_len = sizeof(servaddr);

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  memset(&servaddr, 0, servaddr_len);

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
  {
    printf("socket bind failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully binded..\n");

  // now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0)
  {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening..\n");

  while (1)
  {
    // accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&servaddr, (socklen_t *)&servaddr_len);
    if (connfd < 0)
    {
      printf("server accept failed...\n");
      exit(0);
    }
    else
      printf("server accept the client...\n");

    // calling the function for chatting between client and server
    calculateTotalGrade(connfd);

    // After chatting close the socket
    close(connfd);
  }

  // closing the listening socket
  // Server doesn't shutdown in order to keep serving clients, server closes if there are no clients
  // shutdown(sockfd, SHUT_RDWR);

  return 0;
}
