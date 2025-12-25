#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct shared {
    char sel[100];
    int b;
};

int main() {
    int pipefd[2];  
    pid_t pid;      
    key_t key = 1234; 
    int shmid;      
    struct shared *shm;  

    shmid = shmget(key, sizeof(struct shared), 0666 | IPC_CREAT);         //shared mem
    if (shmid == -1){
        perror("shmget failed");
        exit(1);
    }

    shm = (struct shared *)shmat(shmid, NULL, 0);
    if (shm == (void *) - 1) {
        perror("shmat failed");
        exit(1);
    }

    shm->b = 1000;

    if (pipe(pipefd) == -1){
        perror("pipe failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0){
        perror("fork failed");
        exit(1);
    }

    if (pid > 0){  
        close(pipefd[0]); 

        printf("Provide your input from given options:\n");
        printf("1. Type a to add money\n");
        printf("2. Type w to withdraw money\n");
        printf("3. Type c to check balance\n");

        scanf("%s", shm->sel);
        printf("Your selection: %s\n", shm->sel);

     
        wait(NULL);

        char buffer[100];
        read(pipefd[0], buffer, sizeof(buffer));
        printf("%s\n", buffer);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        close(pipefd[1]);  
    } 
    
    else{  
        close(pipefd[1]);  
        if (strcmp(shm->sel, "a") == 0){  
            int amount;
            printf("\nEnter amount to be added:\n");
            scanf("%d", &amount);

            if (amount > 0) {
                shm->b += amount;
                printf("Balance added successfully\n");
                printf("Updated balance after addition:\n%d\n", shm->b);
            } else {
                printf("Adding failed, Invalid amount\n");
            }

        }else if (strcmp(shm->sel, "w") == 0){  
            int amount;
            printf("\nEnter amount to be withdrawn:\n");
            scanf("%d", &amount);

            if (amount > 0 && amount <= shm->b){
                shm->b -= amount;
                printf("Balance withdrawn successfully\n");
                printf("Updated balance after withdrawal:\n%d\n", shm->b);
            } 
            else{
                printf("Withdrawal failed, Invalid amount\n");
            }

        }
        else if (strcmp(shm->sel, "c") == 0){  
            printf("\nYour current balance is:\n%d\n", shm->b);

        }
        else{  
            printf("\nInvalid selection\n");
        }

        write(pipefd[1], "Thank you for using", 19);
        close(pipefd[0]);  
        exit(0);
    }

    return 0;
}
