#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send message to server
    char message[] = "Hello from client!";
    sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr*)&server_addr, addr_len);

    // Receive response from server
    recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len);
    printf("Received from server: %s\n", buffer);

    // Close socket
    close(sockfd);
    return 0;
}
