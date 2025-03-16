#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define KEY 1234  // Unique identifier for the message queue

struct msg_buffer {
    long msg_type;  // Message type (must be > 0)
    char msg_text[100];  // Message content
};

int main() {
    int msgid;
    struct msg_buffer message;

    // Create or get the message queue
    msgid = msgget(KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Prepare the message
    message.msg_type = 1;  // Message type (can be used for filtering)
    strcpy(message.msg_text, "Hello, this is a message queue example!");

    // Send the message to the queue
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Message sent: %s\n", message.msg_text);
    return 0;
}
