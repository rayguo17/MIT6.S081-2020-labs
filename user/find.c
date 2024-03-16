#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *get_name_from_path(char * path){
    char*tmp = path;
    char *last_slash = path;
    for(;*tmp!='\0';tmp++){
        if(*tmp=='/'){
            last_slash = tmp;
        }
    }
    return last_slash+1;
}


void find(char *path, char *target){
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;
    char *file_name;
    if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch (st.type)
  {
  case T_FILE:
  // check if it is the same with target.
    file_name = get_name_from_path(path);
    if(strcmp(file_name,target)==0){
        fprintf(1,"%s\n",path);
    }
    close(fd);
    return;
  
  case T_DIR:
      //printf("iterating though: %s\n",path);
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      //should call recursive here, i want to reallocate string
      char *new_buf = malloc(512);
      if(strcmp(p,".")==0 || strcmp(p,"..")==0){
          continue;
      }
      strcpy(new_buf,buf);
      find(new_buf,target);
      free(new_buf);
    }
    break;
  }
  close(fd);
  return;
}



int main(int argc, char *argv[]){
    
    if(argc!=3){
        fprintf(1,"expected two arguments!\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}