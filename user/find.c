#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *path, const char* filename){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }
  
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot fstat %s\n", path);
    close(fd);
    exit(1);
  }
  
  if(st.type != T_DIR){
    fprintf(2, "usage: find <DIRETORY> <filename>\n");
    exit(1);
  }
  
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "find: path too long\n");
    exit(1);
  }
  
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';   // p指向最后一个'/'之后
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) continue;
    memmove(p, de.name, DIRSIZ);  // 添加路径名
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", path);
      exit(1);
    }
    
    // 不要在 . 和 .. 目录中递归
    if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0){
      find(buf, filename);
    } else if(strcmp(filename, p) == 0){
      printf("%s\n", buf);
    }
  }
  
  close(fd);
}


int main(int argc, char *argv[]){
  if (argc != 3) {
    fprintf(2, "usage: find <directory> <filename>\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
	
