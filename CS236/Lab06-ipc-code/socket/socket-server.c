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
    exit(1);
}

int main()
{
    int sockfd;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int n;

    /* create socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in server address */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;  // Accept from any IP
    serv_addr.sin_port = htons(PORT);  // Convert port to network byte order

    /* bind socket to this address */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    printf("Server ready, waiting for messages...\n");

    /* read message from client */
    memset(buffer, 0, 256);
    // You can use this cli_addr and cli_len to response back to the sender.
    while((n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &cli_len)) > 0){
        buffer[n] = '\0';   // proper null terminate, n is the number of bytes received.
        if(strlen(buffer) == 4 && !strcmp(buffer, "STOP")){
            break;
        }
        printf("%s", buffer);
    }
    close(sockfd);
    return 0;
}
