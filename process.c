#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *m = (char *)malloc(14 * sizeof(char));
  strcpy(m, "Process");
  int n;
  printf("%s\n", m);
  scanf("%d", &n);
  printf("%s Terminated\n", m);

  return 0;
}
