#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>



int main() {
    char* home_path=getenv("HOME");
    char* filename="/input.txt";
    int length_path=strlen(home_path);
    int length_filename=strlen(filename);
    char* dest=(char*)malloc(length_path+length_filename);
    strcpy(dest,home_path);
    strcat(dest,filename);
    //printf("%s",dest);

    FILE *file;
    file = fopen(dest, "r");
    char* line= (char*) malloc(1024*sizeof(char));
    if (file) {
        while (fgets(line,1024,file)) {
            printf("? ");
            printf(line);
        }
        fclose(file);
    }
    free(line);
    free(dest);
    while(1){
        char* input = (char*) malloc(sizeof(char));
        printf("? ");
        if(fgets(input,512,stdin)==NULL) break;
        char* token[4];
        input = strtok(input,"\n");
        token[0]=strtok(input," ");
        int i=0;
        while(token[i]!=NULL){
            token[i+1]=strtok(NULL," ");
            i++;
        }
//        for(i=0;i<2;i++){
//            printf("%s ",token[i]);
//        }
        if(strcmp(token[0],"exit")==0) exit(0);
        if(strcmp(token[0],"cd")==0) {
            char *dir = NULL;
            //printf("%s",dir);
            if (token[1] == NULL || strcmp(token[1], "~") == 0) {
                dir = getenv("HOME");
            } else {
                dir = token[1];

            }
            int v = chdir(dir);
            if (v == 1) {
                printf("failed to change directory\n");
            }

        }else if(strcmp(token[0],"setenv")==0){
            if(token[2]==NULL) token[2]="\0";
            int v = setenv(token[1],token[2],1);
            if(v==1){
                printf("Failed to set the environment variable");
            }
            char* temp = getenv(token[1]);
            printf("%s\n",temp);
        }else if(strcmp(token[0],"unsetenv")==0){
            if(token[1]==NULL){
                printf("Falied");
            }else{
                int v = unsetenv(token[1]);
                if(v==1){
                    printf("Failed to unset environment variable\n");
                }
//                char* temp = getenv(token[1]);
//                printf("haha= %s",temp);
            }
        }else{
            int child;
            child = fork();
            if(child==0){
                execvp(token[0],&token[0]);

            }else if(child<0){
                printf("error");
            }else{
                wait(child);
            }
        }
    }

    exit(0);
}