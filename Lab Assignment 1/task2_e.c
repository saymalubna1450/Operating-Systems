#include <stdio.h>
#include <string.h>

int main() {
    char str[300];
    printf("Enter a string input: ");
    scanf("%s", str);
    int length = strlen(str);
    
    char *Begin_str = str; 
    char *end_str = str + length - 1; 
    int ans = 1;

    while (Begin_str < end_str) {
        if (*Begin_str != *end_str) {
            ans = 0;
            break;
        }
        Begin_str += 1; 
        end_str -= 1;
    }

    if (ans == 1) {
        printf("Palindrome");
    } else {
        printf("Not Palindrome");
    }

    return 0;
}
