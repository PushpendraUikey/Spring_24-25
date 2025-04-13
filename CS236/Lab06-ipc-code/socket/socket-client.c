#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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

int main(int argc, char * argv[])
{
    if(argc != 2){
        printf("Usage: client <filename>\n");
        exit(1);
    }
    char filename[50];
    strcpy(filename, argv[1]);
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
    
    printf("Sending the file %s...\n", filename);
    memset(buffer, 0, 256);
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        perror("can't open file\n");
        exit(1);
    }
    FILE* fp = fdopen(fd, "r");
    if(!fp){
        close(fd);
        perror("error opening file\n");
        exit(1);
    }
    while(fgets(buffer, 255, fp)){  // after a read offset increases
        sleep(1);
        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (n < 0) 
            error("ERROR writing to socket");
    }
    strcpy(buffer, "STOP");
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    fclose(fp); // this closes fd as well.
    // fgets(buffer, 255, stdin);  // taking the input from the standard input(terminal)
    /* send message to server */
    // printf("Sending data...\n");
    if (n < 0) 
        error("ERROR can't stop server!\n");
    else printf("File sent successfully\n");
    close(sockfd);
    return 0;
}

// Server binds to known address, but client sockets need not bind.