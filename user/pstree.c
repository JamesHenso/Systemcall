#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/ptree.h"
#include "user/user.h"

// Dat cac mang lon tren BSS de khong lam tran User Stack (4KB)
static struct ptreeinfo procs[NPROC];
static int visited[NPROC];

static void
sort_procs(struct ptreeinfo *p_list, int n)
{
  int i, j;
  struct ptreeinfo tmp;

  for (i = 0; i < n - 1; i++) {
    for (j = i + 1; j < n; j++) {
      if (p_list[j].pid < p_list[i].pid) {
        tmp = p_list[i];
        p_list[i] = p_list[j];
        p_list[j] = tmp;
      }
    }
  }
}

static int
has_parent_in_list(struct ptreeinfo *p_list, int n, int idx)
{
  int i;
  int ppid = p_list[idx].ppid;

  if (ppid <= 0 || ppid == p_list[idx].pid)
    return 0;
  for (i = 0; i < n; i++) {
    if (p_list[i].pid == ppid)
      return 1;
  }
  return 0;
}

static void
print_subtree(struct ptreeinfo *p_list, int n, int idx, int depth)
{
  int i;

  visited[idx] = 1;
  for (i = 0; i < depth; i++) {
    printf("  ");
  }
  printf("%d %s state=%d mem=%lu\n", p_list[idx].pid, p_list[idx].name,
         p_list[idx].state, p_list[idx].memsize);

  for (i = 0; i < n; i++) {
    if (!visited[i] && p_list[i].ppid == p_list[idx].pid) {
      print_subtree(p_list, n, i, depth + 1);
    }
  }
}

int
main(int argc, char *argv[])
{
  int n, i;

  (void)argc;
  (void)argv;

  n = ptree(procs, NPROC);
  if (n < 0) {
    fprintf(2, "pstree: ptree failed\n");
    exit(1);
  }

  sort_procs(procs, n);
  memset(visited, 0, sizeof(visited));

  for (i = 0; i < n; i++) {
    if (!visited[i] && !has_parent_in_list(procs, n, i)) {
      print_subtree(procs, n, i, 0);
    }
  }

  for (i = 0; i < n; i++) {
    if (!visited[i]) {
      print_subtree(procs, n, i, 0);
    }
  }

  exit(0);
}
