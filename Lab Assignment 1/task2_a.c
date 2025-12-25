#include <stdio.h>

int main()
{
    float a, b, result;

    printf("enter the first number: ");
    scanf("%f", &a);

    printf("enter the second number: ");
    scanf("%f", &b);

    if (a > b){
        result = a - b;
        printf("Subtraction = %f", result);
    }

    else if(a < b){
        result = a + b;
        printf("Addition = %f", result);

    }

    else{
        result = a * b;
        printf("Multiplication = %f", result);
    }

    return 0;

}