//WSL does not fully support UNIX domain sockets for datagram (SOCK_DGRAM) communication.
// Therefore I used internet based sockets

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <sys/un.h>

// #define SOCK_PATH "unix_socket_example"

// void error(char *msg)
// {
//     perror(msg);
//     exit(0);
// }

// int main(int argc, char *argv[])
// {
//     int sockfd, portno, n;

//     struct sockaddr_un serv_addr;
//     char buffer[256];

//     /* create socket, get sockfd handle */
//     sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
//     if (sockfd < 0) 
//         error("ERROR opening socket");

//     /* fill in server address */
//     bzero((char *) &serv_addr, sizeof(serv_addr));
//     serv_addr.sun_family = AF_UNIX;
//     strcpy(serv_addr.sun_path, SOCK_PATH); 

//     /* ask user for input */
//     printf("Please enter the message: ");
//     bzero(buffer,256);
//     fgets(buffer,255,stdin);

//     /* send user message to server */
//     printf("Sending data...\n");
//     n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

//     if (n < 0) 
//          error("ERROR writing to socket");

//     close(sockfd);
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[256];

    /* create socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in server address */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    /* ask user for input */
    printf("Please enter the message: ");
    memset(buffer, 0, 256);
    fgets(buffer, 255, stdin);

    /* send message to server */
    printf("Sending data...\n");
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (n < 0) 
        error("ERROR writing to socket");

    close(sockfd);
    return 0;
}
