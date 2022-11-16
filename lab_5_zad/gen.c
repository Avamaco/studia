#include <stdio.h>
#include <stdlib.h>

int less_int(const void * a, const void * b) {
  int _a = *(int*)a;
  int _b = *(int*)b;
  if (_a < _b) return -1;
  else if (_a == _b) return 0;
  else return 1;
}

int main() {
  int r, n, k, d;
  scanf("%d%d%d%d", &r, &n, &k, &d);
  srand(r);
  printf("%d\n", n);
  int *t = (int *)malloc(n * sizeof(int));
  for (int i = 0; i < n; ++i)
    t[i] = rand() % d;
  qsort(t, n, sizeof(int), less_int);
  for (int i = 0; i < n; ++i)
    printf("%d %d\n", rand() % k + 1, t[i]);
  free(t);
}