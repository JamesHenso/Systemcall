#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct sysinfo info;
  int rc;

  (void)argc;
  (void)argv;

  rc = sysinfo(&info);
  if (rc < 0) {
    fprintf(2, "sysinfotest: sysinfo failed\n");
    exit(1);
  }

  if (info.freemem == 0 || info.nproc < 1) {
    fprintf(2, "sysinfotest: bad values\n");
    exit(1);
  }

  printf("sysinfotest: OK\n");
  exit(0);
}
