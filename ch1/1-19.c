#include <stdio.h>
#define MAXLINE 1000

int custom_getline(char s[], int lim);
void revstring(char s[], int len);

int main(int argc, char* argv[]) {
  char line[MAXLINE];
  int len;
  while ((len = custom_getline(line, MAXLINE)) > 0) {
    revstring(line, len);
    printf("%s", line);
  }
  
  return 0;
}

int custom_getline(char s[], int lim) {
  int c, i;
  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    s[i] = c;
  }
  if (c == '\n') s[i++] = c;
  s[i] = '\0';
  return i;
}

void revstring(char s[], int len) {
  for (int i = 0, j = len - 1; i < j; ++i, --j) {
    char c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}