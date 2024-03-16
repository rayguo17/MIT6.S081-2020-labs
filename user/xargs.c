#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    char buf[512];
    char *extend_argv[MAXARG-argc];
    char *new_argv[MAXARG];
    char *current_arg;
    int i;
    char* tmp;
    int wait_res;
    int arg_cnt,word_cnt;
    int res = read(0,buf,512);
    if(res<0){
        fprintf(2,"error reading from input!\n");
        exit(1);
    }
    arg_cnt = 0;
    word_cnt = 0;
    current_arg = buf;
    for(tmp = buf;*tmp!='\0';tmp++){
        if(*tmp=='\n'){
            *tmp = '\0';
            tmp=tmp+1;
            extend_argv[arg_cnt] = malloc(word_cnt+1);
            if(strcpy(extend_argv[arg_cnt],current_arg)<0){
                fprintf(2,"copy string error\n");
                exit(1);
            }
            extend_argv[arg_cnt][word_cnt]='\0';
            arg_cnt+=1;
            word_cnt = 0;
            current_arg = tmp;
            if(*(tmp+1)==0){
                break;
            }
        }
        word_cnt+=1;
    }
    for(i=1;i<argc;i++){
        new_argv[i-1] = argv[i];
    }
    new_argv[argc]= 0;
    if(word_cnt!=0){
        extend_argv[arg_cnt] = malloc(word_cnt+1);
        if(strcpy(extend_argv[arg_cnt],current_arg)<0){
            fprintf(2,"copy string error\n");
            exit(1);
        }
        extend_argv[arg_cnt][word_cnt]='\0';
        arg_cnt+=1;
        word_cnt = 0;
    }
    //printf("get %d extended argument\n",arg_cnt);
    for(i=0;i<arg_cnt;i++){
        if(fork()==0){
            new_argv[argc-1] = extend_argv[i];
            //printf("exec: %s with 1:%s 2:%s\n",argv[1],new_argv[0],new_argv[1]);
            
            if(exec(argv[1],new_argv)!=0){
                fprintf(2,"execute %s failed\n",argv[1]);
                exit(1);
            };
            exit(0);
        }else{
            wait(&wait_res);
            free(extend_argv[i]);
        }
    }
    exit(0);
}