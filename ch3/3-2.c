#include <stdio.h>

void escape(char s[], char t[]) {
  int i = 0, j = 0;
  for (; s[i] != '\0'; ++i) {
    switch (s[i]) {
      case '\n':
        t[j++] = '\\';
        t[j++] = 'n';
        break;
      case '\t':
        t[j++] = '\\';
        t[j++] = 't';
        break;
      default:
        t[j++] = s[i];
        break;
    }
  }
  t[j] = '\0';
}

int main(int argc, char* argv[]) {
  char s[100] = "lmao\nomg\tbruh\0";
  char t[100];
  escape(s, t);
  printf("%s\n", s);
  printf("%s\n", t);
}