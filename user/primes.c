#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

// pipe要及时关掉
void primes(int lpipe[2]){
  close(lpipe[WR]);
  int first;
  if(read(lpipe[RD], &first, sizeof(INT_LEN)) != sizeof(INT_LEN)){
    fprintf(2, "error when read the prime number\n");
    exit(1);
  }
  // 没报错
  printf("prime %d\n", first);
  
  int num;
  int flag = read(lpipe[RD], &num, INT_LEN);
  
  if(!flag){
    exit(0);
  }
  
  int new_pipe[2];
  pipe(new_pipe);
  int pid = fork();
  if(pid == -1){
    fprintf(2, "error fork() in child\n");
    exit(1);
  } else if(pid == 0){
    // 子进程
    primes(new_pipe);
  } else {
    // 父进程
    close(new_pipe[RD]);
    
    if(num % first){
      write(new_pipe[WR], &num, INT_LEN);
    }
    
    while(read(lpipe[RD], &num, INT_LEN) > 0){
      if(num % first){
        write(new_pipe[WR], &num, INT_LEN);
      }
    }
    close(new_pipe[WR]);
    close(lpipe[RD]);
    wait(0); // 不wait会出错
  }
  
  exit(0);
}

int main(int argc, char const *argv[]){
  int p[2];
  pipe(p);
  
  int pid = fork();
  
  
  if(pid == -1){
    fprintf(2, "error fork() \n");
    exit(1);
  } else if(pid == 0){
    // 子进程
    primes(p);
  } else {
    // 父进程
    close(p[RD]);
    for(int i = 2; i <= 35; i++){
      if(write(p[WR], &i, INT_LEN) != INT_LEN){
        fprintf(2, "error when write %d\n", i);
        exit(1);
      }
    }
    close(p[WR]);  
    wait(0);
  }
  
  exit(0);
}
	
