#include <stdio.h>

void customstrcat(char *s, char *t) {
  while (*s != '\0') s++;
  while (*t != '\0') *s++ = *t++;
  *s = *t;
}

int main(int argc, char* argv[]) {
  char s[100] = "lmao";
  char t[100] = "xd";
  customstrcat(s, t);
  printf("%s\n", s);
}