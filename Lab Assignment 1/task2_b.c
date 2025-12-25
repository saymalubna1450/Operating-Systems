#include <stdio.h>

int main(void){
    FILE *input, *output;

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    char line;
    char prev_line = ' ';
 

     while(fscanf(input, "%c", &line) != EOF) {

        if(line != ' ' || prev_line != ' ') {
            fprintf(output, "%c", line);
        }

        prev_line = line;
    }

    fclose(input);
    fclose(output);


    return 0; 

}
