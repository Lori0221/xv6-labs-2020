#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_SIZE 512

void execXargs(char *argv[], char* args[MAXARG]){
  int pipe_fd[2];
  if(pipe(pipe_fd) == -1){
    fprintf(2, "error when call pipe\n");
    exit(1);
  }
  
  int pid = fork();
  if(pid == -1){
    fprintf(2, "error when fork\n");
    exit(1);
  }else if(pid == 0){
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    
    exec(argv[1], args);
    exit(1);
  }else{
    wait(0);
  }
}


int main(int argc, char *argv[]){
  if (argc < 3) {
    fprintf(2, "xargs needs at least 3 args.\n");
    exit(1);
  }
  char buf[MAX_SIZE];
  char *args[MAXARG];
  args[0] = argv[1];
  while(1){
    gets(buf, MAX_SIZE);
    if(buf[0] == '\0'){
      break;
    }
    
    buf[strlen(buf) - 1] = '\0';
    for(int i = 2; i < argc; i++){
      args[i -1] = argv[i];
    }
    
    args[argc - 1] = buf;
    args[argc] = 0;
    
    execXargs(argv, args);
  }
  exit(0);
}
	
