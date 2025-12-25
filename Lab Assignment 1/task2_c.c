#include <stdio.h>
#include <string.h>

int main() {
    char pass[150];
    int uppercase = 0, lowercase = 0,  digit = 0, special = 0, printed = 0;

    printf("Enter your password: ");
    scanf("%s", pass);
    int size = strlen(pass);

    for (int i = 0; i < size; i++) {
        if (pass[i] >= 97 && pass[i] <= 122) {
            lowercase = 1;
        } else if (pass[i] >= 65 && pass[i] <= 90) { 
            uppercase = 1;
        } else if (pass[i] >= 48 && pass[i] <= 57) {
            digit = 1;
        } else if (pass[i] == '$' || pass[i] == '_' || pass[i] == '@' || pass[i] == '#') {
            special = 1;
        }
    }

    if (lowercase >= 1 && uppercase >= 1 && digit >= 1 && special >= 1) {
        printf("OK");
    } 
    
    else {
        if (lowercase == 0) {
            printf("Lowercase character is missing");
            printed = 1;
        }
        
        if (uppercase == 0) {
            if (printed == 1){
                printf(", ");
            }
            printf("Uppercase character is missing");
            printed = 1;
        }
        
        if (digit == 0) {
            if (printed == 1){
                printf(", ");
            }
            printf("Digit is missing");
            printed = 1;
        }
        
        if (special == 0) {
            if (printed == 1){
                printf(", ");
            }
            printf("Special character is missing");
            printed = 1;
        }
    }

    return 0;
    
}