#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"


int main(int argc, char *argv[]){
    if(argc!=2){
        write(2, "usecase error!\n",15);
    }
    int tick_to_sleep = atoi(*(argv+1));
    sleep(tick_to_sleep);
    exit(0);
}