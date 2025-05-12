#include <stdio.h>

void getstr(char s[], int lim) {
  int i = 0;
  char c;
  while ((c = getchar()) != '\n' && i < lim - 1) {
    s[i++] = c;
  }
  s[i] = '\0';
}

void squeeze(char s1[], char s2[]) {
  int i = 0, j = 0;
  for (; s1[i] != '\0'; ++i) {
    int safe = 1;
    for (int k = 0; s2[k] != '\0'; ++k) {
      if (s1[i] == s2[k]) {
        safe = 0;
        break;
      }
    }
    if (safe) s1[j++] = s1[i];
  }
  s1[j] = '\0';
}

int main(int argc, char* argv[]) {
  char s1[100], s2[100];
  getstr(s1, 100);
  getstr(s2, 100);
  squeeze(s1, s2);
  printf("s1: %s\n", s1);
  return 0;
}