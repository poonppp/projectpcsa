
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int pid = -1;
int ex = 0;
int pid_arr[100];
int job = 0;

int prefix(const char *pre, const char *str){
    return strncmp(pre,str,strlen(pre)) == 0;
}
void handler1(int signum){
    if(pid>0)kill(pid,SIGTSTP);
}

void handler2(int signum){
    if(pid>0)kill(pid,SIGINT);
}

void doCmd(char *cmd,char *output){
    if(strcmp(cmd,"echo $?")==0){
        printf("%s\n",output);
        ex = 0;
    }else if(strcmp(cmd,"exit ")==0){
        printf("bye\n");
        printf("%d\n", atoi(output));
        ex = atoi(output);
        exit(atoi(output));
    }else if(strcmp(cmd,"echo ")==0){
        printf("%s\n",output);
        ex = 0;
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
            if(cmd[strlen(cmd)-1]=='&'){
                job++;
                pid_arr[job] = pid;
                printf("[%d]%d\n",job,pid);
            }
            int status = 0;
            waitpid(pid, &ex, WUNTRACED);
            ex = status;
        }
        ex = WEXITSTATUS(ex);
    }
}

void process(char *input,char *last,char *cmd,char *output){
    if(strcmp(input,"!!")==0){
        strcpy(input,last);
        printf("%s\n", input);
    }else{
        strcpy(last,input);
    }
    if(strcmp(input,"echo $?")==0){
        strcpy(cmd,input);
    }else if(prefix("echo",input)||prefix("exit",input)){
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
        struct sigaction new_action1;
        struct sigaction new_action2;

        sigemptyset(&new_action1.sa_mask);
        new_action1.sa_handler = handler1;
        new_action2.sa_flags = 0;
        sigaction(SIGTSTP, &new_action1, NULL);

        sigemptyset(&new_action2.sa_mask);
        new_action2.sa_handler = handler2;
        new_action2.sa_flags = 0;
        sigaction(SIGINT, &new_action2, NULL);

        while(1){
            printf("icsh $ <waiting for command> ");
            gets(input);
            process(input,last,cmd,output);
            doCmd(cmd,output);
         }
    }
}