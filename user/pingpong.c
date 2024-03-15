#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(){
    int p[2];
    int p_c[2];
    if(pipe(p_c)<0){
        exit(1);
    }
    if(pipe(p)<0){
        exit(1);
    }
    int child = fork();
    char buf[10];
    if(child<0){
        //printf("i am done\n");
        exit(1);
    }
    if(child==0){
        //child
        //close(p[0]);
        int pid = getpid();
        //printf("in child: %d",pid);
        read(p_c[0],buf,4);
        fprintf(1,"%d: received %s\n",pid,buf);
        write(p[1],"pong",4);
        exit(0);
    }else{
        //parent
        //close(p[1]);
        int pid = getpid();
        //printf("in parent: %d",pid);
        write(p_c[1],"ping",4);
        read(p[0],buf,4);
        fprintf(1,"%d: received %s\n",pid,buf);
    }
    wait(&child);
    exit(0);
}