#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define KEY 1234  // Same unique key as sender

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    int msgid;
    struct msg_buffer message;

    // Get the existing message queue
    msgid = msgget(KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Receive the message (msg_type = 1)
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    printf("Received message: %s\n", message.msg_text);

    // Remove the message queue after reading
    msgctl(msgid, IPC_RMID, NULL);
    // If not deleted 
    // The queue remains in the system, consuming kernel memory.
    //The queue will persist even after the process exits, leading to potential memory leaks.

    return 0;
}

// msgget() does not returns a file descriptor but instead it returns a message queue identifier.