#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *proc = (char *)malloc(14 * sizeof(char));
  strcpy(proc, "Process");
  int n;
  printf("%s\n", proc);
  scanf("%d", &n);
  printf("%s Terminated\n", proc);

  return 0;
}