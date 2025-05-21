#include <stdio.h>

int strrightindex(char s[], char t[]) {
  int i, j, k;
  for (i = 0; s[i] != '\0'; i++);
  i--;
  for (; i >= 0; i--) {
    for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++);
    if (k > 0 && t[k] == '\0')
      return i;
  }
  return -1;
}

int main(int argc, char* argv[]) {
  char s[100] = "abcdefabcdef";
  char t[100] = "cdef";
  printf("%d\n", strrightindex(s, t));
}