#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct msg {
    long int type;        
    char txt[6];          
};

void login_process(int msgid) {
    struct msg message;
    char workspace[10];
    
    printf("Please enter the workspace name:\n");
    scanf("%s", workspace);

    if (strcmp(workspace, "cse321") != 0) {
        printf("Invalid workspace name\n");
        exit(0);
    }

    message.type = 1;
    strcpy(message.txt, workspace);
    msgsnd(msgid, &message, sizeof(message.txt), 0);
    printf("Workspace name sent to OTP generator from log in: %s\n", message.txt);
}

void otp_generator_process(int msgid) {
    struct msg message;

    msgrcv(msgid, &message, sizeof(message.txt), 1, 0);
    printf("OTP generator received workspace name from log in: %s\n", message.txt);

    pid_t otp = getpid();
    snprintf(message.txt, sizeof(message.txt), "%d", otp);

    message.type = 2;
    msgsnd(msgid, &message, sizeof(message.txt), 0);
    printf("OTP sent to log in from OTP generator: %s\n", message.txt);

    message.type = 3;
    msgsnd(msgid, &message, sizeof(message.txt), 0);
    printf("OTP sent to mail from OTP generator: %s\n", message.txt);
}

void mail_process(int msgid) {
    struct msg message;

    msgrcv(msgid, &message, sizeof(message.txt), 3, 0);
    printf("Mail received OTP from OTP generator: %s\n", message.txt);

    message.type = 4;
    msgsnd(msgid, &message, sizeof(message.txt), 0);
    printf("OTP sent to log in from mail: %s\n", message.txt);
}

int main() {
    key_t key;
    int msgid;
    pid_t otp_gen_pid, mail_pid;
    struct msg message1, message2;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    otp_gen_pid = fork();
    if (otp_gen_pid == 0) {
        otp_generator_process(msgid);
        exit(0);
    } else {
        mail_pid = fork();
        if (mail_pid == 0) {
            mail_process(msgid);
            exit(0);
        } else {
            login_process(msgid);

            wait(NULL);
            wait(NULL);

            msgrcv(msgid, &message1, sizeof(message1.txt), 2, 0);
            printf("Log in received OTP from OTP generator: %s\n", message1.txt);

            msgrcv(msgid, &message2, sizeof(message2.txt), 4, 0);
            printf("Log in received OTP from mail: %s\n", message2.txt);

            if (strcmp(message1.txt, message2.txt) == 0) {
                printf("OTP Verified\n");
            } else {
                printf("OTP Incorrect\n");
            }
            msgctl(msgid, IPC_RMID, NULL);
        }
    }

    return 0;
}
