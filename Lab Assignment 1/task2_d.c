#include <stdio.h>
#include <string.h>

int main() {
    char email[200];

    printf("Enter your email address: ");
    scanf("%s", email);

    if (strstr(email, "@sheba.xyz") != NULL) {
        printf("Email address is okay");
    }

    else if (strstr(email, "@kaaj.com") != NULL) {
        printf("Email address is outdated");
    }
    
    

    return 0;
}
