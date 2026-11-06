#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

static int
testcall(void)
{
  uint64 invalid = (uint64)sbrk(0) + PGSIZE;

  if (sysinfo((struct sysinfo *)invalid) != -1)
    return -1;
  if (sysinfo((struct sysinfo *)(uint64)0xdeadbeefdeadbeefULL) != -1)
    return -1;
  return 0;
}

static int
testmem(void)
{
  struct sysinfo before, allocated, restored;
  int allocated_one_page;

  if (sysinfo(&before) < 0 || sbrk(PGSIZE) == SBRK_ERROR)
    return -1;

  if (sysinfo(&allocated) < 0) {
    sbrk(-PGSIZE);
    return -1;
  }
  allocated_one_page =
    before.freemem >= allocated.freemem &&
    before.freemem - allocated.freemem == PGSIZE;

  if (sbrk(-PGSIZE) == SBRK_ERROR || sysinfo(&restored) < 0)
    return -1;

  if (!allocated_one_page || restored.freemem != before.freemem)
    return -1;
  return 0;
}

static int
testproc(void)
{
  struct sysinfo before, childinfo, after;
  int pid, status;

  if (sysinfo(&before) < 0)
    return -1;
  pid = fork();
  if (pid < 0)
    return -1;
  if (pid == 0) {
    if (sysinfo(&childinfo) < 0 || childinfo.nproc != before.nproc + 1)
      exit(1);
    exit(0);
  }

  if (wait(&status) != pid || status != 0)
    return -1;
  if (sysinfo(&after) < 0 || after.nproc != before.nproc)
    return -1;
  return 0;
}

int
main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  if (testcall() < 0) {
    fprintf(2, "sysinfotest: testcall failed\n");
    exit(1);
  }
  if (testmem() < 0) {
    fprintf(2, "sysinfotest: testmem failed\n");
    exit(1);
  }
  if (testproc() < 0) {
    fprintf(2, "sysinfotest: testproc failed\n");
    exit(1);
  }

  printf("sysinfotest: OK\n");
  exit(0);
}
