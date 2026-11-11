#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int is_separator(char c){
  return c == ' ' || c == '\n';
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs <command> [args]\n");
    exit(1);
  }
  int i = 0, j = 0, k = 0, p = 0;
  char path[512], buf[512];
  char *argvs_for_exec[MAXARG+1];
  strcpy(path, argv[1]);
  for(i = 1, j = 0; i < argc; i++, j++){
    if(j >= MAXARG){
      fprintf(2, "xargs: args too long\n");
      exit(1);
    }

    argvs_for_exec[j] = argv[i];
  }
  
  if(j >= MAXARG){
    fprintf(2, "xargs: args too long\n");
    exit(1);
  }

  while(1){
    int c = read(0, buf, sizeof(buf) - 1);
    if(c <= 0 || c >= sizeof(buf) - 1){
      break;
    }
    buf[c] = '\0';
    
    k = j;
    p = 0;
    while(p < c){
      if(k >= MAXARG){
        fprintf(2, "xargs: args too long\n");
        exit(1);
      }
      if(!is_separator(buf[p]) && (p == 0 || buf[p-1] == '\0')){
        argvs_for_exec[k++] = &buf[p];
      }
      
      if(is_separator(buf[p])){
        buf[p] = '\0';
      }
      ++p;
    }

    argvs_for_exec[k] = 0;

    if(fork() == 0){
      exec(path, argvs_for_exec);
      fprintf(2, "xargs: exec failed\n");
      exit(1);
    }else{
      wait(0);
    }
  }

  exit(0);
}