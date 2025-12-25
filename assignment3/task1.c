#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct shared {
    char sel[100];
    int b;
};

int main() {
    key_t key = ftok("shmfile", 65);  // Generate unique key
    int shmid = shmget(key, sizeof(struct shared), 0666 | IPC_CREAT);  // Create shared memory segment
    struct shared *data = (struct shared*) shmat(shmid, NULL, 0);  // Attach shared memory
    data->b = 1000;  // Initialize balance

    int fd[2];
    pipe(fd);  // Create pipe

    // Prompt the user for input
    printf("Provide Your Input From Given Options:\n");
    printf("1. Type a to Add Money\n");
    printf("2. Type w to Withdraw Money\n");
    printf("3. Type c to Check Balance\n");
    
    scanf("%s", data->sel);  // Read user input
    printf("Your selection: %s\n", data->sel);

    if (fork() == 0) {  // Child process (opr)
        close(fd[0]);  // Close reading end of pipe

        if (strcmp(data->sel, "a") == 0) {
            int add_amt;
            printf("Enter amount to be added:\n");
            scanf("%d", &add_amt);
            if (add_amt > 0) {
                data->b += add_amt;
                printf("Balance added successfully\n");
                printf("Updated balance after addition: %d\n", data->b);
            } else {
                printf("Adding failed, Invalid amount\n");
            }
        } else if (strcmp(data->sel, "w") == 0) {
            int withdraw_amt;
            printf("Enter amount to be withdrawn:\n");
            scanf("%d", &withdraw_amt);
            if (withdraw_amt > 0 && withdraw_amt <= data->b) {
                data->b -= withdraw_amt;
                printf("Balance withdrawn successfully\n");
                printf("Updated balance after withdrawal: %d\n", data->b);
            } else {
                printf("Withdrawal failed, Invalid amount\n");
            }
        } else if (strcmp(data->sel, "c") == 0) {
            printf("Your current balance is: %d\n", data->b);
        } else {
            printf("Invalid selection\n");
        }

        char msg[] = "Thank you for using";
        write(fd[1], msg, strlen(msg) + 1);  // Write to pipe
        close(fd[1]);  // Close writing end of pipe
        exit(0);
    } else {  // Parent process (home)
        wait(NULL);  // Wait for child to finish

        close(fd[1]);  // Close writing end of pipe

        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));  // Read from pipe
        printf("%s\n", buffer);
        close(fd[0]);  // Close reading end of pipe

        // Detach and destroy the shared memory
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
