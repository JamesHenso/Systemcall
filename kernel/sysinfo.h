#ifndef _SYSINFO_H_
#define _SYSINFO_H_

#include "types.h"

struct sysinfo {
  uint64 freemem;
  int nproc;
};

#endif
