#ifndef PTREE_H
#define PTREE_H

#include "types.h"

struct ptreeinfo {
  int pid;
  int ppid;
  int state;
  uint64 memsize;
  char name[16];
};

#endif
