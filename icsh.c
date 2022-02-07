
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void doCmd(char *cmd,char *output){

}

void process(char *input,char *last,char *cmd,char *output){

}

int main(){
    char input[255];
    char last[255;
    char cmd[10];
    char output[255];
    if(argc ==2){
        FILE *file = fopen(argv[1], "r");
        int tempChar;
        unsigned int tempCharIdx = 0U;
        while(tempChar = fgetc(file)){
            if(tempChar = EOF){
                input[tempCharIdx-1] = '\0';
                process(input,last,cmd,output);
                doCmd(cmd,output);
                break;
            }else if(tempChar == '\n'){
                input[tempCharIdx-1] = '\0';
                tempCharIdx = 0U;
                process(input,last,cmd,output);
                doCmd(cmd,output);
                continue;
            }else{
                input[tempCharIdx++] = (char)tempChar;
            }
        }
        fclose(file);
    }else{
        printf("Starting IC shell\n");
        while(1){
            printf("icsh $ <waiting for command>");
            gets(input);
            process(input,last,cmd,output);
            doCmd(cmd,output);
        }
    }
}
    
    