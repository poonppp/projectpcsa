
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int pid;

int prefix(const char *pre, const char *str){
    return strncmp(pre,str,strlen(pre)) == 0;
}

void doCmd(char *cmd,char *output){
    if(strcmp(cmd,"echo ")==0){
        printf("%s\n",output);
    }else if(strcmp(cmd,"exit ")==0){
        printf("bye\n");
        printf("%d\n", atoi(output));
        exit(atoi(output));
    }else{
        if((pid=fork())<0){
            perror("Fork failed");
            exit(0);
        }
        if(!pid){
            system(cmd);
            exit(0);
        }
        if(pid){
            waitpid(pid,NULL,0);
        }
    }
}

void process(char *input,char *last,char *cmd,char *output){
    if(strcmp(input,"!!")==0){
        strcpy(input,last);
        printf("%s\n", input);
    }else{
        strcpy(last,input);
    }
    if(prefix("echo",input)||prefix("exit",input)){
        strncpy(cmd, input, 5);
        cmd[5] = '\0';
        int len = strlen(input)-4;
        strncpy(output, &input[5], len-1);
        output[len-1] = '\0';
    }else{
        strcpy(cmd,input);
    }
}

int main(int argc,char *argv[]){
    char input[255];
    char last[255];
    char cmd[10];
    char output[255];
    if(argc == 2){
        FILE* file = fopen(argv[1], "r");
        int tempChar;
        unsigned int tempCharIdx = 0U;
        while(tempChar = fgetc(file)){
            if(tempChar == EOF){
                input[tempCharIdx] = '\0';
                process(input,last,cmd,output);
                doCmd(cmd,output);
                break;
            }else if(tempChar == '\n'){
                input[tempCharIdx] = '\0';
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
            printf("icsh $ <waiting for command> ");
            gets(input);
            process(input,last,cmd,output);
            doCmd(cmd,output);
        }
    }
}