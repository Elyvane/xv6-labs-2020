#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int tick_num = 0;

  if(argc != 2){
    fprintf(2, "Usage: sleep ticks\n");
    exit(1);
  }

  tick_num = atoi(argv[1]);
  if(sleep(tick_num) < 0){
    fprintf(2, "sleep for %s failed\n", argv[1]);
    exit(1);
  }

  exit(0);
}
