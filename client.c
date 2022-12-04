#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 10
#define SA struct sockaddr

void calculateTotalGrade(int sockfd)
{
  float avg, sum;
  char buffer[MAX];

  while (1)
  {
    avg = sum = 0;

    printf("Welcome to the BUE grades calculator.\nTo calculate the average grade enter the numerical grades of five subjects. To exit at anytime type \"exit\".\n");
    for (int i = 0; i < 5; i++)
    {
      printf("Enter the student grade in module %d:\n", i + 1);
      fgets(buffer, MAX, stdin);
      if ((strncmp(buffer, "exit", 4)) == 0)
      { // checks if the input is "exit" to close the client
        write(sockfd, buffer, MAX);
        printf("Client Exit...\n");
        return;
      }
      sum += atof(buffer); // if the input is not "exit" converts it from string to float to calculate average
    }

    // Calculate the average grade
    avg = sum / 5.0;

    // Convert from float to string and storing it in the buffer
    sprintf(buffer, "%02.4f", avg);

    // Send the average grade to the server
    write(sockfd, buffer, MAX);
    memset(buffer, 0, MAX);

    // Get the grade from the server
    read(sockfd, buffer, MAX);

    printf("\nThe student is total grade: %s\n\n", buffer);
  }
}

int main()
{
  int sockfd, client_fd;
  struct sockaddr_in servaddr;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  memset(&servaddr, 0, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (client_fd = connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else
    printf("connected to the server..\n");

  // calling the function for communicating between client and server
  calculateTotalGrade(sockfd);

  // close the client-connected socket
  close(client_fd);

  return 0;
}
