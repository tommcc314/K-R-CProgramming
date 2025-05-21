#include <stdio.h>

int recreverse(char s[], int idx) {
  char c = s[idx];
  if (c == '\0') return 0;
  int newidx = recreverse(s, idx + 1);
  s[newidx] = c;
  return newidx + 1;
}

int main(int argc, char* argv[]) {
  char s[100] = "lmao";
  recreverse(s, 0);
  printf("%s\n", s);
}