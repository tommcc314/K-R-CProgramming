#include <stdio.h>

int main(int argc, char* argv[]) {
  int c, nchar[10] = {0}, maxcnt = 0;
  char hist = 'O', blank = ' ';
  while ((c = getchar()) != EOF) {
    if (c >= '0' && c <= '9') {
      ++nchar[c - '0'];
    }
  }
  for (int i = 0; i < 10; ++i) {
    if (nchar[i] > maxcnt) {
      maxcnt = nchar[i];
    }
  }
  for (int i = maxcnt; i >= 1; --i) {
    for (int j = 0; j < 10; ++j) {
      if (nchar[j] >= i) {
        printf("%c ", hist);
      }
      else {
        printf("%c ", blank);
      }
    }
    printf("\n");
  }
  for (int i = 0; i < 10; ++i) {
    printf("%d ", i);
  }
  printf("\n");
  return 0;
}