#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char input[100];
    char last[100];
    int dolast = 0;
    printf("Starting IC shell\n");
    while(1){
        printf("icsh $ <waiting for command>");
        gets(input);
        if(strcmp(input, "!!")==0){
            dolast = 1;
            strcpy(input,last);
            printf("%s\n",input);
        }else{
            dolast = 0;
            strcpy(last,input);
        }
        char cmd[10];
        strncpy(cmd,input,5);
        cmd[5] = '\0';
        int len = strlen(input)-4;
        char output[len];
        strncpy(output,&input[5],len-1);
        output[len-1] = '\0';
        if(strcmp(cmd,"echo ")==0){
            printf("%s\n",output);
        }
        if(strcmp(cmd,"exit")==0){
            printf("bye\n");
            exit(atoi(output));
        }
    }
}
