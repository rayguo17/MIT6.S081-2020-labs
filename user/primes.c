#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"


int child_handler(int cur_prime){
    int up[2];
    int down;
    int has_child = 0;
    fprintf(1,"prime %d\n",cur_prime);
    if(pipe(up)<0){
        exit(1);
    }
    int fork_res = fork();
    if(fork_res<0){
        exit(1);
    }
    if(fork_res>0){
        //return write end
        close(up[0]);
        return up[1];
    }
    close(up[1]);
    while (1)
    {
        int a=0;
        if(read(up[0],&a,4)<=0){
            break;
        }
        if(a%cur_prime==0){
            continue;
        }
        if(has_child==1){
            int write_res = write(down,&a,4);
            if(write_res<0){
                fprintf(1,"write to down error\n");
            }
        }else{
            down = child_handler(a);
            has_child = 1;
        }
    }
    //fprintf(1,"%d closing!\n",cur_prime);
    close(up[0]);
    if(has_child==1){
        close(down);
    }
    int wait_pid = 0;
    wait(&wait_pid);
    exit(0);
}

int main(){
    int write_end = child_handler(2);
    for(int i=2;i<36;i++){
        if(write(write_end,&i,4)<0){
            fprintf(1,"write down error in main!");
            break;
        }
    }
    close(write_end);
    int wait_pid = 0;
    wait(&wait_pid);
    exit(0);
}

